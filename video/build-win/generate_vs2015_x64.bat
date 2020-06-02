if exist %cd%\cmake_temp_x64 (
    rd /q/s cmake_temp_x64
)

md cmake_temp_x64
cd cmake_temp_x64

cmake -G"Visual Studio 14 2015 Win64"  -DWINDOWS_PLATFORM=x64 ../..


