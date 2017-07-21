cmake_minimum_required(VERSION 3.2.1)

set(packager_cmd "$ENV{SHINE_BASE_DIR}/bin/tools/ShGamePackager.exe")
	
# ConfigureShineSAAS(${BUILD_FILE_PATH} ${BUILD_CONFIG_NAME}) # generation du Manifest, des icones etc ...
macro(ConfigureShineSAAS _BuildFilePath _BuildConfigName)

	execute_process(COMMAND ${packager_cmd} "--configure" ${_BuildFilePath} ${_BuildConfigName}
		RESULT_VARIABLE res_var)
		
	if(NOT "${res_var}" STREQUAL "0")
		message("ShGamePackager --configure failed with error '${res_var}'")
	endif()

endmacro()

# CopyToToShineSAAS(${BUILD_FILE_PATH} ${BUILD_CONFIG_NAME} ${TARGET_NAME})
macro(CopyToToShineSAAS _BuildFilePath _BuildConfigName _TargetName)

#	execute_process(COMMAND ${packager_cmd} "--copy" ${_BuildFilePath} ${_BuildConfigName} $<TARGET_FILE:${EXECUTABLE_NAME}>
#		RESULT_VARIABLE res_var)
#		
#	if(NOT "${res_var}" STREQUAL "0")
#		message("ShGamePackager --copy failed with error '${res_var}'")
#	endif()
	
	add_custom_target(AutoBuild
		COMMAND ${packager_cmd} "--copy" ${_BuildFilePath} ${_BuildConfigName} $<TARGET_FILE:${EXECUTABLE_NAME}>
		DEPENDS ${EXECUTABLE_NAME})
	
endmacro()

  