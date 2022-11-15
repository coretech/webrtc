#include <modules/audio_mixer/audio_mixer_impl.h>
#include "rtc_base/logging.h"

#include "conference_audio_record_source.h"
#include "conference_audio_mixer_global.h"

conference_audio_mixer::ConferenceAudioRecordSource::ConferenceAudioRecordSource(int32_t ssrc,
                                                  int32_t sample_rate,
                                                  int32_t channel_num,
                                                  int32_t frame_duration_ms,
                                                  float volume)
        : ConferenceAudioSource(volume),
          ssrc_(ssrc),
          sample_rate_(sample_rate),
          channel_num_(channel_num),
          frame_duration_ms_(frame_duration_ms),
          report_output_samples_(sample_rate / (1000 / webrtc::AudioMixerImpl::kFrameDurationInMs)),
          real_buffer_num_elements_(channel_num * sample_rate / (1000 / frame_duration_ms)) {
    buffer_.Clear();
}

conference_audio_mixer::ConferenceAudioRecordSource::~ConferenceAudioRecordSource() {
    buffer_.Clear();
}

void conference_audio_mixer::ConferenceAudioRecordSource::OnAudioRecorded(const void* data, int32_t size) {
    buffer_.AppendData(static_cast<const int16_t*>(data),
                       static_cast<size_t>(size / sizeof(int16_t)));
}

webrtc::AudioMixer::Source::AudioFrameInfo
conference_audio_mixer::ConferenceAudioRecordSource::GetAudioFrameWithInfo(int32_t sample_rate_hz,
                                                      webrtc::AudioFrame* audio_frame) {

    if (frame_duration_ms_) {/* Compiler check*/}

    if (sample_rate_hz != sample_rate_) {
        return webrtc::AudioMixer::Source::AudioFrameInfo::kError;
    }

    if (buffer_.size() < (size_t) real_buffer_num_elements_) {
        return webrtc::AudioMixer::Source::AudioFrameInfo::kMuted;
    }

    audio_frame->UpdateFrame(0, buffer_.data(),
                             static_cast<size_t>(report_output_samples_),
                             sample_rate_,
                             webrtc::AudioFrame::SpeechType::kNormalSpeech,
                             webrtc::AudioFrame::VADActivity::kVadActive,
                             static_cast<size_t>(channel_num_));

    memmove(buffer_.data(), buffer_.data() + real_buffer_num_elements_,
            (buffer_.size() - real_buffer_num_elements_) * sizeof(int16_t));

    buffer_.SetSize(buffer_.size() - real_buffer_num_elements_);

    ApplyVolume(audio_frame);

    return webrtc::AudioMixer::Source::AudioFrameInfo::kNormal;
}

int32_t conference_audio_mixer::ConferenceAudioRecordSource::Ssrc() const {
    return ssrc_;
}

int32_t conference_audio_mixer::ConferenceAudioRecordSource::PreferredSampleRate() const {
    return sample_rate_;
}
