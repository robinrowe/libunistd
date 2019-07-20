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


# --- portable ---
set(LIBPORTABLE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
set(LIBPORTABLE_LIBRARIES libportable.lib)
set(LIBPORTABLE_LIB_DIRS
        debug ${CMAKE_CURRENT_LIST_DIR}/build/win64/portable/Debug
        optimized ${CMAKE_CURRENT_LIST_DIR}/build/win64/portable/Release )
# --- uuid ---
set(LIBUUID_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
set(LIBUUID_LIBRARIES libuuid.lib)
set(LIBUUID_LIB_DIRS
        debug ${CMAKE_CURRENT_LIST_DIR}/build/win64/uuid/Debug
        optimized ${CMAKE_CURRENT_LIST_DIR}/build/win64/uuid/Release )
# --- lmdb ---
set(LIBLMDB_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
set(LIBLMDB_LIBRARIES liblmdb.lib)
set(LIBLMDB_LIB_DIRS
        debug ${CMAKE_CURRENT_LIST_DIR}/build/win64/lmdb/Debug
        optimized ${CMAKE_CURRENT_LIST_DIR}/build/win64/lmdb/Release )
# --- regex ---
set(LIBREGEX_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
set(LIBREGEX_LIBRARIES libregex.lib)
set(LIBREGEX_LIB_DIRS
        debug ${CMAKE_CURRENT_LIST_DIR}/build/win64/regex/Debug
        optimized ${CMAKE_CURRENT_LIST_DIR}/build/win64/regex/Release )
# --- sqlite ---
set(LIBSQLITE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
set(LIBSQLITE_LIBRARIES libsqlite.lib)
set(LIBSQLITE_LIB_DIRS
        debug ${CMAKE_CURRENT_LIST_DIR}/build/win64/sqlite/Debug
        optimized ${CMAKE_CURRENT_LIST_DIR}/build/win64/sqlite/Release )
# --- xxhash ---
set(LIBXXHASH_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
set(LIBXXHASH_LIBRARIES libxxhash.lib)
set(LIBXXHASH_LIB_DIRS
        debug ${CMAKE_CURRENT_LIST_DIR}/build/win64/xxhash/Debug
        optimized ${CMAKE_CURRENT_LIST_DIR}/build/win64/xxhash/Release )
