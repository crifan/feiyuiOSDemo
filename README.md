# feiyu-cloud-ios-voice-record-demo
飞语云平台iOS点对点通话录音Demo
此Demo用于演示，使用飞语SDK实现，点对点的，A呼叫B，的语音通话的演示。

内部是通过[飞语官网](https://www.feiyucloud.com)的SDK包`FYRtcEngineKit`，发起`UDP`的数据包，实现了`VOIP`的语音通信。

本demo项目是基于官网的[iOS库+Demo](http://cdn.feiyucloud.com/objective-c-3.0.0.zip)




# 代码逻辑
> 核心代码均在`ViewController.swift`

## 点对点 网络对网络 且带录音的基本流程
> 前提：主叫A和被叫B的手机均已正常联网

1.主叫和被叫都调用SDK初始化`sharedEngine`
```swift
fyRtcEngine = FYRtcEngineKit.sharedEngine(withAppId: FY_APPID, appToken: FY_APPTOKEN, delegate: self)
```
2.被叫一方需要先调用`calleePrepare`去让自己准备好被别人呼叫
```swift
fyRtcEngine.calleePrepare(selfUid, prepareSuccess: self.calleePrepareSuccess)
```

3.主叫方调用`dialPeer`
```swift
fyRtcEngine.dialPeer(otherUid, callerUid: selfUid, optionData: dialOption)
```

此时，主叫方自己会收到`outgoingCall`的回调

4.被叫方在`incomingCall`的回调中调用`answerCall`接听电话
```swift
    func onFYRtcEngine(_ engine: FYRtcEngineKit!, incomingCall caller: String!) {
        ...
        fyRtcEngine.answerCall()
        ...
    }
```

5.主叫方和被叫方均可正常通话，同时每隔2秒会收到`reportRtcStats`的语音统计信息

统计信息如下字段：
```swift
 "reportRtcStats: 累计发送字节数:\(status.sendBytes),累计接收字节数:\(status.recvBytes),下行带宽:\(status.downBandwidth),上行带宽:\(status.upBandwidth),发送丢包率:\(status.sendLossRate),接受丢包率:\(status.recvLossRate)"
```

6.想要挂掉，则调用`endCall`结束通话
```swift
fyRtcEngine.endCall(self.endCallCallback)
```

然后会收到回调`callEnd`
```swift
    func onFYRtcEngine(_ engine: FYRtcEngineKit!, callEnd status: FYRtcEngineStatus!) {
        self.infoNotice("通话结束: \(statusToShortStr(fyStatus: status))")
        
        self.afterCallEnd()
    }
```

## 点对点 网络对手机（PSTN） 且带录音的基本流程
> 前提：主叫A的手机已正常联网

其实和`点对点 网络对网络`的流程是一样的，只不过是被叫方**不需要网络**，当前也就不需要`calleePrepare`等动作了。就相当于别人直接拨打你电话而已。

1.主叫方调用SDK初始化`sharedEngine`
```swift
fyRtcEngine = FYRtcEngineKit.sharedEngine(withAppId: FY_APPID, appToken: FY_APPTOKEN, delegate: self)
```

2.主叫方调用`dialPstn`
```swift
fyRtcEngine.dialPstn(pstnCalleePhone, callerUid: selfUid, display: pstnDisplayPhone, optionData: dialPstnOption)
```

主叫会收到`outgoingCall`的回调，表示正在拨号出去。

参数解释：
- pstnCalleePhone: 被叫的手机号，比如+18613812345678
- display: pstnDisplayPhone：被叫在接到电话的界面中显示什么号码

> 提示：
> - 被叫手机号和显示号必须是符号`PSTN E164`标准的正常的+86开头的国内的手机号，否则会报错，比如
>    - `10086`：提示格式不合法
>    - `+8610086`：提示无权限使用此号

3.被叫方就可以正常接收到并接听电话了

几点说明：
- 不过是有点延迟，大概要等个10秒前后，被叫方才收到打入的电话
- 被叫方如果选择接电话，就可以正常通话了
    - 此时，主叫方可以收到`onFYRtcEngineCallConnect`的回调
- 主叫方每隔2秒会收到`reportRtcStats`的语音统计信息

4.想要挂掉，则调用`endCall`结束通话
```swift
fyRtcEngine.endCall(self.endPstnCallCallback)
```

## 代码说明
1. 如果你想要调试你自己的app，则需要去[飞语官网](https://www.feiyucloud.com)注册账号，创建自己的app，获得`APPID`和`APPTOKEN`，替换`ViewController.swift`中的`FY_APPID`和`FY_APPTOKEN`
2. 相关文档：
    - [FeiyuCloud/objc-sdk-demo: Feiyu Cloud Objective-C SDK Demo](https://github.com/FeiyuCloud/objc-sdk-demo)
    - [点到点语音 · DOCS](https://feiyucloud.gitbooks.io/docs/content/test/dian-dao-dian-yu-yin.html#点到点语音相关delegate-方法fyrtcenginekitdelegate)
    - [API · PrivateDocs](https://feiyucloud.gitbooks.io/privatedocs/content/tong-xin-api-objective-c/api.html)


# 使用步骤
## 点对点 网络对网络 且带录音的基本流程

1. 用Xcode编译项目，安装到iPhone真机1，iPhone真机2中

- 真机1 主叫138:
![主叫138](./img/caller_138.jpg)

- 真机2 被叫139:
![被叫139](./img/callee_139.png)

2. 主叫138中，点击拨号按钮
![主叫138开始呼叫被叫139](./img/caller_call_out.jpg)

3. 被叫139中会自动接听
![被叫139正在通话中](./img/calling_callee.png)

同时主叫138也可以通话了：
![主叫138正在通话中](./img/calling_caller.jpg)

### 使用说明
1. demo中的主叫和被叫的号码（138，和139）均可以自己随意改动，只是要确保互相设置的值是匹配即可。

## 点对点 网络对手机（PSTN） 且带录音的基本流程
1. 用Xcode编译项目，安装到iPhone真机中
2. 输入被叫手机号和（在被叫手机上要）显示的号码，点击拨号
    - 被叫号码可以自己随意改动，只是要确保符合`PSTN E164`标准即可，比如`+8613900001111`.
3. 被叫方过会（大概等10多秒左右）即可接到电话，点击接听，双方即可通话
    - 被叫方：被叫此时就接听别人的普通的电话是一样的效果

![被叫方显示号码设置为假的`+8613900001111`](./img/callee_display_8613900001111.png)

4. 任何一方均可挂断电话
