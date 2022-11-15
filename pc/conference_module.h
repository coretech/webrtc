#ifndef PC_CONFERENCE_MODULE_H_
#define PC_CONFERENCE_MODULE_H_

#include <map>
#include "rtc_base/logging.h"
#include "conference_audio_mixer.h"
#include "conference_mixer_config.h"
#include "conference_mixer_source.h"

class ConferenceModule {

private:
    static constexpr int32_t MIN_CONF_STREAMS = 2;
    static constexpr int32_t MAX_CONF_STREAMS = 2;
    static constexpr int32_t kVolume = 10;
    static constexpr int32_t kLocalSsrc = 0;

    static ConferenceModule* instance_;
    bool is_merged_ = false;
    int32_t output_sample_rate_;
    int32_t output_channel_num_;
    int32_t frame_duration_ms_;
    std::map<uint32_t, conference_audio_mixer::ConferenceAudioMixer*> conference_mixers_;
protected:
    ConferenceModule() {}
    ~ConferenceModule() {}
public:

    ConferenceModule(ConferenceModule &other) = delete;
    void operator=(const ConferenceModule &) = delete;

    static ConferenceModule* GetInstance();

    void Config(int32_t output_sample_rate, int32_t output_channel_num, int32_t frame_duration_ms) {
        output_sample_rate_ = output_sample_rate;
        output_channel_num_ = output_channel_num;
        frame_duration_ms_ = frame_duration_ms;
    }

    bool AddStream(uint32_t send_stream_ssrc) {
        conference_audio_mixer::ConferenceMixerConfig mixerConfig(output_sample_rate_, output_channel_num_, frame_duration_ms_);
        conference_audio_mixer::ConferenceAudioMixer* conference_audio_mixer = new conference_audio_mixer::ConferenceAudioMixer(mixerConfig);

        const conference_audio_mixer::ConferenceMixerSource conference_mixer_local_source(kLocalSsrc, kVolume, output_sample_rate_, output_channel_num_);
        conference_audio_mixer->AddSource(conference_mixer_local_source);

        conference_mixers_.insert({ send_stream_ssrc, conference_audio_mixer });
        return true;
    }

    bool RemoveStream(uint32_t send_stream_ssrc) {
        for (auto const& mixer : conference_mixers_) {
            mixer.second->RemoveSource(send_stream_ssrc);
        }

        auto mixer = conference_mixers_.find(send_stream_ssrc);
        if (mixer != conference_mixers_.end()) {
            mixer->second->RemoveSource(kLocalSsrc);
            conference_mixers_.erase(mixer);
            delete mixer->second;
        }

        if (is_merged_ && conference_mixers_.size() < MIN_CONF_STREAMS) {
            Unmerge();
        }

        return true;
    }

    bool Merge() {
        if (is_merged_) {
            return true;
        }

        if (conference_mixers_.size() < MIN_CONF_STREAMS) {
            return false;
        }

        if (conference_mixers_.size() > MAX_CONF_STREAMS) {
            return false;
        }

        for (auto const& mixer1 : conference_mixers_) {
            uint32_t send_stream_ssrc1 = mixer1.first;

            for (auto const& mixer2 : conference_mixers_) {
                uint32_t send_stream_ssrc2 = mixer2.first;

                if (send_stream_ssrc1 != send_stream_ssrc2) {
                    const conference_audio_mixer::ConferenceMixerSource conference_mixer_source(send_stream_ssrc2, kVolume, output_sample_rate_, output_channel_num_);
                    mixer1.second->AddSource(conference_mixer_source);
                }
            }
        }

        is_merged_ = true;

        return true;
    }

    bool Unmerge() {
        for (auto const& mixer : conference_mixers_) {
            mixer.second->ClearReceivers();
        }

        is_merged_ = false;

        return true;
    }

    bool IsMerged() {
        return is_merged_;
    }

    void AddLocalData(uint32_t send_stream_ssrc, const void* data, int32_t size) {
        auto mixer = conference_mixers_.find(send_stream_ssrc);
        if (mixer != conference_mixers_.end()) {
            mixer->second->AddRecordedData(kLocalSsrc, data, size);
        }
    }

    void AddRemoteData(uint32_t send_stream_ssrc, const void* data, int32_t size) {
        for (auto const& mixer : conference_mixers_) {
            if (send_stream_ssrc != mixer.first) {
                mixer.second->AddRecordedData(send_stream_ssrc, data, size);
            }
        }
    }

    webrtc::AudioFrame* Mix(uint32_t send_stream_ssrc) {
        auto mixer = conference_mixers_.find(send_stream_ssrc);
        if (mixer != conference_mixers_.end()) {
            return mixer->second->Mix();
        }
        return nullptr;
    }
};

#endif  // PC_CONFERENCE_MODULE_H_
