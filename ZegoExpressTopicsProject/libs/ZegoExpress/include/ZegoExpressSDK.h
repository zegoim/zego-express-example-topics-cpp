//
//  Version: 1.5.5.153_stable
//
//  Copyright © 2019 Zego. All rights reserved.
//

#ifndef __ZEGOEXPRESSSDK_H__
#define __ZEGOEXPRESSSDK_H__

#include "internal/ZegoInternalImpl.hpp"

namespace ZEGO {
    namespace EXPRESS {

        class ZegoExpressSDK
        {
        private:
            ZegoExpressSDK() = default;
            ~ZegoExpressSDK() = default;
        public:

            /**
             * 创建引擎单例对象
             *
             * 在调用其他 API 前需要先创建并初始化引擎；SDK 只支持创建一个 ZegoExpressEngine 实例，多次调用此接口返回的都是同一个对象。
             * @param appID ZEGO 为开发者签发的应用 ID，请从 ZEGO 管理控制台 https://console-express.zego.im/ 申请，取值范围为 0~4294967295。
             * @param appSign 每个 AppID 对应的应用签名，请从 ZEGO 管理控制台申请。为 64 字符的字符串。每个字符范围为'0'~'9', 'a'~'z'。
             * @param isTestEnv 选择使用测试环境还是正式商用环境，正式环境需要在 ZEGO 管理控制台提交工单配置。测试环境为测试开发用，限制房间数为 30，用户数为 230。正式环境 App 正式上线用，ZEGO 会根据开发者在管理控制台提交的配置记录提供相应的服务资源。测试环境与正式环境是两套环境，不可互通。
             * @param scenario 所属的应用场景，开发者可根据所开发的 App 的场景选择其中一个，引擎会通过所设置的场景预设置一个针对具体场景的较为通用的设置。当设置具体场景后，开发者若有定制化参数设置，依然可以调用具体接口来设置具体参数。
             * @param eventHandler 事件通知回调。传 [nullptr] 则意味着不接收任何回调通知。之后也可通过 [setEventHandler] 进行设置
             * @return 引擎单例对象
             */
            static IZegoExpressEngine* createEngine(unsigned int appID, const std::string& appSign, bool isTestEnv, ZegoScenario scenario, std::shared_ptr<IZegoEventHandler> eventHandler) {
                return ZegoExpressSDKInternal::createEngine(appID, appSign, isTestEnv, scenario, eventHandler);
            }

            /**
             * 异步销毁引擎单例对象
             *
             * 用于释放 SDK 使用的资源。
             * @param engine 先前调用 createEngine 方法创建的引擎实例
             * @param callback 销毁引擎完成的通知回调，可监听此回调以确保设备硬件资源被释放完成。本回调只作为引擎内部资源释放完成的通知，开发者不可以在此回调内释放与引擎相关的资源。若开发者只使用 SDK 来实现音视频功能，该参数可传 [nullptr]。
             */
            static void destroyEngine(IZegoExpressEngine*& engine, ZegoDestroyCompletionCallback callback = nullptr) {
                ZegoExpressSDKInternal::destroyEngine(engine, callback);
            }

            /**
             * 获取引擎单例对象
             *
             * 如果引擎尚未创建或已经销毁，则返回 [nullptr]
             * @return 引擎单例对象
             */
            static IZegoExpressEngine* getEngine() {
                return ZegoExpressSDKInternal::getEngine();
            }

            /**
             * 设置引擎进阶配置，仅在创建引擎之前调用才生效
             *
             * 开发者当需要引擎的高级功能时需要调用此接口来设置高级功能配置。
             * @param config 引擎进阶配置
             */
            static void setEngineConfig(ZegoEngineConfig config) {
                ZegoExpressSDKInternal::setEngineConfig(config);
            }

            /**
             * 获取 SDK 版本号
             *
             * SDK 在运行过程中，当开发者发现与预期情况不符时，可将问题与相关日志提交给 ZEGO 技术人员定位，ZEGO 技术人员可能需要 engine 的版本的信息来辅助定位问题。
             * 开发者也可以收集此信息作为 App 所使用的 engine 的版本信息，以便统计线上各版本 App 对应的各版本 SDK。
             * @return SDK 版本号
             */
            static std::string getVersion() {
                return ZegoExpressSDKInternal::getVersion();
            }

        };

    } //namespace EXPRESS
} //namespace ZEGO

#endif //__ZEGOEXPRESSSDK_H__
