if exist %cd%\cmake_temp_x86 (
    rd /q/s cmake_temp_x86
)

md cmake_temp_x86
cd cmake_temp_x86

cmake -G"Visual Studio 12 2013"  -DWINDOWS_PLATFORM=x86 ../..

