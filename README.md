# CTR-XPlatform
![rptx logo](https://cldup.com/W9fXBWKrXq.png)
### Set up RPi to use CMAKE files and CLang C++17
#### 1. Setup CLang 8
Download the official binary of CLang 8 for Raspberry Pi
```
cd ~
wget http://releases.llvm.org/8.0.0/clang+llvm-8.0.0-armv7a-linux-gnueabihf.tar.xz
#Raspbery Pi Zero ia ARMv6
#Zero uses the original BCM2835 SoC used on the first generation of Pi's and all the other single core models
```
Extract the archive and move the extracted compilers to /usr/local
```
tar -xf clang+llvm-8.0.0-armv7a-linux-gnueabihf.tar.xz
rm clang+llvm-8.0.0-armv7a-linux-gnueabihf.tar.xz
mv clang+llvm-8.0.0-armv7a-linux-gnueabihf clang_8.0.0
sudo mv clang_8.0.0 /usr/local
```
Add CLang to system search paths
```
echo 'export PATH=/usr/local/clang_8.0.0/bin:$PATH' >> .bashrc
echo 'export LD_LIBRARY_PATH=/usr/local/clang_8.0.0/lib:$LD_LIBRARY_PATH' >> .bashrc
source .bashrc
clang++ --version
```
Add CLang as a system option install as a new alternative: 
```
sudo update-alternatives --install /usr/bin/cc cc /usr/local/clang_8.0.0/bin/clang 10
sudo update-alternatives --install /usr/bin/c++ c++ /usr/local/clang_8.0.0/bin/clang++ 10
```
Configure CLang for C and C++
```
sudo update-alternatives --config cc
sudo update-alternatives --config c++
```
Select CLang system wide
```
  Selection    Path            Priority   Status
------------------------------------------------------------
* 0            /usr/bin/gcc     20        auto mode
  1            /usr/bin/clang   10        manual mode
  2            /usr/bin/gcc     20        manual mode
Press enter to keep the current choice[*], or type selection number:
```


#### 2. Setup CMAKE v3.14
Download CMAKE v 3.14 and extract it
``` 
wget https://cmake.org/files/v3.14/cmake-3.14.5.tar.gz
tar -xvzf cmake-3.14.5.tar.gz
```
Compile and install cmake

```
cd cmake-3.14.5/
sudo ./bootstrap
sudo make
sudo make install
```
Remove old CMAKE data 
```
sudo apt-get remove cmake cmake-data
```
Direct OS to new CMAKE, reload bashrc and check version
```
export CMAKE_ROOT=/usr/share/cmake-3.14
. ~/.bashrc 
cmake --version

cmake version 3.14.5

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```
#### 3. Using CMAKE
Example CMakeList.txt optimize for memory footprint
```cmake
cmake_minimum_required(VERSION 3.14)
project(CTR_XPlatform)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "--debug -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-Os") #optimize for memory footprint

add_executable(CTR_XPlatform
        main.cpp)
```

Using CMAKE inside project directory containing CMakeLists.txt
```
mkdir build
cd build
cmake ..
make
```
