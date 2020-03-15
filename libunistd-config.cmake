# libunistd/libunistd-config.cmake
#-- Could NOT find LIBUNISTD (missing: LIBUNISTD_INCLUDE_DIR LIBUNISTD_LIBRARY)
# /code/github/libunistd/build/x64/unistd/Debug/libunistd.lib
# Edit CMakeLists.txt:
#
# find_package(LIBUNISTD REQUIRED
#   NAMES libunistd
#	PATHS ${LIBUNISTD_PATH} NO_DEFAULT_PATH
# )

message("<<< Parsing libunistd-config.cmake")
message("Found libunistd:  CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")

#set(CMAKE_STATIC_LIBRARY_PREFIX "") #Avoid Linux "liblib__.a"
#message("--- Setting output directories ---")
if(${LIBUNISTD_DOWNLOAD})
#	message("Downloading libunistd...")
	set(OUTPUTDIR ${CMAKE_CURRENT_SOURCE_DIR})
else(${LIBUNISTD_DOWNLOAD})
	set(OUTPUTDIR ${CMAKE_CURRENT_BINARY_DIR})
endif(${LIBUNISTD_DOWNLOAD})

# CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG:
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUTDIR} )
#message(" CMAKE_LIBRARY_OUTPUT_DIRECTORY = ${OUTPUTDIR}")
foreach( outputconfig ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${outputconfig} OUTPUTCONFIG )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUTDIR}/${outputconfig} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUTDIR}/${outputconfig} )
#	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "VALUE" CACHE STRING ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG}})
#	message(" _${OUTPUTCONFIG} = ${OUTPUTDIR}/${outputconfig}" )
endforeach( outputconfig ${CMAKE_CONFIGURATION_TYPES} )
# CMAKE_LIBRARY_OUTPUT_DIRECTORY = C:/Code/gitlab/ccLater/build/x64
# _DEBUG = C:/Code/gitlab/ccLater/build/x64/Debug
# _RELEASE = C:/Code/gitlab/ccLater/build/x64/Release
# _MINSIZEREL = C:/Code/gitlab/ccLater/build/x64/MinSizeRel
# _RELWITHDEBINFO = C:/Code/gitlab/ccLater/build/x64/RelWithDebInfo

if(WIN32)
	set(CMAKE_STATIC_LIBRARY_PREFIX "")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
else(WIN32)
	set(CMAKE_STATIC_LIBRARY_PREFIX "lib")    
	set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
endif(WIN32)

#message("CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG = ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG}")

if(${LIBUNISTD_DOWNLOAD})
	set(LIBUNISTD_LIB_DIRS ${CMAKE_CURRENT_LIST_DIR})
# Adjusting LIBUNISTD_DOWNLOAD path 
#   C:/Code/gitlab/ccLater/build/x64/libunistd/libunistd-src...
#   /c/code/gitlab/cclater/build/x64/build/x64/libunistd/libunistd-src/unistd/Debug/libunistd.lib
	message(" Using LIBUNISTD_DOWNLOAD path ${LIBUNISTD_LIB_DIRS}")
	set(LIBUNISTD_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/unistd)
	set(LIBUNISTD_LIB_DIRS 
		debug ${LIBUNISTD_LIB_DIRS}/build/x64/unistd/Debug
		optimized ${LIBUNISTD_LIB_DIRS}/build/x64/unistd/Release ) 
else(${LIBUNISTD_DOWNLOAD})
	set(LIBUNISTD_LIB_DIRS ${CMAKE_CURRENT_LIST_DIR})
	message(" Using local libunistd path ${LIBUNISTD_LIB_DIRS}")
	set(LIBUNISTD_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/unistd)
	set(LIBUNISTD_LIB_DIRS 
		debug ${LIBUNISTD_LIB_DIRS}/build/x64/Debug
		optimized ${LIBUNISTD_LIB_DIRS}/build/x64/Release ) 
endif(${LIBUNISTD_DOWNLOAD})

if(WIN32)
	set(LIBUNISTD_LIBRARIES libunistd) #${CMAKE_STATIC_LIBRARY_SUFFIX})
	message("--- Windows libunistd paths ---")
	message(" LIBUNISTD_INCLUDE_DIRS = ${LIBUNISTD_INCLUDE_DIRS}")
	message(" LIBUNISTD_LIBRARIES = ${LIBUNISTD_LIBRARIES}")
	message(" LIBUNISTD_LIB_DIRS = ${LIBUNISTD_LIB_DIRS}")
endif(WIN32)

set(LIBPORTABLE_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
set(LIBPORTABLE_LIBRARIES libuuid liblmdb libregex libsqlite libxxhash libportable )
set(LIBPORTABLE_LIB_DIRS ${LIBUNISTD_LIB_DIRS})

message("--- libportable paths ---")
message(" LIBPORTABLE_INCLUDE_DIRS = ${LIBPORTABLE_INCLUDE_DIRS}")
message(" LIBPORTABLE_LIBRARIES = ${LIBPORTABLE_LIBRARIES}")
message(" LIBPORTABLE_LIB_DIRS = ${LIBPORTABLE_LIB_DIRS}")