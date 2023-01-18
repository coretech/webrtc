
#import "RTCAudioConference.h"

#include "pc/conference_module.h"

@implementation RTC_OBJC_TYPE (RTCAudioConference) {
}

- (void)merge {
    ConferenceModule::GetInstance()->Merge();
}

- (void)unmerge {
    ConferenceModule::GetInstance()->Unmerge();
}

- (void)mute {
    ConferenceModule::GetInstance()->Mute();
}

- (void)unmute {
    ConferenceModule::GetInstance()->Unmute();
}

@end
