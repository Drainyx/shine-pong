# - Try to find ShSDK
# Once done this will define
#  SHSDK_FOUND - System has ShSDK
#  SHSDK_INCLUDE_DIRS - The ShSDK include directories
#  SHSDK_LIBRARIES - The libraries needed to use ShSDK

find_path(SDK_HEADER "ShSDK/ShSDK.h" HINTS ENV SHINE_BASE_DIR "../Shine/" PATHS /usr/local/ PATH_SUFFIXES "include")

macro(FIND_SHSDK_LIBRARY _var)
	find_library(${_var} NAMES ${ARGN} HINTS ENV SHINE_BASE_DIR "../Shine/" PATHS /usr/local/ PATH_SUFFIXES "lib")
	mark_as_advanced(${_var})
endmacro()

# ------------------------------------------------------------------------------------------------

if (MSVC)
	set(SHSDK_COMPILE_FLAGS "/EHs-c- /GR-") # disable exceptions / disable RTTI
else (MSVC)
	set(SHSDK_COMPILE_FLAGS "-fno-exceptions -fno-rtti")
endif (MSVC)

set(SHSDK_COMPILE_DEFINITIONS "SH_$<UPPER_CASE:$<CONFIG>>=1")

if (MSVC)
	list(APPEND SHSDK_COMPILE_DEFINITIONS "_ITERATOR_DEBUG_LEVEL=0")
endif (MSVC)

# ------------------------------------------------------------------------------------------------

FIND_SHSDK_LIBRARY(SHSDK_LIBRARY ShSDK)
FIND_SHSDK_LIBRARY(SHENTRYPOINT_LIBRARY ShEntryPoint)
FIND_SHSDK_LIBRARY(STUBUSERSYSTEM_LIBRARY StubUserSystem)
FIND_SHSDK_LIBRARY(SHSDK_EDITOR_LIBRARY ShSDK_Editor)

# ------------------------------------------------------------------------------------------------

set(SHSDK_LIBRARIES "${SHSDK_LIBRARY}")
set(SHSDK_EDITOR_LIBRARIES "${SHSDK_EDITOR_LIBRARY}")
set(SHSDK_INCLUDE_DIRS "${SDK_HEADER}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Android")

	# TODO

elseif (APPLE)

	# TODO

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

	find_package(Threads QUIET)
	find_package(OpenGL QUIET)
	find_package(OpenAL QUIET)
	find_package(X11 QUIET)

	list(APPEND SHSDK_LIBRARIES "${OPENGL_LIBRARIES}" "${X11_LIBRARIES}" "${OPENAL_LIBRARY}" "${CMAKE_THREAD_LIBS_INIT}")

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

	find_package(DirectX9 QUIET)
	find_package(XAudio QUIET)
	find_package(XInput QUIET)

	list(APPEND SHSDK_INCLUDE_DIRS "${DirectX9_INCLUDE_DIR}")
	list(APPEND SHSDK_LIBRARIES "${DirectX9_LIBRARIES}" "${XInput_LIBRARIES}" "${XAudio_LIBRARY}")

else ()

	# TODO

	message("Platform not supported")

endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(ShSDK DEFAULT_MSG SHSDK_LIBRARIES SHSDK_EDITOR_LIBRARIES SHSDK_INCLUDE_DIRS)
