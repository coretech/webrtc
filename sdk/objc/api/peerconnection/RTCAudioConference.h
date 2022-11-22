#import <Foundation/Foundation.h>

#import "RTCMacros.h"

NS_ASSUME_NONNULL_BEGIN

RTC_OBJC_EXPORT
@interface RTC_OBJC_TYPE (RTCAudioConference) : NSObject

- (instancetype)init NS_UNAVAILABLE;

/** Merge calls into an audio conference. */
- (void)merge;

/** Unmerges all call from an audio conference. */
- (void)unmerge;

@end

NS_ASSUME_NONNULL_END
