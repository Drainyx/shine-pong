cmake_minimum_required(VERSION 3.1)

# main file
if (WIN32)
	set(SOURCES_main dllmain.cpp)
	source_group("" FILES ${SOURCES_main})
else(WIN32)
	set(SOURCES_main )
endif (WIN32)

# Plugin files
set(SOURCES_Plugin CPluginPong.cpp CPluginPong.h PluginFactoryPong.cpp PluginFactoryPong.h)
source_group("Plugin" FILES ${SOURCES_Plugin})

# Put everything together before adding platform-specific files
set(SOURCES ${SOURCES_Plugin})
 
# Compile to use it on Runtime
add_library(Plugin STATIC ${SOURCES}) 
set_property(TARGET Plugin PROPERTY FOLDER "Runtime")
set_property(TARGET Plugin APPEND PROPERTY COMPILE_DEFINITIONS "${SHSDK_COMPILE_DEFINITIONS}")
set_property(TARGET Plugin APPEND PROPERTY COMPILE_FLAGS "${SHSDK_COMPILE_FLAGS}")

target_link_libraries(Plugin INTERFACE ${SHSDK_LIBRARIES})
target_include_directories(Plugin PRIVATE "${SHSDK_INCLUDE_DIRS}")
target_include_directories(Plugin INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

# Compile to use it in the Shine Editor
add_library(Plugin_Editor SHARED ${SOURCES})
set_property(TARGET Plugin_Editor PROPERTY FOLDER "Editor")
set_property(TARGET Plugin_Editor APPEND PROPERTY COMPILE_DEFINITIONS "${SHSDK_COMPILE_DEFINITIONS}")
set_property(TARGET Plugin_Editor APPEND PROPERTY COMPILE_FLAGS "${SHSDK_COMPILE_FLAGS}")
set_property(TARGET Plugin_Editor APPEND PROPERTY COMPILE_DEFINITIONS "SH_EDITOR=1" "_INTERNAL_DLL") # FIXME
set_property(TARGET Plugin_Editor APPEND PROPERTY POSITION_INDEPENDENT_CODE True)

target_link_libraries(Plugin_Editor PRIVATE ${SHSDK_EDITOR_LIBRARIES})
target_include_directories(Plugin_Editor PRIVATE "${SHSDK_INCLUDE_DIRS}")
