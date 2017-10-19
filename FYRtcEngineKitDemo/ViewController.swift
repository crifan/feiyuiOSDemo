//
//  ViewController.swift
//  FYRtcEngineKitDemo
//
//  Created by zhangyusheng on 2017/9/23.
//  Copyright © 2017年 zhangyusheng. All rights reserved.
//

import UIKit

let FY_APPID = "FBC74CB1B2D8CFAE88372FCED6AB8EC4"
let FY_APPTOKEN = "27998A17033E4139CC7B3404B4BDF4E7"
//let FY_CALLER_ID = "13800001111"
//let FY_CALLEE_ID = "13922223333"

class ViewController: UIViewController, UITextFieldDelegate, FYRtcEngineKitDelegate{
    let SelfTextFieldTag = 1
    let OtherTextFieldTag = 2
    let SpeakerButtonTag = 3
    let CallButtonTag = 4
    let MuteButtonTag = 5
    
    var selfTextField:UITextField!
    var otherTextField:UITextField!
    var speakerButton:UIButton!
    var callButton:UIButton!
    var muteButton:UIButton!
    
    var isCalling = false
    
    override init(nibName nibNameOrNil: String?, bundle nibBundleOrNil: Bundle?) {
        super.init(nibName: nibNameOrNil, bundle: nibBundleOrNil)
    }
    
    required init?(coder aDecoder: NSCoder) {
        //fatalError("init(coder:) has not been implemented")
        super.init(coder: aDecoder)
    }
    
    var fyRtcEngine: FYRtcEngineKit!
    override func viewDidLoad() {
        super.viewDidLoad()

//        fyRtcEngine = FYRtcEngineKit.sharedEngine(withAppId:"your appid", appToken:"your apptoken", delegate: nil)
//        fyRtcEngine.joinChannel("channelId123", uid: nil, optionData: nil, joinSuccess: nil)
        fyRtcEngine = FYRtcEngineKit.sharedEngine(withAppId: FY_APPID, appToken: FY_APPTOKEN, delegate: self)

        self.selfTextField = self.view.viewWithTag(SelfTextFieldTag) as! UITextField
        self.selfTextField.delegate = self
        self.selfTextField.returnKeyType = .done
        
        self.otherTextField = self.view.viewWithTag(OtherTextFieldTag) as! UITextField
        self.otherTextField.delegate = self
        self.otherTextField.returnKeyType = .done

        self.speakerButton = self.view.viewWithTag(SpeakerButtonTag) as! UIButton
        self.speakerButton.setImage(UIImage(named: "speaker_normal"), for: UIControlState.normal)
        self.speakerButton.setImage(UIImage(named: "speaker_pressed"), for: UIControlState.selected)

        self.callButton = self.view.viewWithTag(CallButtonTag) as! UIButton
        
        self.muteButton = self.view.viewWithTag(MuteButtonTag) as! UIButton
        self.muteButton.setImage(UIImage(named: "mute_normal"), for: UIControlState.normal)
        self.muteButton.setImage(UIImage(named: "mute_pressed"), for: UIControlState.selected)
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(true)
    }

    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(true)
        
        if let sdkVerStr = fyRtcEngine.version() {
            self.infoNotice("飞语SDK版本：\(sdkVerStr)")
            
            self.selfPrepareReceiveCall()
        }
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    @IBAction func speakerBtnEvent(_ sender: UIButton) {
        sender.isSelected = !sender.isSelected
        fyRtcEngine.enabledSpeaker( sender.isSelected)
        self.infoNotice(sender.isSelected ? "开启外放🔉" : "关闭外放🔉")
    }

    @IBAction func muteBtnEvent(_ sender: UIButton) {
        sender.isSelected = !sender.isSelected
        fyRtcEngine.muteLocalAudio(sender.isSelected)
        self.infoNotice(sender.isSelected ? "开启静音🔇" : "取消静音🔇")
    }

    @IBAction func endCallBtnEvent(_ sender: UIButton) {
        //        fyRtcEngine.leaveChannel(nil)

        if (isCalling) {
            fyRtcEngine.endCall(self.endCallCallback)
            self.infoNotice("挂断通话")
        } else {
            let dialOption:FYOptionData = FYOptionData()
            dialOption.isRecord = true
            //dialOption.maxDuration = 60 * 60 * 24
            dialOption.maxDuration = 60 * 10
            let otherUid = self.otherTextField.text!
            let selfUid = self.selfTextField.text!
            fyRtcEngine.dialPeer(otherUid, callerUid: selfUid, optionData: dialOption)
            self.infoNotice("主叫：调用dialPeer")
            
            self.isCalling = true
            self.updateCallButtonImage()
        }
    }
    
    /****************************************************************
     * UITextFieldDelegate
     ****************************************************************/

    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        textField.resignFirstResponder()
        return false
    }
    
    func textFieldShouldEndEditing(_ textField: UITextField) -> Bool {
        textField.resignFirstResponder()
        return true
    }
    
    func textFieldDidEndEditing(_ textField: UITextField) {
        textField.resignFirstResponder()
        
        if textField === selfTextField {
            self.selfPrepareReceiveCall()
        }
    }

    /****************************************************************
     * 自定义函数
     ****************************************************************/
    
    func updateCallButtonImage(){
        dispatchMain_sync{
            if (self.isCalling) {
                //self.callButton.imageView?.image = UIImage(named: "hangup_normal")
                self.callButton.setImage(UIImage(named: "hangup_normal"), for: UIControlState.normal)
            } else {
                //self.callButton.imageView?.image = UIImage(named: "answer_normal")
                self.callButton.setImage(UIImage(named: "answer_normal"), for: UIControlState.normal)
            }
        }
    }
    
    func selfPrepareReceiveCall(){
        let selfUid = self.selfTextField.text!
        fyRtcEngine.calleePrepare(selfUid, prepareSuccess: self.calleePrepareSuccess)
        self.infoNotice("\(selfUid) 准备接听")
    }
    
    func calleePrepareSuccess() {
        self.infoNotice("准备接听成功")
    }
    
    func endCallCallback(status:FYRtcEngineStatus?) {
        self.infoNotice("挂断回调: \(statusToShortStr(fyStatus: status))")
        self.afterCallEnd()
    }
    
    func afterCallEnd(){
        self.isCalling = false
        self.updateCallButtonImage()
        
        self.selfPrepareReceiveCall()
    }
    
    func statusToFullStr(fyStatus:FYRtcEngineStatus?) -> String {
        var statusStr = ""
        if let status = fyStatus {
            statusStr = "reportRtcStats: 累计发送字节数:\(status.sendBytes),累计接收字节数:\(status.recvBytes),下行带宽:\(status.downBandwidth),上行带宽:\(status.upBandwidth),发送丢包率:\(status.sendLossRate),接受丢包率:\(status.recvLossRate)"
        }
        return statusStr
    }

    func statusToShortStr(fyStatus:FYRtcEngineStatus?) -> String {
        var statusStr = ""
        if let status = fyStatus {
            statusStr = "已发:\(status.sendBytes),已收:\(status.recvBytes),下行:\(status.downBandwidth),上行:\(status.upBandwidth),发丢率:\(status.sendLossRate),接丢率:\(status.recvLossRate)"
        }
        return statusStr
    }

    /****************************************************************
     * 点到点语音相关Delegate 方法(FYRtcEngineKitDelegate)
     ****************************************************************/
    
    func onFYRtcEngine(_ engine: FYRtcEngineKit!, callEnd status: FYRtcEngineStatus!) {
        self.infoNotice("通话结束: \(statusToShortStr(fyStatus: status))")
        
        self.afterCallEnd()
    }

    func onFYRtcEngine(_ engine: FYRtcEngineKit!, didAudioMuted muted: Bool, uid: String!) {
        self.infoNotice("静音回调: muted=\(muted),uid=\(uid)")
    }

    func onFYRtcEngine(_ engine: FYRtcEngineKit!, incomingCall caller: String!) {
        self.infoNotice("电话☎️打入：\(caller)，去调用接听")
        fyRtcEngine.answerCall()

        self.isCalling = true
        self.updateCallButtonImage()
    }

    func onFYRtcEngine(_ engine: FYRtcEngineKit!, reportRtcStats status: FYRtcEngineStatus!) {
        let logStr = statusToShortStr(fyStatus: status)
        print(logStr)
//        self.infoNotice(logStr)
        self.noticeTop(logStr)
    }
    
    //- (void)onFYRtcEngine:(FYRtcEngineKit *)engine OutgoingCall:(NSString *)callee Uid:(NSString *)uid
    func onFYRtcEngine(_ engine: FYRtcEngineKit!, outgoingCall callee: String!, uid: String!) {
        let logStr = "打出电话: 被叫=\(callee!),主叫=\(uid!)"
        print(logStr)
        self.infoNotice(logStr)
    }
    
    //    接通中回调,被叫已经开始接听点到点语音
    //    - (void)onFYRtcEngineCallConnect:(FYRtcEngineKit *)engine
    func onFYRtcEngineCallConnect(_ engine: FYRtcEngineKit!) {
        self.infoNotice("接通中回调")
    }

    func onFYRtcEngineDialBackSuccess(_ engine: FYRtcEngineKit!) {
        self.infoNotice("回拨成功")
    }
    
    //- (void)onFYRtcEngine:(FYRtcEngineKit *)engine OnError:(FYError *)error
    func onFYRtcEngine(_ engine: FYRtcEngineKit!, onError error: FYError!) {
        let logStr = "出错: \(error.code)=\(error.msg!)"
        print(logStr)
        //self.infoNotice(logStr)
        self.errorNotice(logStr)
    }

}

