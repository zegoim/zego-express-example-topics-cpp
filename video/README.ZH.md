# ZegoExpressVideo Topics

ZegoExpresVideo SDK（CPP） 示例，可运行于 Win/Mac 系统。

## 准备环境

请确保开发环境满足以下技术要求：

* Win系统：win7 及以上版本，安装了Visual Studio 2015 及以上版本
* Mac系统：安装了 XCode
* 安装了 CMake: [下载地址](https://cmake.org/download/)
* 安装了 Qt5.9 及以上版本 [下载地址](http://download.qt.io/official_releases/qt/5.9/5.9.9/), 首次使用 Qt 的开发者可阅读其 [官方教程](https://doc.qt.io/qt-5/gettingstarted.html)
* 麦克风、摄像头等支持音视频功能的外部设备正常

## 添加环境变量

下面给出的 CMake 和 Qt 路径供参考，用户需要填入自己机器上的实际路径。

* Win系统：

1. 修改环境变量：给 Path 中添加 `C:\Program Files\CMake\bin`
2. 修改环境变量：给 Path 中添加 `C:\Qt\Qt5.9.8\5.9.8\msvc2015\bin`
3. 修改环境变量：给 Path 中添加 `C:\Qt\Qt5.9.8\5.9.8\msvc2015_64\bin`
4. 添加用户变量：添加 `QT_DIR` 用户变量为 `C:\Qt\Qt5.9.8\5.9.8\msvc2015`
5. 添加用户变量：添加 `QT_DIR_x64` 用户变量为 `C:\Qt\Qt5.9.8\5.9.8\msvc2015_64`

* Mac系统：

```bash
export PATH="/Users/zego/Qt5.9.8/5.9.8/clang_64/bin:$PATH"
export QTDIR="/Users/zego/Qt5.9.8/5.9.8/clang_64/"
```

## 目录结构说明

> 如下所示是 **video** 目录的 文件结构，本文后面所涉及到的文件路径均相对于此目录：

```bash
.
├── CMakeLists.txt
├── README.ZH.md
├── README.md
├── build-mac
│   └── generate_mac.sh-------------------------用于生成 xcode 工程
├── build-win
│   ├── generate_vs2013_x64.bat-----------------用于生成 vs 工程
│   ├── generate_vs2013_x86.bat
│   ├── generate_vs2015_x64.bat
│   └── generate_vs2015_x86.bat
├── libs
│   └── ZegoExpress
│       ├── mac---------------------------------ZegoExpressVideo Mac SDK 存放目录
│       │   ├── include
│       │   └── lib
│       └── win---------------------------------ZegoExpressVideo Win SDK 存放目录
│           ├── include
│           └── lib
│               ├── x64
│               └── x86
└── src
    ├── AppSupport
    │   └── ZegoConfigManager.cpp---------------填写申请的 AppID 和 AppSign
    ├── CMakeLists.txt
    └── ZegoExpressExample.pro------------------可使用 QtCreator 打开工程
```

## 下载SDK

> **若如上所示的 SDK 存放目录中已经有了对应的 SDK 文件，则请跳过此步**。

* 下载 [ZegoExpressVideo-Win](https://storage.zego.im/express/video/windows/en/zego-express-video-windows-en.zip)，解压后放到 `/libs/ZegoExpress/win` 目录里
* 下载 [ZegoExpressVideo-Mac](https://storage.zego.im/express/video/mac-cpp/en/zego-express-video-mac-cpp-en.zip)，解压后放到 `/libs/ZegoExpress/mac` 目录里

## 运行

### 填写 AppID 和 AppSign

示例代码中缺少 SDK 创建引擎必须的 AppID 和 AppSign，请参考 [获取 AppID 和 AppSign 指引 \|_blank](https://doc.zego.im/API/HideDoc/GetExpressAppIDGuide/GetAppIDGuideline.html) 获取，并将 AppID 和 AppSign 填写进 `/src/AppSupport/ZegoConfigManager.cpp` 文件。

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

示例自带了 Qt 工程文件，支持在 Win 和 Mac 上使用 Qt Creator 打开。对于需要使用 visual studio 或 XCode 的用户，示例提供了 cmakelist.txt 文件，用户可利用 CMake 生成平台特有的 .sln 或 .xcodeproj 工程文件

#### 使用 Qt Creater 打开项目

1. 菜单栏/文件/打开文件或项目，打开 `/src/ZegoExpressExample.pro` 工程文件

#### 生成 .sln 工程文件

1. 双击 `/build-win/generate_vs2015_x86.bat`，将在 `/build-win/cmake_temp_x86` 目录下生成的 32 位sln工程
2. 双击 `/build-win/generate_vs2015_x64.bat`，将在 `/build-win/cmake_temp_x64` 目录下生成的 64 位sln工程

#### 生成 .xcodeproj 工程文件

1. 执行 `/build-mac/generate_mac.sh` 脚本, 将在 `/build-mac/cmake_temp` 目录下生成xcode工程
