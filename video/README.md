# ZegoExpressVideo Topics

ZegoExpressVideo Demo（CPP), can be run on both Windows and Mac system.

## Prepare the environment

Please ensure that the development environment meets the following technical requirements:

* Win: win7 or above and Visual Studio 2015 or above installed
* Mac: Xcode installed
* Cmake Installed: [download address](https://cmake.org/download/)
* Qt 5.9.0 or above installed: [Download address](http://download.qt.io/official_releases/qt/5.9/5.9.0/), Read [Get Started](https://doc.qt.io/qt-5/gettingstarted.html) if you are new to Qt
* Make sure audio and video Devices available

## Set system environment variable

Refer to the cmake and Qt paths given blow, and set them with the actual path on the your machine.

* Windows:

1. Set system environment variable: add `C:\Program Files\CMake\bin` to `Path`
2. Set system environment variable: add `C:\Qt\Qt5.9.8\5.9.8\msvc2015\bin` to `Path`
3. Set system environment variable: add `C:\Qt\Qt5.9.8\5.9.8\msvc2015_64\bin` to `Path`
4. Add user variable `QT_DIR`:  `C:\Qt\Qt5.9.8\5.9.8\msvc2015`
5. Add user variable `QT_DIR_x64`:  `C:\Qt\Qt5.9.8\5.9.8\msvc2015_x64`

* Mac:

```bash
export PATH="/Users/zego/Qt5.9.8/5.9.8/clang_64/bin:$PATH"
export QTDIR="/Users/zego/Qt5.9.8/5.9.8/clang_64/"
```

## The video directory tree

> The **video** directory tree is showed below, **the paths given later will the relative to this directory** :

```bash
.
├── CMakeLists.txt
├── README.ZH.md
├── README.md
├── build-mac
│   └── generate_mac.sh-------------------------generate xcode project
├── build-win
│   ├── generate_vs2013_x64.bat-----------------generate vs project
│   ├── generate_vs2013_x86.bat
│   ├── generate_vs2015_x64.bat
│   └── generate_vs2015_x86.bat
├── libs
│   └── ZegoExpress
│       ├── mac---------------------------------ZegoExpressVideo Mac SDK should be here
│       │   ├── include
│       │   └── lib
│       └── win---------------------------------ZegoExpressVideo Win SDK should be here
│           ├── include
│           └── lib
│               ├── x64
│               └── x86
└── src
    ├── AppSupport
    │   └── ZegoConfigManager.cpp---------------input AppID 和 AppSign
    ├── CMakeLists.txt
    └── ZegoExpressExample.pro------------------use QtCreator to open project
```

## Download SDK

> **If the SDK is already in the diretory like above, just jump to the next step**

* Download [ZegoExpressVideo-Win](https://storage.zego.im/express/video/windows/en/zego-express-video-windows-en.zip), unzip to `/libs/ZegoExpress/win`
* Download [ZegoExpressVideo-Mac](https://storage.zego.im/express/video/mac-cpp/en/zego-express-video-mac-cpp-en.zip), unzip to `/libs/ZegoExpress/mac`

## Run

### Fill in AppID and AppSign

The AppID and AppSign required by SDK initialization are missing default, please refer to [ZEGO Admin Console User Manual \| _blank](https://doc-en.zego.im/en/1271.html) to obtain AppID and AppSign, and then fill the `/src/AppSupport/ZegoConfigManager.cpp` file.

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

Use Qt Creator to open the `/src/ZegoExpressExample.pro` file.

#### Generate .sln project

1. Double-click `/build-win/generate_vs2015_x86.bat`, the sln project will be generated in the `/build-win/cmake_temp_x86` directory
2. Double-click `/build-win/generate_vs2015_x64.bat`, the sln project will be generated in the `/build-win/cmake_temp_x64` directory

#### Generate .xcodeproj

1. Execute `/build-mac/generate_mac.sh`, the xcode project will be generated in the `/build-mac/cmake_temp` directory
