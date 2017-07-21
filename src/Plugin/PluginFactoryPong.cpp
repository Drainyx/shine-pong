#include "PluginFactoryPong.h"

CPluginPong * g_pInstance = shNULL;

extern "C"
{

	PLUGIN_PONG_EXPORT void RegisterPluginPong(CShPluginPongListener * pListener)
	{
		if (!g_pInstance)
		{
			g_pInstance = new CPluginPong(pListener);
			SH_ASSERT(g_pInstance);
		}

		ShApplication::RegisterPlugin(g_pInstance);
	}

	PLUGIN_PONG_EXPORT void UnRegisterPluginPong(void)
	{
		ShApplication::UnRegisterPlugin(g_pInstance);
	}

}