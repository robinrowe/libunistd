# libunistd/libportable-config.cmake
#find_package(LIBUNISTD REQUIRED
#    NAMES libuuid liblmdb libportable libregex libsqlite libunistd libxxhash
#	PATHS ${LIBUNISTD_PATH} NO_DEFAULT_PATH
#)

set(LIBUUID_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(LIBUUID_INCLUDE_DIRS ${LIBUUID_INCLUDE_DIR})
if(WIN64)
        set(LIBUUID_LIBRARY
        debug /build/win64/unistd/Debug/libuuid.lib
        optimized /build/win64/unistd/Release/libuuid.lib )
endif(WIN64)
set(LIBUUID_LIBRARIES ${LIBUUID_LIBRARY})

set(LIBLMDB_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(LIBLMDB_INCLUDE_DIRS ${LIBLMDB_INCLUDE_DIR})
if(WIN64)
        set(LIBLMDB_LIBRARY
        debug /build/win64/unistd/Debug/liblmdb.lib
        optimized /build/win64/unistd/Release/liblmdb.lib )
endif(WIN64)
set(LIBLMDB_LIBRARIES ${LIBLMDB_LIBRARY})

set(LIBPORTABLE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(LIBPORTABLE_INCLUDE_DIRS ${LIBPORTABLE_INCLUDE_DIR})
if(WIN64)
        set(LIBPORTABLE_LIBRARY
        debug /build/win64/unistd/Debug/libportable.lib
        optimized /build/win64/unistd/Release/libportable.lib )
endif(WIN64)
set(LIBPORTABLE_LIBRARIES ${LIBPORTABLE_LIBRARY})

set(LIBREGEX_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(LIBREGEX_INCLUDE_DIRS ${LIBREGEX_INCLUDE_DIR})
if(WIN64)
        set(LIBREGEX_LIBRARY
        debug /build/win64/unistd/Debug/libregex.lib
        optimized /build/win64/unistd/Release/libregex.lib )
endif(WIN64)
set(LIBREGEX_LIBRARIES ${LIBREGEX_LIBRARY})

set(LIBSQLITE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(LIBSQLITE_INCLUDE_DIRS ${LIBSQLITE_INCLUDE_DIR})
if(WIN64)
        set(LIBSQLITE_LIBRARY
        debug /build/win64/unistd/Debug/libsqlite.lib
        optimized /build/win64/unistd/Release/libsqlite.lib )
endif(WIN64)
set(LIBSQLITE_LIBRARIES ${LIBSQLITE_LIBRARY})

set(LIBXXHASH_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(LIBXXHASH_INCLUDE_DIRS ${LIBXXHASH_INCLUDE_DIR})
if(WIN64)
        set(LIBXXHASH_LIBRARY
        debug /build/win64/unistd/Debug/libxxhash.lib
        optimized /build/win64/unistd/Release/libxxhash.lib )
endif(WIN64)
set(LIBXXHASH_LIBRARIES ${LIBXXHASH_LIBRARY})





