# ZegoExpressSDK Qt Demo

ZegoExpressSDK 采用标准 C++ 编写，支持跨平台开发。示例 Demo 的 UI 亦采用了跨平台框架 Qt 开发，来为 Windows、Mac 等不同平台的客户提供示例教程。不同平台的开发者可能采用不同的UI框架，但所有 UI 背后的内部逻辑却往往大同小异。在实际开发中，用户可参照 Demo 来调用 SDK 以实现不同的音视频功能逻辑。在界面绘制上，将 Demo 所用的 Qt 组件替换为自己采用的 UI 组件即可。

## 1 准备环境

请确保开发环境满足以下技术要求：

* 操作系统 Windows7 或以上版本
* 麦克风、摄像头等支持音视频功能的外部设备, 确保电脑网络连接正常
* 安装 Visual Studio 2015 或以上版本
* 安装 Qt5.9 及以上版本 [下载地址](http://download.qt.io/official_releases/qt/5.9/5.9.9/)

> 运行 Qt 安装程序，在选择组件页面里，请注意勾选您本机安装的 VS 对应版本的组件

## 2 运行示例代码

### 2.1 填写 AppID 和 AppSign

示例代码中缺少 SDK 创建引擎必须的 AppID 和 AppSign，请参考 [获取 AppID 和 AppSign 指引 \|_blank](https://doc.zego.im/API/HideDoc/GetExpressAppIDGuide/GetAppIDGuideline.html) 获取，并将 AppID 和 AppSign 填写进 `ZegoExpressTopicsProject/src/AppSupport/ZegoConfigManager.cpp` 文件。

```c++
unsigned int ZegoConfigManager::getAppID()
{
    return ;    // input AppID here
}

std::string ZegoConfigManager::getAppSign()
{
    return "";  // input AppSign here
}
```

### 2.2 编译运行

#### 2.2.1 使用 Qt Creator 打开项目

使用 Qt Creator 打开 `ZegoExpressTopicsProject/src/ZegoExpressTopics.pro` 工程文件，点击构建、运行即可。

#### 2.2.2 生成 sln 工程文件 并使用 Visual Studio 打开

Demo 支持跨平台开发，没有自带平台特有的 .sln 或 .xcodeproj 工程文件。可利用 cmake 生成对应工程。Windows 系统生成 sln 工程的具体步骤如下：

1. 安装 cmake ： [下载地址](https://cmake.org/download/)
2. 设置 Path 环境变量：将 cmake 安装目录 `C:\Program Files\CMake\bin` 添加到 Path 环境变量中
3. 设置 Path 环境变量：将 Qt 安装目录 `C:\Qt\Qt5.9.8\5.9.8\msvc2015\bin` 添加到 Path 环境变量中
4. 设置 `QT_DIR` 环境变量：设置 `QT_DIR` 环境变量为 `C:\Qt\Qt5.9.8\5.9.8\msvc2015` 目录
5. 双击 `build-win/generate_vs2015_x86.bat`，将在 cmake_temp_x86 目录下生成的 sln工程

> 用户需要参照上面给出的 cmake 和 Qt 路径，找到并修改为用户本机实际的路径。
>
> 若生成的 cmake_temp 目录下没有产生sln工程文件，则可以查看 `generate_vs2015_x86.bat` 运行时的报错，以进一步解决。
