# Libunistd/CMakeList.txt

if(WIN32)
add_subdirectory(vcpp)
endif()

add_subdirectory(portable)
add_subdirectory(tabstop)

