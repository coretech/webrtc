#include <vector>
#include <rtc_base/checks.h>
#include <modules/audio_mixer/audio_mixer_impl.h>
#include "rtc_base/logging.h"

#include "conference_audio_mixer.h"
#include "conference_audio_mixer_global.h"
#include "conference_mixer_source.h"

namespace conference_audio_mixer {

std::shared_ptr<ConferenceAudioMixerApi> ConferenceAudioMixerApi::Create(const ConferenceMixerConfig& config) {
    return std::make_shared<ConferenceAudioMixer>(config);
}

ConferenceAudioMixer::ConferenceAudioMixer(const ConferenceMixerConfig& config)
        : mixer_(webrtc::AudioMixerImpl::Create()),
          mixed_frame_(std::make_unique<webrtc::AudioFrame>()),
          output_sample_rate_(config.output_sample_rate),
          output_channel_num_(config.output_channel_num) {
    RTC_CHECK(config.frame_duration_ms <= webrtc::AudioMixerImpl::kFrameDurationInMs)
    << "frame duration too long";

    frame_duration_ms_ = config.frame_duration_ms;
    report_output_samples_ =
            output_sample_rate_ / (1000 / webrtc::AudioMixerImpl::kFrameDurationInMs);
    real_output_samples_ = output_sample_rate_ / (1000 / frame_duration_ms_);

    mixed_frame_->UpdateFrame(0, nullptr, static_cast<size_t>(report_output_samples_),
                              output_sample_rate_,
                              webrtc::AudioFrame::SpeechType::kUndefined,
                              webrtc::AudioFrame::VADActivity::kVadUnknown,
                              static_cast<size_t>(output_channel_num_));
}

ConferenceAudioMixer::~ConferenceAudioMixer() {
    for (const auto& item : sources_) {
        mixer_->RemoveSource(item.second.get());
    }

    sources_.clear();
}

void ConferenceAudioMixer::ClearReceivers() {
    std::map<int32_t, std::shared_ptr<ConferenceAudioSource>>::iterator it = sources_.begin();
    while (it != sources_.end()) {
        if (it->first != kLocalSsrc) {
            mixer_->RemoveSource(it->second.get());
            it = sources_.erase(it);
        } else {
            ++it;
        }
    }
}

void ConferenceAudioMixer::UpdateVolume(int32_t ssrc, float volume) {
    auto source = sources_.find(ssrc);
    if (source != sources_.end()) {
        source->second->UpdateVolume(volume);
    }
}

bool ConferenceAudioMixer::AddSource(const ConferenceMixerSource& source) {
    std::shared_ptr<ConferenceAudioSource> audio_source = DoAddSource(source);
    mixer_->AddSource(audio_source.get());
    return true;
}

bool ConferenceAudioMixer::RemoveSource(int32_t ssrc) {
    auto source = sources_.find(ssrc);
    if (source != sources_.end()) {
        mixer_->RemoveSource(source->second.get());
        sources_.erase(source);

        return true;
    }

    return false;
}

webrtc::AudioFrame* ConferenceAudioMixer::Mix() {
    mixer_->Mix(static_cast<size_t>(output_channel_num_), mixed_frame_.get());
    return mixed_frame_.get();
}

bool ConferenceAudioMixer::AddRecordedData(int32_t ssrc, const void* data, int32_t size) {
    auto source = sources_.find(ssrc);
    if (source != sources_.end()) {
        reinterpret_cast<ConferenceAudioRecordSource*>(source->second.get())->OnAudioRecorded(data, size);
        return true;
    }
    return false;
}

std::shared_ptr<ConferenceAudioSource> ConferenceAudioMixer::DoAddSource(const ConferenceMixerSource& source) {
    RTC_CHECK(source.sample_rate == output_sample_rate_)
    << "record source must have the same sample rate as output";
    RTC_CHECK(source.channel_num == output_channel_num_)
    << "record source must have the same channels as output";

    std::shared_ptr<ConferenceAudioRecordSource> record_source = std::make_shared<ConferenceAudioRecordSource>(
            source.ssrc, output_sample_rate_, output_channel_num_, frame_duration_ms_,
            source.volume
    );
    sources_.insert(std::pair<int32_t, std::shared_ptr<ConferenceAudioSource>>(
            source.ssrc, record_source
    ));

    return record_source;
}

}