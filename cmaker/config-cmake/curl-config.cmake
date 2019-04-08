# curl-config.cmake
# libssh2/cmake/curl-config.cmake
# Edit curl/CMakeLists.txt:
#   find_package(curl REQUIRED CONFIG)
# cmake -DENABLE_curl_COMPRESSION=ON -DBUILD_SHARED_LIBS=OFF -DCMAKE_USE_WINSSL=ON .. -A x64

set(curl_INCLUDE_DIR /code/curl/include)
set(curl_LIBRARY /code/curl/build/lib/Debug/libcurl-d.lib)
set(curl_INCLUDE_DIRS ${curl_INCLUDE_DIR})
set(curl_LIBRARIES ${curl_LIBRARY})

set(CURL_LIBS 
	wldap32
	crypt32.lib
	Ws2_32.lib
)
