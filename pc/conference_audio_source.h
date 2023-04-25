#pragma once

#include <api/audio/audio_mixer.h>

namespace conference_audio_mixer {

class ConferenceAudioSource : public webrtc::AudioMixer::Source {
public:
    ConferenceAudioSource(float volume);

    void ApplyVolume(webrtc::AudioFrame* frame);

    void UpdateVolume(float volume);

private:
    float volume_;
};

}