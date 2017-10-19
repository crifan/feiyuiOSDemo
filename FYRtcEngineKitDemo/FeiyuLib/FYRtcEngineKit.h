//
//  FYRtcEngineKit.h
//  FyCloud
//
//  Created by zhangyusheng on 2017/8/17.
//  Copyright © 2017年 feiyu. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, FYRtcErrorCode) {
    FeiyuRtc_Error_Init = 200000,
    FeiyuRtc_Error_InvalidArgument = 200001,
    FeiyuRtc_Error_DialBack = 200002,
    FeiyuRtc_Error_Microphone_Authority = 200003,
    FeiyuRtc_Error_Call_Current_Status_Is_InAvailable = 200004,
    FeiyuRtc_Error_JionChannel = 200005,
    
    FeiyuRtc_Error_Call_CalleePrepare = 200101,
    FeiyuRtc_Error_Call_Failed = 200102,
    FeiyuRtc_Error_Call_NotCall = 200103,
    FeiyuRtc_Error_Call_Forbidden = 200104,
    FeiyuRtc_Error_Call_TemporarilyUnavailable= 200105,
    FeiyuRtc_Error_Call_BusyHere = 200106,
    FeiyuRtc_Error_Call_CallDeclined = 200107,
    FeiyuRtc_Error_Call_ServiceUnavailable = 200108,
    FeiyuRtc_Error_Call_NotOnline = 200109,
    FeiyuRtc_Error_Call_NotNetwork_Exception = 200110,
    FeiyuRtc_Error_Call_CalleeNotfound = 200111,
    FeiyuRtc_Error_Call_RequestTimeout = 200112,
    FeiyuRtc_Error_Call_RequestTerminated = 200113,
    FeiyuRtc_Error_Call_IntervalTooBrief = 200114,
};



@interface FYError :NSObject
@property (assign ,nonatomic) NSInteger code;
@property (copy ,nonatomic) NSString *msg;
@end

@interface FYOptionData :NSObject
@property (nonatomic, assign)   BOOL isRecord;
@property (nonatomic, assign)   int maxDuration;
@property (nonatomic, assign)   NSString *extraData;
@end

@interface FYRtcEngineStatus :NSObject
@property (nonatomic ,assign) float downBandwidth;
@property (nonatomic ,assign) float upBandwidth;
@property (nonatomic ,assign) float sendLossRate;
@property (nonatomic ,assign) float recvLossRate;
@property (nonatomic ,assign) float sendBytes;
@property (nonatomic ,assign) float recvBytes;

@end

@class FYRtcEngineKit;
@protocol FYRtcEngineKitDelegate <NSObject>
@optional
/**
 * Event of remote user joined.
 *
 * @param engine     FYRtcEngineKit
 *
 * @param uid        The remote user id
 */
- (void)onFYRtcEngine:(FYRtcEngineKit *)engine didJoinOfUid:(NSString *)uid;

/**
 * Event of remote user Leaved.
 *
 * @param engine     FYRtcEngineKit
 *
 * @param uid        The remote user id
 */
- (void)onFYRtcEngine:(FYRtcEngineKit *)engine didLeaveOfUid:(NSString *)uid;

/**
 * Event of local user leaved.
 *
 * @param engine     FYRtcEngineKit
 *
 * @param channelId  channelId
 *
 * @param status     FYRtcEngineStatus
 */
- (void)onFYRtcEngine:(FYRtcEngineKit *)engine didLeaveChannel:(NSString *)channelId Stats:(FYRtcEngineStatus *)status;

/**
 * Event of local user joined.
 *
 * @param engine     FYRtcEngineKit
 *
 * @param channelId  Channel Id
 *
 * @param uid        User Id
 */
- (void)onFYRtcEngine:(FYRtcEngineKit *)engine didJoinChannel:(NSString *)channelId Uid:(NSString *)uid;


/**
 * Rtc status. Updated every two seconds.
 *
 * @param engine     FYRtcEngineKit
 *
 * @param status     FYRtcEngineStatus
 *
 */
- (void)onFYRtcEngine:(FYRtcEngineKit *)engine reportRtcStats:(FYRtcEngineStatus *)status;

/**
 * Event of user audio muted or unmuted
 *
 * @param engine     FYRtcEngineKit
 *
 * @param muted      Muted or unmuted
 *
 * @param uid        The user id
 */
- (void)onFYRtcEngine:(FYRtcEngineKit *)engine didAudioMuted:(BOOL)muted Uid:(NSString *)uid;

/**
 * Event of the incoming call.
 *
 * @param engine  FYRtcEngineKit
 *
 * @param caller  caller
 */
- (void)onFYRtcEngine:(FYRtcEngineKit *)engine IncomingCall:(NSString *)caller;

/**
 * Event of the outgoing call.
 *
 * @param engine  FYRtcEngineKit
 *
 * @param callee  callee number
 */
- (void)onFYRtcEngine:(FYRtcEngineKit *)engine OutgoingCall:(NSString *)callee Uid:(NSString *)uid;

/**
 * Event of call connected.
 *
 * @param engine  FYRtcEngineKit
 */
- (void)onFYRtcEngineCallConnect:(FYRtcEngineKit *)engine;

/**
 * Event of call ended.
 *
 * @param engine  FYRtcEngineKit
 *
 * @param status  FYRtcEngineStatus
 */
- (void)onFYRtcEngine:(FYRtcEngineKit *)engine CallEnd:(FYRtcEngineStatus *)status;

/**
 * Event of call prepare receive call sucess
 *
 * @param engine     FYRtcEngineKit
 *
 * @param muted      Muted or unmuted
 *
 * @param uid        The user id
 */
- (void)onFYRtcEngineCalleePrepareSucess:(FYRtcEngineKit *)engine;

/**
 * The error occurred in SDK, app need to handle it.
 *
 * @param engine  FYRtcEngineKit
 *
 * @param error   FYError
 */
- (void)onFYRtcEngine:(FYRtcEngineKit *)engine OnError:(FYError *)error;

/**
 * Event of the DialBack success.
 *
 * @param engine  FYRtcEngineKit
 */
- (void)onFYRtcEngineDialBackSuccess:(FYRtcEngineKit *)engine;

@end

@interface FYRtcEngineKit : NSObject
/**
 * Init FYRtcEngineKit.
 *
 * @param appId     The appId is issued to the application developers by Feiyu Cloud.
 
 * @param appToken  The appToken is issued to the application developers by Feiyu Cloud.
 *
 * @param delegate  FYRtcEngineKitDelegate
 *
 * @return FYRtcEngineKit
 */
+ (instancetype)sharedEngineWithAppId:(NSString *)appId AppToken:(NSString *)appToken delegate:(id <FYRtcEngineKitDelegate>)delegate;

/**
 * Create an open UDP socket to the FYRtcEngineKit cloud service to join a channel.
 Users in the same channel can talk to each other with same appId.
 Users using different appID cannot call each other.
 *
 * @param channelId  Joining in the same channel indicates those clients have entered in one room.
 *
 * @param uid        Optional, this argument is the unique ID for each member in one channel.
 If not specified, set to nil, the SDK automatically allocates an ID, and the id could be gotten in join Channel success call back.
 *
 * @param optionData      Add optional options (isRecord/maxDuration/extraData)
 */
- (void)joinChannel:(NSString *)channelId Uid:(NSString *)uid OptionData:(FYOptionData *)optionData JoinSuccess:(void(^)(NSString *uid, NSString *channel))joinChannelBlock;

/**
 * Create an open UDP socket to the FYRtcEngineKit cloud service to call PSTN number.
 * Users using different appId cannot call each other.
 *
 * @param calleeNumber    callee's phone number. the format is PSTN E164. eg. Chinese number: +86136********
 *
 * @param uid             Optional, this argument is the unique ID for caller in a call.
                          If not specified, set to nil, the SDK automatically allocates an ID, and the id could be gotten in outgoingCall or callConnect event.
 *
 * @param display         display number. the format is PSTN E164. eg. Chinese number: +86136********
 *
 * @param optionData      Add optional options (isRecord/maxDuration/extraData)
 */
- (void)dialPstn:(NSString *)calleeNumber CallerUid:(NSString *)uid Display:(NSString *)display OptionData:(FYOptionData *)optionData;

/**
 * Create an open UDP socket to the FYRtcEngineKit cloud service to call remote FYRtcEngineKit cloud client.
   Users using different appId cannot call each other.
 *
 * @param calleeUid    callee's user id
 *
 * @param uid          Optional, this argument is the unique ID for caller in a call.
                       If not specified, or set to nil, the SDK automatically allocates an ID, and the id could be gotten in outgoingCall or callConnect
 *
 * @param optionData      Add optional options (isRecord/maxDuration/extraData)
 */
- (void)dialPeer:(NSString *)calleeUid CallerUid:(NSString *)uid OptionData:(FYOptionData *)optionData;

/**
 * Create a dialBack. Call the callee succeed then call callerNumber.
 *
 * @param calleeNumber       callee's phone number. the format is PSTN E164. eg. Chinese number: +86136********
 *
 * @param calleeDisplay      display number. the format is PSTN E164. eg. Chinese number: +86136********
 *
 * @param callerNumber       caller's phone number. the format is PSTN E164. eg. Chinese number: +86136********
 *
 * @param callerDisplay      display number. the format is PSTN E164. eg. Chinese number: +86136********
 *
 * @param optionData         Add optional options (isRecord/maxDuration/extraData)
 */
- (void)dialBack:(NSString *)calleeNumber CalleeDisplay:(NSString *)calleeDisplay CallerNumber:(NSString *)callerNumber  CallerDisplay:(NSString *)callerDisplay OptionData:(FYOptionData *)optionData DialPstnSuccess:(void(^)(NSString *dialBackId))dialBackBlock;

/**
 * prepare receive dialPeer call when client as callee.
 *
 * @param calleeUid    this argument is the unique ID for callee in a call.
 */
- (void)calleePrepare:(NSString *)calleeUid prepareSuccess:(void(^)(void))prepareBlock;

/**
 *  Mutes / Unmutes other remote audio without uid
 *
 *  @param enable YES: Mutes other received audio. NO: Unmutes other received audio.
 */
- (void)muteOtherRemoteAudio:(BOOL)enable Uid:(NSString *)uid;

/**
 *  Mutes / Unmutes all remote audio.
 *
 *  @param enable YES: Mutes all remote received audio. NO: Unmutes all remote received audio.
 */
- (void)muteRemoteAudio:(BOOL)enable Uid:(NSString *)uid;
- (void)muteAllRemoteAudio:(BOOL)enable;

/**
 * Mutes / Unmutes local audio.
 *
 * @param enable YES: Mutes the local audio. NO: Unmutes the local audio.
 */
- (void)muteLocalAudio:(BOOL)enable;

/**
 *  Enable / Disable speaker of device
 *
 *  @param enable YES: Switches to speakerphone. NO: Switches to headset.
 */
- (void)enabledSpeaker:(BOOL)enable;

/**
 *  lets the user leave a channel, i.e., hanging up or exiting a call.
    After joining a channel, the user must call the leaveChannel method to end the call before joining another one.
 *
 *  @param leaveChannelBlock  The statistics of the call, including duration, sent bytes and received bytes
 */
- (void)leaveChannel:(void(^)(FYRtcEngineStatus* stat))leaveChannelBlock;

/**
 *  hanging up or exiting a call.
 *
 *  @param endBlock  The statistics of the call, including duration, sent bytes and received bytes
 */
- (void)endCall:(void(^)(FYRtcEngineStatus* stat))endBlock;

/**
 * answer call
 */
- (void)answerCall;

/**
 * Send the specified dtmf.
 *
 * The dtmf is automatically played to the user.
 *
 * @param dtmf The dtmf name specified as a char, must be ’0’,’1’,’2’,’3’,’4’,’5’,’6’,’7’,’8’,’9’,’#’,’*’
 **/
- (void)sendDtmf:(char)dtmf;

/**
 * Sets the path to a wav file used for ringing. The file must be a wav 16bit linear. Local ring is disabled if null.
 *
 * @param path The path to a wav file to be used for ringing
 **/
- (void)setRing:(NSString *)path;

/**
 *  Specifies the SDK output log file.
 *
 *  @param path The full file path of the log file.
 */
- (void)setLogfile:(NSString *)path;

/**
 *  Get the version of Feiyu cloud.
 *
 *  @return string, sdk version
 */
- (NSString *)version;
@end
