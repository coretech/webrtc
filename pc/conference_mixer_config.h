#pragma once

#include <cstdint>
#include <utility>

namespace conference_audio_mixer {

struct ConferenceMixerConfig final {
    int32_t output_sample_rate;
    int32_t output_channel_num;
    int32_t frame_duration_ms;

    ConferenceMixerConfig(int32_t output_sample_rate_,
                int32_t output_channel_num_,
                int32_t frame_duration_ms_)
    : output_sample_rate(std::move(output_sample_rate_))
    , output_channel_num(std::move(output_channel_num_))
    , frame_duration_ms(std::move(frame_duration_ms_))
    {}
};

}  // namespace conference_audio_mixer