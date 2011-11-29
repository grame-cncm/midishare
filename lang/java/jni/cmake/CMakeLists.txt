#######################################
# CMAKE midisharejni
#######################################
project(midisharejni)
cmake_minimum_required(VERSION 2.6)

#######################################
# versions management
set (VERSION 1.16)
set (STRVERS "v.1.16")
set (SSTRVERS "1.16")

set (target JMidi-${VERSION})

FIND_PACKAGE(JNI REQUIRED)

if(APPLE)
	set (CMAKE_OSX_ARCHITECTURES "x86_64 i386")
	set (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
	set (CMAKE_C++_FLAGS -mmacosx-version-min=10.4)
	add_definitions(-D__Macintosh__)
endif(APPLE)

if(WIN32)
 add_definitions(-DWINVER=0x0400 -DWIN32)
 if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 6")
  add_definitions(-DVC6)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005")
  add_definitions(-DVC2005)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005 Win64")
  add_definitions(-DVC2005)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 9 2008")
  add_definitions(-DVC2005)
 endif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 6")
 
 if (${CMAKE_GENERATOR} MATCHES ".*Win64")
  set (WIN64 1)
  set (target JMidi64)
 endif (${CMAKE_GENERATOR} MATCHES ".*Win64")

endif(WIN32)

#######################################
# set directories, src and headers.
set (JNI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set (MIDI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src)

set (SRC "${JNI_PATH}/*.c")					# add source files
set (HEADERS "${JNI_PATH}/*.h")				# add header files
set (INCL ${JNI_INCLUDE_DIRS} ${MIDI_SRC}/include)


#######################################
# set includes
include_directories( ${INCL})

#######################################
# set library target
file (GLOB LIBSRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})
file (GLOB LIBH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${HEADERS})
set (MS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/cmake/)
if (WIN32)
 if (WIN64)
  find_library (MSLIB mshare64 PATHS ${MS_PATH}/Release NO_DEFAULT_PATH)
 else(WIN64)
  find_library (MSLIB mshare32 PATHS ${MS_PATH}/Release NO_DEFAULT_PATH)
 endif(WIN64)

else(WIN32)
 find_library (MSLIB MidiShare PATHS ${MS_PATH}/Release NO_DEFAULT_PATH)
endif(WIN32)

set (LIBCONTENT ${LIBSRC} ${LIBH})
set (LINK ${JNI_LIBRARIES} ${MSLIB})

if(WIN32)
	enable_language(RC)
	set(LIBCONTENT ${LIBCONTENT} ../win32/JMidi.rc)
endif(WIN32)

add_library(${target} SHARED ${LIBCONTENT})
set_target_properties (${target} PROPERTIES VERSION ${VERSION})
target_link_libraries (${target} ${LINK})

#######################################
# install settings
if(!WIN32)
set (LIBDEST /usr/lib)

install ( TARGETS ${target}	LIBRARY DESTINATION ${LIBDEST})
endif(!WIN32)