
#pragma once

#include <cstdint>
#include <string>
#include <utility>

namespace conference_audio_mixer {

struct ConferenceMixerSource final {

    int32_t ssrc;
    float volume;
    int32_t sample_rate;
    int32_t channel_num;

    ConferenceMixerSource(
                int32_t ssrc_,
                float volume_,
                int32_t sample_rate_,
                int32_t channel_num_)
    : ssrc(std::move(ssrc_))
    , volume(std::move(volume_))
    , sample_rate(std::move(sample_rate_))
    , channel_num(std::move(channel_num_))
    {}
};

}  // namespace conference_audio_mixer