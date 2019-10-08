#!/bin/sh

set -x

if [ -d build ];then
        rm build -rf
fi
mkdir build

cd build && cmake -DCMAKE_TOOLCHAIN_FILE=../imx6ull.cmake -DCMAKE_INSTALL_PREFIX=/opt/imx6ull/usr/arm-phoenix-linux-gnueabihf/sysroot/usr .. && make && make install && cd ..

