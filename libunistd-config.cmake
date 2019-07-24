# libunistd/libunistd-config.cmake
#-- Could NOT find LIBUNISTD (missing: LIBUNISTD_INCLUDE_DIR LIBUNISTD_LIBRARY)
# /code/github/libunistd/build/win64/unistd/Debug/libunistd.lib
# Edit CMakeLists.txt:
#
#find_package(LIBUNISTD REQUIRED
#    NAMES libunistd
#	PATHS ${LIBUNISTD_PATH} NO_DEFAULT_PATH
#)

set(LIBUNISTD_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/unistd)
set(LIBUNISTD_LIBRARIES libunistd.lib)
set(LIBUNISTD_LIB_DIRS ${CMAKE_CURRENT_LIST_DIR})
message("Use LIBUNISTD_LIB_DIRS = ${CMAKE_CURRENT_LIST_DIR}")

	
