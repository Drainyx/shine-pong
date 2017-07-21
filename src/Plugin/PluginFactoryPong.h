#pragma once

#include "CPluginPong.h"

extern "C"
{
	PLUGIN_PONG_EXPORT void RegisterPluginPong(CShPluginPongListener * pListener);
	PLUGIN_PONG_EXPORT void UnRegisterPluginPong(void);
}