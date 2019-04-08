# zlib/cmake/zlib-config.cmake
# Edit zlib/CMakeLists.txt:
#   find_package(ZLIB REQUIRED CONFIG)
# cmake -DENABLE_ZLIB_COMPRESSION=ON -DZLIB_DIR=../cmake .. -A x64

set(ZLIB_INCLUDE_DIR /code/zlib)
set(ZLIB_LIBRARY /code/zlib/build/Debug/zlibstaticd.lib)
set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR}/build)
set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
