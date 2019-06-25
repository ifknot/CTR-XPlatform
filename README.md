# CTR Mode AES Block Chain Cipher
![rptx logo](https://cldup.com/W9fXBWKrXq.png)
### C++17 set up RPi(arm7)* to use CLang 8 and CMAKE files
*not RPi Zero(arm6) - see [Pi Zero GCC 9 and CMAKE](https://github.com/ifknot/ctr/wiki/RPi-Zero(w)-C--17-set-up-to-use-GCC-9-and-CMAKE-files)
#### 1. Setup CLang 8
Download the official binary of CLang 8 for Raspberry Pi
```
cd ~
wget http://releases.llvm.org/8.0.0/clang+llvm-8.0.0-armv7a-linux-gnueabihf.tar.xz
#Raspbery Pi Zero is ARMv6 so will *not* work
#Zero uses the originalthanks to: 
111
+ [solarian programmer](https://solarianprogrammer.com/2017/12/08/raspberry-pi-raspbian-install-gcc-compile-cpp-17-programs/)
112
+  BCM2835 SoC used on the first generation of Pi's and all the other single core models
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

  Selection    Path            Priority   Status
------------------------------------------------------------
* 0            /usr/bin/gcc     20        auto mode
  1            /usr/bin/clang   10        manual mode
  2            /usr/bin/gcc     20        manual mode
Press enter to keep the current choice[*], or type selection number:
```
> Removal

```
sudo rm -rf /usr/local/clang_8.0.0
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
project(ctr)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wthanks to: 
111
+ [solarian programmer](https://solarianprogrammer.com/2017/12/08/raspberry-pi-raspbian-install-gcc-compile-cpp-17-programs/)
112
+ all")
set(CMAKE_CXX_FLAGS_DEBUG "--debug -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-Os") #optimize for memory footprint

add_executable(ctr
        main.cpp)
```

Using CMAKE inside project directory containing CMakeLists.txt
```
mkdir build
cd build
cmake ..
make
```

thanks to: 
+ [solarian programmer](https://solarianprogrammer.com/2017/12/08/raspberry-pi-raspbian-install-gcc-compile-cpp-17-programs/)
+ [nick cullen](https://nickcullen.net/blog/raspberry-pi-tutorials/raspberry-pi-c-using-cmake/)
