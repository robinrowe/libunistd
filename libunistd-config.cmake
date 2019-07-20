# libunistd/libunistd-config.cmake
#-- Could NOT find LIBUNISTD (missing: LIBUNISTD_INCLUDE_DIR LIBUNISTD_LIBRARY)
# /code/github/libunistd/build/win64/unistd/Debug/libunistd.lib
# Edit CMakeLists.txt:
#
#find_package(LIBUNISTD REQUIRED
#    NAMES libunistd
#	PATHS ${LIBUNISTD_PATH} NO_DEFAULT_PATH
#)

set(LIBUNISTD_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/unistd)
set(LIBUNISTD_INCLUDE_DIRS ${LIBUNISTD_INCLUDE_DIR})
set(LIBUNISTD_LIBRARY libunistd.lib)
set(LIBUNISTD_LIBRARIES ${LIBUNISTD_LIBRARY})
set(LIBUNISTD_LIB_DIRS 
	debug ${CMAKE_CURRENT_LIST_DIR}/build/win64/unistd/Debug
	optimized ${CMAKE_CURRENT_LIST_DIR}/build/win64/unistd/Release ) 

#add_library(libunistd STATIC IMPORTED GLOBAL)
#set_target_properties(libunistd PROPERTIES IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_LIST_DIR}/build/win64/unistd/Debug)
#set_target_properties(libunistd PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/build/win64/unistd/Release) 

#set(libunistd_INCLUDE_DIRS ${libunistd_DIR}/include)
#set(libunistd_LIBRARY_DIR ${libunistd_DIR}/lib)
#   set(libunistd_LIBRARIES ${libunistd_LIBRARIES} debug ${libunistd_LIBRARY_DIR}/liblibunistd_${component}-vc110-mt-gd-1_50.lib)
 #   set(libunistd_LIBRARIES ${libunistd_LIBRARIES} optimized ${libunistd_LIBRARY_DIR}/liblibunistd_${component}-vc110-mt-1_50.lib)
#endforeach()
#add_definitions( -D_WIN32_WINNT=0x0501 )
	
