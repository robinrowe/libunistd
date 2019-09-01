# libunistd/libunistd-config.cmake
#-- Could NOT find LIBUNISTD (missing: LIBUNISTD_INCLUDE_DIR LIBUNISTD_LIBRARY)
# /code/github/libunistd/build/win64/unistd/Debug/libunistd.lib
# Edit CMakeLists.txt:
#
# find_package(LIBUNISTD REQUIRED
#   NAMES libunistd
#	PATHS ${LIBUNISTD_PATH} NO_DEFAULT_PATH
# )

message("--- Found libunistd! ---")
message("  CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")

#if (NOT DEFINED CMAKE_STATIC_LIBRARY_SUFFIX)
    if (WIN32)
        set(CMAKE_STATIC_LIBRARY_PREFIX "")
        set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
    else()
        set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
        set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
    endif()
#endif()

set(LIBUNISTD_LIBRARIES ${CMAKE_STATIC_LIBRARY_PREFIX}unistd${CMAKE_STATIC_LIBRARY_SUFFIX})

if(${LIBUNISTD_DOWNLOAD})
	set(LIBUNISTD_LIB_DIRS ${CMAKE_CURRENT_LIST_DIR})
	set(LIBUNISTD_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/unistd)
# Adjusting LIBUNISTD_DOWNLOAD path 
#   C:/Code/gitlab/ccLater/build/win64/libunistd/libunistd-src...
#   /c/code/gitlab/cclater/build/win64/build/win64/libunistd/libunistd-src/unistd/Debug/libunistd.lib
	message("Adjusting LIBUNISTD_DOWNLOAD path ${LIBUNISTD_LIB_DIRS}...")
	set(LIBUNISTD_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
	set(LIBUNISTD_LIB_DIRS 
		debug ${LIBUNISTD_LIB_DIRS}/build/win64/unistd/Debug
		optimized ${LIBUNISTD_LIB_DIRS}/build/win64/unistd/Release ) 
else(${LIBUNISTD_DOWNLOAD})
	set(LIBUNISTD_LIB_DIRS ${CMAKE_CURRENT_LIST_DIR})
	set(LIBUNISTD_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
	set(LIBUNISTD_LIB_DIRS 
		debug ${LIBUNISTD_LIB_DIRS}/unistd/Debug
		optimized ${LIBUNISTD_LIB_DIRS}/unistd/Release ) 
endif(${LIBUNISTD_DOWNLOAD})

set(LIBPORTABLE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
set(LIBPORTABLE_LIBRARIES libuuid liblmdb libregex libsqlite libxxhash libportable )

message(" LIBUNISTD_INCLUDE_DIRS = ${LIBUNISTD_INCLUDE_DIRS}")
message(" LIBUNISTD_LIBRARIES = ${LIBUNISTD_LIBRARIES}")
message(" LIBUNISTD_LIB_DIRS = ${LIBUNISTD_LIB_DIRS}")
message(" LIBPORTABLE_INCLUDE_DIRS = ${LIBPORTABLE_INCLUDE_DIRS}")
message(" LIBPORTABLE_LIBRARIES = ${LIBPORTABLE_LIBRARIES}")