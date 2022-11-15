#include "conference_module.h"

ConferenceModule* ConferenceModule::instance_{nullptr};

ConferenceModule *ConferenceModule::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new ConferenceModule();
    }
    return instance_;
}