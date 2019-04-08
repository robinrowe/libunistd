# curl-config.cmake
# libssh2/cmake/curl-config.cmake
# Edit curl/CMakeLists.txt:
#   find_package(curl REQUIRED CONFIG)
# cmake -DENABLE_curl_COMPRESSION=ON -Dcurl_DIR=../cmake .. -A x64

set(curl_INCLUDE_DIR /code/curl/include)
set(curl_LIBRARY /code/curl/build/lib/Debug/libcurl-d_imp.lib)
set(curl_INCLUDE_DIRS ${curl_INCLUDE_DIR})
set(curl_LIBRARIES ${curl_LIBRARY})
