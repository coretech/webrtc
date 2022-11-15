#pragma once

#include <rtc_base/buffer.h>

#include "conference_audio_source.h"

namespace conference_audio_mixer {

class ConferenceAudioRecordSource : public ConferenceAudioSource {
public:
    ConferenceAudioRecordSource(int32_t ssrc, int32_t sample_rate, int32_t channel_num,
                      int32_t frame_duration_ms, float volume);

    ~ConferenceAudioRecordSource();

    void OnAudioRecorded(const void* data, int32_t size);

    AudioFrameInfo
    GetAudioFrameWithInfo(int32_t sample_rate_hz, webrtc::AudioFrame* audio_frame) override;

    int32_t Ssrc() const override;

    int32_t PreferredSampleRate() const override;

private:
    int32_t ssrc_;

    int32_t sample_rate_;
    int32_t channel_num_;

    int32_t frame_duration_ms_;
    int32_t report_output_samples_;
    int32_t real_buffer_num_elements_;

    rtc::BufferT<int16_t> buffer_;
};

}