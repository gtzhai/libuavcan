# this is required
SET(CMAKE_SYSTEM_NAME Linux)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /opt/imx6ull/usr/bin/arm-linux-gcc)
SET(CMAKE_CXX_COMPILER /opt/imx6ull/usr/bin/arm-linux-g++)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -fPIC -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall -fPIC -std=c++11")
set(CMAKE_C_FLAGS_DEBUG  "-O0 -Wall -g -fPIC -std=c++11")
set(CMAKE_C_FLAGS_RELEASE "-O3 -Wall -fPIC -std=c++11")

# where is the target environment 
set(CMAKE_FIND_ROOT_PATH  "/opt/imx6ull/usr/arm-phoenix-linux-gnueabihf/sysroot/")

# search for programs in the build host directories (not necessary)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# configure Boost and Qt
#SET(QT_QMAKE_EXECUTABLE /opt/qt-embedded/qmake)
#SET(BOOST_ROOT /opt/boost_arm)

