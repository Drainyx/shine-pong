cmake_minimum_required(VERSION 3.2.1)

#############
### Setup ###
#############
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
	project(${PROJECT_NAME})
endif()

#######################
### Detect platform ###
#######################
if(${CMAKE_SYSTEM_NAME} MATCHES "XboxOne")
	message(STATUS "Detected platform: Xbox One")
	set(SH_XBOXONE 1)
elseif(XBOX360)
	message(STATUS "Detected platform: Xbox 360")
	set(SH_XBOX360 1)
elseif(PS4)
	message(STATUS "Detected platform: PS4")
	set(SH_PS4 1)
elseif(PS3)
	message(STATUS "Detected platform: PS3")
	set(SH_PS3 1)
elseif(PSVITA)
	message(STATUS "Detected platform: PSVITA")
	set(SH_PSVITA 1)
elseif(WIN32)
	message(STATUS "Detected platform: Windows")
	set(SH_PC 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
	message(STATUS "Detected platform: Android")
	set(SH_ANDROID 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message(STATUS "Detected platform: Linux")
	set(SH_LINUX 1)
elseif(APPLE)
	set(SH_APPLE 1)
	if (IOS) # IOS is defined in the toolchain
		message(STATUS "Detected platform: iOS")
		set(SH_IOS 1)
	else () # What about Android on Mac ?
		message(STATUS "Detected platform: Mac")
		set(SH_MAC 1)
	endif ()
else()
	message(STATUS "Detected platform: Unknown")
endif()

#######################
### C++11 and flags ###
#######################
set(CMAKE_CXX_STANDARD 11)
if (MSVC)
	string(REPLACE "/EHsc" "/EHs-c-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")		# Disable exceptions
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")								# Disable RTTI
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS /wd4800")	# Set Warning levels
else (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")					# Disable exceptions
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")							# Disable RTTI
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")						# Set Warning levels
endif (MSVC)

##########################
### Internal Libraries ###
##########################
find_path(SDK_HEADER "ShSDK/ShSDK.h" PATHS ${SHINE_SDK_HEADER_PATH}/Internal NO_CMAKE_FIND_ROOT_PATH)
include_directories(${SDK_HEADER})