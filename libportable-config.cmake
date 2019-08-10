# portable-config.cmake
# Created by Robin Rowe 2019-07-19
# find_package(PORTABLE REQUIRED)
#   NAMES portable uuid lmdb regex sqlite xxhash
#   PATHS ${PORTABLE_PATH} NO_DEFAULT_PATH
# include_directories(${PORTABLE_PATH})
# if(WIN32)
# include_directories(${PORTABLE_INCLUDE_DIRS})
#   link_directories(${PORTABLE_LIB_DIRS})
#   link_libraries(${PORTABLE_LIBRARIES})
# endif(WIN32)

message("Found libportable: ${CMAKE_CURRENT_LIST_DIR}")

if(NOT WIN32)
	set_target_properties(libportable PROPERTIES SUFFIX ".a")
endif(NOT WIN32)

if (NOT DEFINED CMAKE_STATIC_LIBRARY_SUFFIX)
    if (WIN32)
        set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
    else()
        set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
        set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
    endif()
endif()

set(LIBPORTABLE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})

if(${LIBUNISTD_DOWNLOAD})
	set(LIBUNISTD_LIB_DIRS 
		debug ${LIBUNISTD_LIB_DIRS}/unistd/Debug
		optimized ${LIBUNISTD_LIB_DIRS}/unistd/Release ) 
else(${LIBUNISTD_DOWNLOAD})
	set(LIBUNISTD_LIB_DIRS 
		debug ${LIBUNISTD_LIB_DIRS}/build/win64/unistd/Debug
		optimized ${LIBUNISTD_LIB_DIRS}/build/win64/unistd/Release ) 
endif(${LIBUNISTD_DOWNLOAD})

set(LIBPORTABLE_LIBRARIES libportable)
set(LIBPORTABLE_LIB_DIRS ${CMAKE_CURRENT_LIST_DIR})

set(LIBUUID_LIBRARIES libuuid.${LIB_EXT})
set(LIBUUID_LIB_DIRS ${CMAKE_CURRENT_LIST_DIR})

set(LIBLMDB_LIBRARIES liblmdb.${LIB_EXT})
set(LIBLMDB_LIB_DIRS ${CMAKE_CURRENT_LIST_DIR})

set(LIBSQLITE_LIBRARIES libsqlite.${LIB_EXT})
set(LIBSQLITE_LIB_DIRS ${CMAKE_CURRENT_LIST_DIR})

set(LIBXXHASH_LIBRARIES libxxhash.${LIB_EXT})
set(LIBXXHASH_LIB_DIRS ${CMAKE_CURRENT_LIST_DIR})

message("  LIBPORTABLE_INCLUDE_DIRS = ${LIBPORTABLE_INCLUDE_DIRS}")
message("  LIBPORTABLE_LIBRARIES = ${LIBPORTABLE_LIBRARIES}")
message("  LIBPORTABLE_LIB_DIRS = ${LIBPORTABLE_LIB_DIRS}")