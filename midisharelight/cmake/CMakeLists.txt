#######################################
# CMAKE midishare
#######################################
project(midisharelight)
cmake_minimum_required(VERSION 2.6)

if(WIN32)
	set (target midisharelight)
else()
	set (target midisharelight)
endif()

#######################################
# versions management
set (VERSION A)
set (STRVERS "v.1.00")
set (SSTRVERS "1.00")

if(UNIX)
	add_definitions(-Wall -DGCC -fPIC)
endif(UNIX)

if(APPLE)
	set (CMAKE_OSX_ARCHITECTURES "x86_64 i386")
 	add_definitions(-D__Macintosh__ -D__MacOSX__)
endif(APPLE)

if(WIN32)
	add_definitions(-DWINVER=0x0400 -DWIN32)
endif(WIN32)
add_definitions(-DMSLIGHT)


#######################################
# set directories, src and headers.
set (ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/..)
set (SRC_PATH  ${ROOT_PATH}/src)
set (COMMON_PATH  ${ROOT_PATH}/../src/common)
set (HEADERS_PATH ${SRC_PATH})
set (TESTS_PATH ${ROOT_PATH}/test)


#######################################
# add src and headers files
set (SRC ${SRC} "${SRC_PATH}/*.cpp")				# add source files
set (SRC ${SRC} "${COMMON_PATH}/atomic/lffifo.c"  "${COMMON_PATH}/atomic/lflifo.c")

if(WIN32)
	set (SRC ${SRC} "${SRC_PATH}/common/atomic/msAtomicWin32.asm")
endif(WIN32)
file (GLOB LIBSRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})

set (PUBLICHEADERS ${SRC_PATH}/MidiShareMem.h)
set (HEADERS ${SRC_PATH}/*.h ${SRC_PATH}/atomic/*.h)

file (GLOB PUBLICHEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${PUBLICHEADERS}")
file (GLOB LIBH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${HEADERS})

#######################################
# set includes
set(INCL ${HEADERS_PATH} ${COMMON_PATH}/Headers ${COMMON_PATH}/Memory ${COMMON_PATH}/atomic)		# add include folders

include_directories( ${INCL})
set_source_files_properties (${INCL} PROPERTIES HEADER_FILE_ONLY TRUE)

file (GLOB PUBLICHEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${HEADERS_PATH}/MidiShareLight.h" "${HEADERS_PATH}/midifile.h")


#######################################
# set library target
set(LIBCONTENT ${LIBSRC} ${LIBH} ${PUBLICHEADERS})

add_library(${target} STATIC ${LIBCONTENT})
set_target_properties (${target} PROPERTIES PUBLIC_HEADER "${PUBLICHEADERS}")

if(WIN32)
set_target_properties (${target} PROPERTIES LINK_FLAGS /ENTRY:DllEntryPoint)
elseif(APPLE)
set_target_properties (${target} PROPERTIES LINK_FLAGS -flat_namespace)
endif(WIN32)

#target_link_libraries (${target} ${LINK})

#######################################
# set test targets
add_executable( read ${TESTS_PATH}/read.cpp )
target_link_libraries( read ${target})
add_dependencies(read ${target})

add_executable( write ${TESTS_PATH}/write.cpp )
target_link_libraries( write ${target})
add_dependencies(write ${target})


#######################################
# install setup
if (NOT WIN32)
	set(LIBPATH "/usr/local/lib")
install ( TARGETS ${target}
    	ARCHIVE DESTINATION ${LIBPATH}
    	PUBLIC_HEADER DESTINATION /usr/local/include/
)
endif(NOT WIN32)


