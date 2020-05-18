# ZegoExpressSDK Topics - cpp

Zego Express CPP (windows/Mac) 示例专题 Demo

## 准备环境

请确保开发环境满足以下技术要求：

* Win系统：win7 及以上版本，安装了Visual Studio 2015 及以上版本
* Mac系统：安装了 XCode
* 安装了 Qt5.9 及以上版本
* 麦克风、摄像头等支持音视频功能的外部设备正常

> Qt 的[下载地址](http://download.qt.io/official_releases/qt/5.9/5.9.9/)，首次使用 Qt 的开发者可阅读其官方教程 [Get Started](https://doc.qt.io/qt-5/gettingstarted.html)

## 下载SDK

如下所示是 demo 的文件目录。如果当前的工程目录中缺少对应平台的 SDK，请先手动下载SDK；如果当前工程目录中已经有了对应的 SDK，则请跳过此步。

```c++
|-------------------------------------------项目根目录
    | README.md
    | README.ZH.md
    | ZegoExpressTopicsProject
        | libs
            | ZegoExpress
                | win-----------------------win版本的sdk
                    | include
                    | lib
                | mac-----------------------mac版本的sdk
                    | include
                    | lib
```

下载对应平台的 SDK: [windows SDK](https://storage.zego.im/express/video/windows/en/zego-express-video-windows-en.zip) | [mac SDK](https://storage.zego.im/express/video/mac-cpp/en/zego-express-video-mac-cpp-en.zip), 并解压后放到 `/ZegoExpressTopicsProject/deps/ZegoExpress` 目录里，如上所示。

## 运行

### 填写 AppID 和 AppSign

示例代码中缺少 SDK 创建引擎必须的 AppID 和 AppSign，请参考 [获取 AppID 和 AppSign 指引 \|_blank](https://doc.zego.im/API/HideDoc/GetExpressAppIDGuide/GetAppIDGuideline.html) 获取，并将 AppID 和 AppSign 填写进 `/ZegoExpressTopicsProject/src/AppSupport/ZegoConfigManager.cpp` 文件。

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

### 打开工程

ZegoExpressSDK 支持跨平台开发，示例自带了 Qt 工程文件，支持在 Win 和 Mac 上使用 Qt Creator 打开。
对于需要使用 visual studio 或 XCode 的用户，示例提供了 cmakelist.txt 文件，用户可利用 CMake 生成平台特有的 .sln 或 .xcodeproj 工程文件

#### 使用 Qt Creater 打开项目

1. 菜单栏/文件/打开文件或项目，选中 `/ZegoExpressTopicsProject/src/ZegoExpressTopics.pro` 工程文件
2. 点击运行

#### 生成 .sln 工程文件

下面给出的 CMake 和 Qt 路径供参考，用户需要填入自己机器上的实际路径。

1. 安装 CMake ： [下载地址](https://cmake.org/download/)
2. 修改环境变量：给 Path 中添加 `C:\Program Files\CMake\bin`
3. 修改环境变量：给 Path 中添加 `C:\Qt\Qt5.9.8\5.9.8\msvc2015\bin`
4. 修改环境变量：给 Path 中添加 `C:\Qt\Qt5.9.8\5.9.8\msvc2015_64\bin`
5. 添加用户变量：添加 `QT_DIR` 用户变量为 `C:\Qt\Qt5.9.8\5.9.8\msvc2015`
6. 添加用户变量：添加 `QT_DIR_x64` 用户变量为 `C:\Qt\Qt5.9.8\5.9.8\msvc2015_64`
7. 双击 `ZegoExpressTopicsProject\build-win\generate_vs2015_x86.bat`，将在 `ZegoExpressTopicsProject\build-win\cmake_temp_x86` 目录下生成的 32 位sln工程
8. 双击 `ZegoExpressTopicsProject\build-win\generate_vs2015_x64.bat`，将在 `ZegoExpressTopicsProject\build-win\cmake_temp_x64` 目录下生成的 64 位sln工程

#### 生成 .xcodeproj 工程文件

1. 安装 CMake: [下载地址](https://cmake.org/download/)
2. 执行 `ZegoExpressTopicsProject/build-mac/generate_mac.sh` 脚本, 将在 `ZegoExpressTopicsProject/build-mac/cmake_temp` 目录下生成xcode工程
