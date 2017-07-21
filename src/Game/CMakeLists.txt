cmake_minimum_required(VERSION 3.8)

add_executable(Game WIN32 Game.cpp Game.h CGame.cpp CGame.h)
set_property(TARGET Game PROPERTY FOLDER "Runtime")

set_property(TARGET Game APPEND PROPERTY COMPILE_DEFINITIONS "${SHSDK_COMPILE_DEFINITIONS}")
set_property(TARGET Game APPEND PROPERTY COMPILE_FLAGS "${SHSDK_COMPILE_FLAGS}")

target_link_libraries(Game PRIVATE Plugin ${SHSDK_LIBRARIES} ${SHENTRYPOINT_LIBRARY} ${STUBUSERSYSTEM_LIBRARY})
target_include_directories(Game PRIVATE "${SHSDK_INCLUDE_DIRS}")

set_target_properties(Game PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../Cooking/Windows/")