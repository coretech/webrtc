#pragma once

#include <cstdint>
#include <memory>

namespace conference_audio_mixer {

struct ConferenceMixerConfig;
struct ConferenceMixerSource;

class ConferenceAudioMixerApi {
public:
    virtual ~ConferenceAudioMixerApi() {}

    static constexpr int32_t MAX_BUF_SIZE = 7680;

    static constexpr int32_t SAMPLE_SIZE = 2;

    static std::shared_ptr<ConferenceAudioMixerApi> Create(const ConferenceMixerConfig & config);

    virtual void UpdateVolume(int32_t ssrc, float volume) = 0;

    virtual bool AddSource(const ConferenceMixerSource & source) = 0;

    virtual bool RemoveSource(int32_t ssrc) = 0;
};

}  // namespace conference_audio_mixer