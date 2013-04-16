#######################################
# CMAKE midisharejni
#######################################
project(midisharejni)
cmake_minimum_required(VERSION 2.6)

#######################################
# versions management
set (VERSION 1.17)
set (STRVERS "v.1.17")
set (SSTRVERS "1.17")

set (target JMidi-${VERSION})

FIND_PACKAGE(JNI REQUIRED)
if(APPLE)
	if (${CMAKE_SYSTEM_VERSION} VERSION_LESS 9.0.0)
		set (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.4u.sdk)
	elseif (${CMAKE_SYSTEM_VERSION} VERSION_LESS 10.8.0)
		set (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
	elseif (${XCODE_VERSION} VERSION_LESS 4.0.0)
		set (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.6.sdk)
	else ()
		set (CMAKE_OSX_SYSROOT "macosx10.7")
	endif ()
	set (CMAKE_OSX_ARCHITECTURES "x86_64 i386")
	set (CMAKE_C++_FLAGS -mmacosx-version-min=10.4)
	add_definitions(-D__Macintosh__)
endif()

if(WIN32)
 set (libgen vs2005)
 add_definitions(-DWINVER=0x0400 -DWIN32) 
 if (${CMAKE_GENERATOR} MATCHES ".*Win64")
  set (WIN64 1)
  set (libgen vs2005-64)
 endif()

 set (CMAKE_CURRENT_BINARY_DIR ${libgen})
 if (MSVC)   # Do not generate manifests for the plugins - caused issues loading plugins
   message (STATUS "You should manually modify the VS project to generate a manifest.")
 endif()
endif()

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
set (MS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/${libgen}/)
if (WIN32)
 if (WIN64)
  find_library (MSLIB mshare64 PATHS "${MS_PATH}/Release" NO_DEFAULT_PATH)
 else(WIN64)
  find_library (MSLIB mshare32 PATHS "${MS_PATH}/Release" NO_DEFAULT_PATH)
 endif()

else(WIN32)
 find_library (MSLIB MidiShare PATHS ${MS_PATH}/Release NO_DEFAULT_PATH)
endif()

set (LIBCONTENT ${LIBSRC} ${LIBH})
set (LINK ${JNI_LIBRARIES} ${MSLIB})

if(WIN32)
	enable_language(RC)
	set(LIBCONTENT ${LIBCONTENT} ../win32/JMidi.rc)
endif()

add_library(${target} SHARED ${LIBCONTENT})
set_target_properties (${target} PROPERTIES VERSION ${VERSION})
target_link_libraries (${target} ${LINK})

#######################################
# install settings
if(!WIN32)
set (LIBDEST /usr/lib)

install ( TARGETS ${target}	LIBRARY DESTINATION ${LIBDEST})
endif()