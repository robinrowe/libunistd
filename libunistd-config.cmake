# libunistd/libunistd-config.cmake
#-- Could NOT find LIBUNISTD (missing: LIBUNISTD_INCLUDE_DIR LIBUNISTD_LIBRARY)
# /code/github/libunistd/build/win64/unistd/Debug/libunistd.lib
# Edit CMakeLists.txt:
#
# find_package(LIBUNISTD REQUIRED
#   NAMES libunistd
#	PATHS ${LIBUNISTD_PATH} NO_DEFAULT_PATH
# )

message("Found libunistd: ${CMAKE_CURRENT_LIST_DIR}")

set(LIBUNISTD_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/unistd)
set(LIBUNISTD_LIBRARIES libunistd.lib)
set(LIBUNISTD_LIB_DIRS ${CMAKE_CURRENT_LIST_DIR})

if (NOT DEFINED CMAKE_STATIC_LIBRARY_SUFFIX)
    if (WIN32)
        set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
    else()
        set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
        set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
    endif()
endif()

if(${LIBUNISTD_DOWNLOAD})
	message("Adjusting LIBUNISTD_DOWNLOAD paths...")
	set(LIBUNISTD_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
	set(LIBUNISTD_LIB_DIRS 
		debug ${LIBUNISTD_LIB_DIRS}/unistd/Debug
		optimized ${LIBUNISTD_LIB_DIRS}/unistd/Release ) 
else(${LIBUNISTD_DOWNLOAD})
	set(LIBUNISTD_LIB_DIRS 
		debug ${LIBUNISTD_LIB_DIRS}/build/win64/unistd/Debug
		optimized ${LIBUNISTD_LIB_DIRS}/build/win64/unistd/Release ) 
endif(${LIBUNISTD_DOWNLOAD})

set(LIBPORTABLE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
set(LIBPORTABLE_LIBRARIES libuuid liblmdb libregex libsqlite libxxhash libportable )

message(" LIBUNISTD_INCLUDE_DIRS = ${LIBUNISTD_INCLUDE_DIRS}")
message(" LIBUNISTD_LIBRARIES = ${LIBUNISTD_LIBRARIES}")
message(" LIBUNISTD_LIB_DIRS = ${LIBUNISTD_LIB_DIRS}")
message(" LIBPORTABLE_INCLUDE_DIRS = ${LIBPORTABLE_INCLUDE_DIRS}")
message(" LIBPORTABLE_LIBRARIES = ${LIBPORTABLE_LIBRARIES}")