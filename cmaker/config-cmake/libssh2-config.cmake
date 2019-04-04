# curl/CMake/libssh2-config.cmake
#-- Could NOT find LibSSH2 (missing: LIBSSH2_INCLUDE_DIR LIBSSH2_LIBRARY)
# Edit CMakeLists.txt:
#	find_package(LIBSSH2 REQUIRED CONFIG)
# cmake -DCMAKE_USE_LIBSSH2=ON .. -A x64

set(LIBSSH2_INCLUDE_DIR /code/libssh2/include)
set(LIBSSH2_LIBRARY /code/libssh2/build/Debug/zlibstaticd.lib)
set(LIBSSH2_INCLUDE_DIRS ${LIBSSH2_INCLUDE_DIR})
set(LIBSSH2_LIBRARIES ${LIBSSH2_LIBRARY})


