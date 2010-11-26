#######################################
# CMAKE player
#######################################
project(midishare)
cmake_minimum_required(VERSION 2.6)

set (target Player)

#######################################
# versions management
set (VERSION 2.12)
set (STRVERS "v.2.12")
set (SSTRVERS "2.12")

if(UNIX)
	add_definitions(-Wall -DGCC)
endif(UNIX)

if(APPLE)
	set (CMAKE_OSX_ARCHITECTURES "x86_64 i386")
	set (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
	set (CMAKE_C++_FLAGS -mmacosx-version-min=10.4)
 	add_definitions(-D__Macintosh__ -D__MacOSX__)
endif(APPLE)

if(WIN32)
	add_definitions(-DWINVER=0x0400 -DWIN32 -D__x86_64__)
endif(WIN32)

#######################################
# set directories, src and headers.
set (ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/..)
set (SRC_PATH  ${ROOT_PATH}/src)

set (SRCFOLDERS  
	Chasers Events Factory Library Loop MidiFile Players Score Synchroniser Tracks Utils
	Framework/Events Framework/MidiAppl Framework/Scheduler Framework/Score Framework/Tempo)

set (HEADERS_PATH ${SRC_PATH}/Include)


#######################################
# add src and headers files
foreach(folder ${SRCFOLDERS})
	set (SRC ${SRC} "${SRC_PATH}/${folder}/*.c" "${SRC_PATH}/${folder}/*.cpp")	# add source files
endforeach(folder)
file (GLOB LIBSRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})

foreach(folder ${HEADERS_PATH})
	set(HEADERS ${HEADERS} "${folder}/*.h")		# add header files
endforeach(folder)
file (GLOB LIBH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${HEADERS})

set (PUBLICHEADERS ${SRC_PATH}/Include/Player.h)

#######################################
# set includes
include_directories( ${HEADERS_PATH} ${ROOT_PATH}/../../src/include)
set_source_files_properties (${INCL} PROPERTIES HEADER_FILE_ONLY TRUE)

#######################################
# set input libraries
if(APPLE)
	set(LINK "-framework MidiShare")
endif(APPLE)

if(WIN32)
	set(LINK "mshare.lib")
endif(WIN32)

#######################################
# set library target
if(WIN32)
	enable_language(RC)
	set(LIBCONTENT ${LIBSRC} ${LIBH} ${ROOT_PATH}/win-mono/midishare.rc)
	set(LINK_FLAGS /ENTRY:"DllEntryPoint")
else(WIN32)
	set(LIBCONTENT ${LIBSRC} ${LIBH})
endif(WIN32)


add_library(${target} SHARED ${LIBCONTENT})
set_target_properties (${target} PROPERTIES
			FRAMEWORK TRUE
			PUBLIC_HEADER "${PUBLICHEADERS}"
			VERSION ${VERSION}
			MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${STRVERS}
			MACOSX_FRAMEWORK_BUNDLE_VERSION ${SSTRVERS}
			MACOSX_FRAMEWORK_BUNDLE_VERSION "A"
)
if(WIN32)
set_target_properties (${target} PROPERTIES LINK_FLAGS /ENTRY:DllEntryPoint)
endif(WIN32)

target_link_libraries (${target} ${LINK})
