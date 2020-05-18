# ZegoExpressSDK Topics - cpp

Zego Express CPP (windows/Mac) Demo

## Prepare the environment

Please ensure that the development environment meets the following technical requirements:

* Win: win7 or above and Visual Studio 2015 or above installed
* Mac: Xcode installed
* Qt 5.9.0 or above installed
* Make sure the network available
* Make sure audio and video Devices available

> you can [Download](http://download.qt.io/official_releases/qt/5.9/5.9.0/) Qt here and Read [Get Started](https://doc.qt.io/qt-5/gettingstarted.html) if you are new to Qt

## Download SDK

The Demo's directory is showed blow. If the SDK for your platform whether win or mac is missing from the directory, please download it first.

```c++
|-------------------------------------------Project Root Path
    | README.md
    | README.ZH.md
    | ZegoExpressTopicsProject
        | libs
            | ZegoExpress
                | win-----------------------win SDK
                    | include
                    | lib
                | mac-----------------------mac SDK
                    | include
                    | lib
```

Download SDK according to your os system: [windows SDK](https://storage.zego.im/express/video/windows/en/zego-express-video-windows-en.zip) | [mac SDK](https://storage.zego.im/express/video/mac-cpp/en/zego-express-video-mac-cpp-en.zip) and unzip to `/ZegoExpressTopicsProject/deps/ZegoExpress` like above.

## Run

### Fill in AppID and AppSign

The AppID and AppSign required by SDK initialization are missing default, please refer to [ZEGO Admin Console User Manual \| _blank](https://doc-en.zego.im/en/1271.html) to obtain AppID and AppSign, and then fill the `ZegoExpressTopicsProject/src/AppSupport/ZegoConfigManager.cpp` file.

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

### Open Project

Using Qt Creator to open .pro file is supported both on win or mac. The platform-specific .sln or .xcodeproj project file is missing, and CMake can be used to generate them.

#### Open project with Qt Creator

Use Qt Creator to open the `ZegoExpressTopicsProject/src/ZegoExpressTopics.pro` file, click build and run.

#### Generate .sln project

The user needs to refer to the cmake and Qt paths given blow, and set them with the actual path on the user's machine.

1. Install cmake: [download address](https://cmake.org/download/)
2. Set system environment variable: add `C:\Program Files\CMake\bin` to `Path`
3. Set system environment variable: add `C:\Qt\Qt5.9.8\5.9.8\msvc2015\bin` to `Path`
4. Set system environment variable: add `C:\Qt\Qt5.9.8\5.9.8\msvc2015_64\bin` to `Path`
5. Add user variable `QT_DIR`:  `C:\Qt\Qt5.9.8\5.9.8\msvc2015`
6. Add user variable `QT_DIR_x64`:  `C:\Qt\Qt5.9.8\5.9.8\msvc2015_x64`
7. Double-click `ZegoExpressTopicsProject\build-win\generate_vs2015_x86.bat`, the sln project will be generated in the `ZegoExpressTopicsProject\build-win\cmake_temp_x86` directory
8. Double-click `ZegoExpressTopicsProject\build-win\generate_vs2015_x64.bat`, the sln project will be generated in the `ZegoExpressTopicsProject\build-win\cmake_temp_x64` directory

#### Generate .xocdeproj

1. Install cmake: [download address](https://cmake.org/download/)
2. Execute `ZegoExpressTopicsProject/build-mac/generate_mac.sh`, the xcode project will be generated in the `ZegoExpressTopicsProject/build-mac/cmake_temp` directory
