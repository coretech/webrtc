#ifndef PC_CONFERENCE_SOURCE_H_
#define PC_CONFERENCE_SOURCE_H_

#include "api/audio/audio_mixer.h"
#include "modules/audio_processing/include/audio_processing.h"
#include "rtc_base/logging.h"

class ConferenceSource:public webrtc::AudioMixer::Source {
public:
	explicit ConferenceSource(){}

    void OnAudioFrame(webrtc::AudioFrame* frame) {
        audio_frame_ = frame;
    }

	AudioFrameInfo GetAudioFrameWithInfo(int target_rate_hz, webrtc::AudioFrame* frame) override {
        if (frame != NULL) {
            frame->samples_per_channel_ = audio_frame_->samples_per_channel();
		    frame->num_channels_ = audio_frame_->num_channels();
		    frame->sample_rate_hz_ = audio_frame_->sample_rate_hz();

            if (audio_frame_->data() && frame->mutable_data()) {
                memcpy(frame->mutable_data(), audio_frame_->data(), sizeof(int16_t) * audio_frame_->samples_per_channel());
            }
        }
	    return AudioFrameInfo::kNormal;
	}
    void SetSsrc(int ssrc) { ssrc_ = ssrc; }
	int Ssrc() const override { return ssrc_; }
	int PreferredSampleRate() const override { return webrtc::AudioProcessing::NativeRate::kSampleRate8kHz; }
private:
    webrtc::AudioFrame* audio_frame_;
    int ssrc_;
};

#endif // PC_CONFERENCE_SOURCE_H_