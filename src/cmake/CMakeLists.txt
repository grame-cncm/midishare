#######################################
# CMAKE midishare
#######################################
project(midishare)
cmake_minimum_required(VERSION 2.6)

set (target MidiShare)
#######################################
# versions management
set (VERSION 1.93)
set (STRVERS "v.1.93")
set (SSTRVERS "1.93")

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
set (SRC_PATH  ${ROOT_PATH})
set (TOOLS_PATH  ${ROOT_PATH}/mstools)
set (SRCFOLDERS  common/Clients common/Drivers common/Kernel common/Memory  common/Sorter)
set (HEADERS_PATH ${SRC_PATH}/common/Headers ${SRC_PATH}/common/atomic ${SRC_PATH}/lib)
if(WIN32)
	set(IMPL_PATH ${ROOT_PATH}/win-mono)
elseif(APPLE)
	set(IMPL_PATH ${ROOT_PATH}/macosx-mono)
elseif(UNIX)
	set(IMPL_PATH ${ROOT_PATH}/linux)
endif(WIN32)


#######################################
# add src and headers files
foreach(folder ${SRCFOLDERS})
	set (SRC ${SRC} "${SRC_PATH}/${folder}/*.c")					# add source files
endforeach(folder)
set (SRC ${SRC} "${SRC_PATH}/common/atomic/lffifo.c"  "${SRC_PATH}/common/atomic/lflifo.c")

if(WIN32)
	set (SRC ${SRC} "${SRC_PATH}/common/atomic/msAtomicWin32.asm")
	set (SRC ${SRC} "${IMPL_PATH}/*.c")					# add source files
elseif(APPLE)
	set (SRC ${SRC} "${IMPL_PATH}/*.c"  "${SRC_PATH}/lib/msPrefs.c" "${SRC_PATH}/lib/profport.c")
	set (HEADERS_PATH ${HEADERS_PATH} ${SRC_PATH}/lib)
elseif(UNIX)
	set (SRC ${SRC} "${IMPL_PATH}/library*.c" "${SRC_PATH}/lib/profport.c")		# add source files
endif(WIN32)
file (GLOB LIBSRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})

set (PUBLICHEADERS ${SRC_PATH}/include)

foreach(folder ${HEADERS_PATH})
	set(HEADERS ${HEADERS} "${folder}/*.h")		# add header files
endforeach(folder)
file (GLOB LIBH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${HEADERS})
file (GLOB PUBLICHEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${PUBLICHEADERS}/*.h")

#######################################
# set includes
foreach(folder ${HEADERS_PATH})
	set(INCL ${INCL} "${folder}")				# add include folders
endforeach(folder)

include_directories( ${INCL})
set_source_files_properties (${INCL} PROPERTIES HEADER_FILE_ONLY TRUE)

#######################################
# set input libraries
if(APPLE)
	set(LINK "-framework CoreAudio")
endif(APPLE)

if(WIN32)
	set(LINK "winmm.lib")
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


#######################################
# set tools targets
set (MSTOOLS MidiGetIndAppl MidiGetTime MidiIsConnected MidiConnect MidiGetName MidiGetVersion
MidiCountAppls MidiGetNamedAppl)

include_directories( ${SRC_PATH}/include)

foreach(tool ${MSTOOLS})
	add_executable( ${tool} ${TOOLS_PATH}/${tool}.c )
	target_link_libraries( ${tool} ${target})
	add_dependencies(${tool} ${target})
endforeach(tool)
