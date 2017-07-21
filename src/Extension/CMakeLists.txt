cmake_minimum_required(VERSION 3.1)

set(SHINE_EDITOR_EXT_DIR "" CACHE PATH "Extension directory")
set(SHSDK_EDITOR_LIBRARIES "" CACHE FILEPATH "ShSDK (editor) path") # FIXME

# main file
if (WIN32)
	set(SOURCES_main dllmain.cpp)
	source_group("" FILES ${SOURCES_main})
else(WIN32)
	set(SOURCES_main )
endif (WIN32)

# Extension files
set(SOURCES_Extension Extension.cpp Extension.h)
source_group("Extension" FILES ${SOURCES_Extension})

# Put everything together before adding platform-specific files
set(SOURCES ${SOURCES_main} ${SOURCES_Extension})

add_library(ExtPong SHARED ${SOURCES})
set_property(TARGET ExtPong PROPERTY FOLDER "Editor")
set_property(TARGET ExtPong APPEND PROPERTY COMPILE_DEFINITIONS "${SHSDK_COMPILE_DEFINITIONS}")
set_property(TARGET ExtPong APPEND PROPERTY COMPILE_FLAGS "${SHSDK_COMPILE_FLAGS}")
set_property(TARGET ExtPong APPEND PROPERTY COMPILE_DEFINITIONS "SH_$<UPPER_CASE:$<CONFIG>>=1")
set_property(TARGET ExtPong APPEND PROPERTY COMPILE_DEFINITIONS "SH_EDITOR=1" "_INTERNAL_DLL") # FIXME

target_link_libraries(ExtPong PRIVATE ${SHSDK_EDITOR_LIBRARIES})
target_include_directories(ExtPong PRIVATE "${SHSDK_INCLUDE_DIRS}")

if (WIN32)
	set_property(TARGET ExtPong PROPERTY RUNTIME_OUTPUT_DIRECTORY "${SHINE_EDITOR_EXT_DIR}")
	set_property(TARGET ExtPong PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG "${SHINE_EDITOR_EXT_DIR}")
	set_property(TARGET ExtPong PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE "${SHINE_EDITOR_EXT_DIR}")
else (WIN32)
	set_property(TARGET ExtPong PROPERTY LIBRARY_OUTPUT_DIRECTORY "${SHINE_EDITOR_EXT_DIR}")
	set_property(TARGET ExtPong PROPERTY LIBRARY_OUTPUT_DIRECTORY_DEBUG "${SHINE_EDITOR_EXT_DIR}")
	set_property(TARGET ExtPong PROPERTY LIBRARY_OUTPUT_DIRECTORY_RELEASE "${SHINE_EDITOR_EXT_DIR}")
endif (WIN32)

set_property(TARGET ExtPong PROPERTY PREFIX "")
set_property(TARGET ExtPong PROPERTY DEBUG_POSTFIX "")
set_property(TARGET ExtPong PROPERTY RELEASE_POSTFIX "")

file(MAKE_DIRECTORY "${SHINE_EDITOR_EXT_DIR}")
file(COPY "Pong.configext" DESTINATION "${SHINE_EDITOR_EXT_DIR}")
