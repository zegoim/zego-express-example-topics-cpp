//
//  Version: 1.3.4.73_release
//
//  Copyright © 2019 Zego. All rights reserved.
//

#ifndef __ZEGOEXPRESSSDK_H__
#define __ZEGOEXPRESSSDK_H__

#include "ZegoImplementation.h"

namespace ZEGO {
    namespace EXPRESS {

    class ZegoExpressSDK
    {
    public:
    
    /**
     * 创建引擎实例对象
     * 在调用其他 API 前需要先创建并初始化引擎；SDK 只支持创建一个 ZegoExpressEngine 实例，多次调用此接口返回的都是同一个对象。创建失败则返回nullptr。
     * @param appID ZEGO 为开发者签发的应用 ID，请从 ZEGO 管理控制台申请
     * @param appSign 每个 AppID 对应的应用签名，请从 ZEGO 管理控制台申请
     * @param isTestEnv 选择使用测试环境还是正式商用环境，正式环境需要在 ZEGO 管理控制台提交工单配置
     * @param scenario 所属的应用场景，当前请选择为 ZegoScenarioGeneral
     * @param eventHandler 事件通知回调。传空则意味着不接收任何回调通知。之后也可通过 addEventHandler/RemoveHandler 进行管理
     * @return (IZegoExpressEngine*), 引擎实例对象
     */
    static IZegoExpressEngine* createEngine(unsigned int appID, std::string appSign, bool isTestEnv, ZegoScenario scenario, std::shared_ptr<IZegoEventHandler> eventHandler) {
        return ZegoExpressEngineInternal::createEngine(appID, appSign, isTestEnv, scenario, eventHandler);
    }

    /**
     * 销毁引擎实例对象
     * 用于释放 IZegoExpressEngine 使用的资源。
     * @param engine 先前调用createEngine方法创建的引擎实例
     */
    static void destroyEngine(IZegoExpressEngine*& engine) {
        ZegoExpressEngineInternal::destroyEngine(engine);
    }

    /**
     * 获取SDK版本号
     * 
     * @return (std::string), SDK 版本号
     */
    static std::string getVersion() {
        return ZegoExpressEngineInternal::getVersion();
    }


    private:
        ZegoExpressSDK() = default;
        ~ZegoExpressSDK() = default;
    };

} //namespace EXPRESS
} //namespace ZEGO

#endif //__ZEGOEXPRESSSDK_H__
    