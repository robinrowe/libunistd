# libunistd/unistd/CMakeLists.txt
# Copyright 2016 Robin.Rowe@cinepaint.org
# License MIT open source

if(_M_X64)
	enable_language(ASM_MASM)
	set(INT128_SRC Int128x64.asm)
else(_M_X64)
	set(INT128_SRC int128/Int128x86.cpp)
endif(_M_X64)

file(STRINGS sources.cmake SOURCES)
if (BUILD_SHARED_LIBS AND NOT BUILD_STATIC_LIBS AND BUILD_UNISTD_SHARED)
	set(lib_type SHARED)
else ()
	set(lib_type STATIC)
endif ()
set(LIBRARY_NAME "libunistd")
add_library(${LIBRARY_NAME}
	${lib_type}
	${SOURCES}
	int128/Int128.h
	${INT128_SRC}
)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIBRARY_NAME} LINK_PUBLIC shlwapi)

#######################################
# Export / installation configuration #
#######################################

include(GNUInstallDirs)

# setup the version LIBRARY_NAME
set_property(TARGET "${LIBRARY_NAME}" PROPERTY VERSION "${${LIBRARY_NAME}_VERSION}")
set_property(TARGET "${LIBRARY_NAME}" PROPERTY SOVERSION "${${LIBRARY_NAME}_VERSION_MAJOR}")

# install rules
list(APPEND installable_libs "${LIBRARY_NAME}")
if (TARGET "${DEPENDANT_LIBRARY}")
	list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif (TARGET "${DEPENDANT_LIBRARY}")
install(TARGETS ${installable_libs}
		EXPORT "${LIBRARY_NAME}Targets"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES int128/Int128.h
		TYPE "INCLUDE")
