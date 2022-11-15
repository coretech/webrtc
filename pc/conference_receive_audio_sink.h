#ifndef PC_CONFERENCE_RECEIVE_AUDIO_SINK_INTERFACE_H_
#define PC_CONFERENCE_RECEIVE_AUDIO_SINK_INTERFACE_H_

#include "api/call/audio_sink.h"
#include "rtc_base/logging.h"

#include "pc/conference_module.h"
#include "pc/conference_mixer_source.h"
#include "api/audio/audio_frame.h"

class ConferenceReceiveAudioSink : public webrtc::AudioSinkInterface {
 public:
  ConferenceReceiveAudioSink(uint32_t ssrc) : ssrc_(ssrc) {}
  ~ConferenceReceiveAudioSink() {}

  void OnData(const Data& audio) override {
    //const int16_t* data;          The actual 16bit audio data.
    //size_t samples_per_channel;   Number of frames in the buffer.
    //int sample_rate;              Sample rate in Hz.
    //size_t channels;              Number of channels in the audio data.
    //uint32_t timestamp;           The RTP timestamp of the first sample.

    if (ConferenceModule::GetInstance()->IsMerged()) {
        std::unique_ptr<webrtc::AudioFrame> audio_frame(new webrtc::AudioFrame());
        audio_frame->UpdateFrame(
        audio_frame->timestamp_, static_cast<const int16_t*>(audio.data),
        audio.samples_per_channel, audio.sample_rate, audio_frame->speech_type_,
        audio_frame->vad_activity_, audio.channels);

        if (audio_frame) {
            std::unique_ptr<webrtc::AudioFrame> audio_frame_copy(new webrtc::AudioFrame());
            audio_frame_copy->CopyFrom(*audio_frame);

            const size_t length = audio_frame_copy.get()->samples_per_channel() * audio_frame_copy.get()->num_channels();
            const int32_t size = sizeof(int16_t) * length;

            ConferenceModule::GetInstance()->AddRemoteData(ssrc_, audio_frame_copy.get()->data(), size);
        }
    }
  }
private:
  uint32_t ssrc_;
};

#endif  // PC_CONFERENCE_RECEIVE_AUDIO_SINK_INTERFACE_H_