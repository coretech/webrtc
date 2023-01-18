#include <audio/utility/audio_frame_operations.h>

#include "conference_audio_source.h"

conference_audio_mixer::ConferenceAudioSource::ConferenceAudioSource(float volume) : volume_(volume) {
}

void conference_audio_mixer::ConferenceAudioSource::ApplyVolume(webrtc::AudioFrame* frame) {
    if (volume_ < 0.99f || volume_ > 1.01f) {
        webrtc::AudioFrameOperations::ScaleWithSat(volume_, frame);
    }
}

void conference_audio_mixer::ConferenceAudioSource::UpdateVolume(float volume) {
    volume_ = volume;
}