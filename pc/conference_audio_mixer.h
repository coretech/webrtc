
#pragma once

#include <map>

#include <api/audio/audio_mixer.h>
#include <api/scoped_refptr.h>

#include "conference_audio_mixer_api.h"
#include "conference_mixer_config.h"
#include "conference_audio_source.h"
#include "conference_audio_record_source.h"

namespace conference_audio_mixer {

class ConferenceAudioMixer : public ConferenceAudioMixerApi {
public:
    ConferenceAudioMixer(const ConferenceMixerConfig& config);

    ~ConferenceAudioMixer();

    void UpdateVolume(int32_t ssrc, float volume) override;

    bool AddSource(const ConferenceMixerSource& source) override;

    bool RemoveSource(int32_t ssrc) override;

    void ClearReceivers();

    webrtc::AudioFrame* Mix();

    bool AddRecordedData(int32_t ssrc, const void* data, int32_t size);

private:
    static constexpr int32_t kLocalSsrc = 0;
    std::shared_ptr<ConferenceAudioSource> DoAddSource(const ConferenceMixerSource& source);

    rtc::scoped_refptr<webrtc::AudioMixer> mixer_;
    std::map<int32_t, std::shared_ptr<ConferenceAudioSource>> sources_;
    std::unique_ptr<webrtc::AudioFrame> mixed_frame_;
    int32_t output_sample_rate_;
    int32_t output_channel_num_;

    int32_t frame_duration_ms_;
    int32_t report_output_samples_;
    int32_t real_output_samples_;
};

}