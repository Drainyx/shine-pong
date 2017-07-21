#include "Extension.h"

#include "ShSDK/ShSDK.h"

typedef void (*register_fn)(void*);
typedef void (*unregister_fn)(void);

register_fn pRegister = shNULL;
unregister_fn pUnRegister = shNULL;

const CShString strPluginFilePath("C:\\Users\\Guillaume\\Desktop\\Shine\\test\\Build\\src\\Plugin\\Debug\\Plugin_Editor.dll");
static CShDynamicLibrary plugin_library(strPluginFilePath);

extern "C"
{

EXTENSION_EXPORT void ExtensionInitialize(void)
{
	// ...
}

EXTENSION_EXPORT void ExtensionRelease(void)
{
	// ...
}

EXTENSION_EXPORT void ExtensionOnPluginStarted(const CShIdentifier & levelIdentifier)
{
	//
	// Load Library
	bool bInit = plugin_library.Initialize();

	//
	// Get Functions
	if (bInit)
	{
		pRegister = (register_fn)plugin_library.GetFunctionAddress(CShString("RegisterPluginPong"));
		SH_ASSERT(shNULL != pRegister);

		pUnRegister = (unregister_fn)plugin_library.GetFunctionAddress(CShString("UnRegisterPluginPong"));
		SH_ASSERT(shNULL != pUnRegister);
	}

	//
	// Call factory (auto-register)
	if (shNULL != pRegister)
	{
		pRegister(shNULL);
	}
}

EXTENSION_EXPORT void ExtensionOnPluginStopped(const CShIdentifier & levelIdentifier)
{
	//
	// Call factory (auto-unregister)
	if (shNULL != pUnRegister)
	{
		pUnRegister();
	}

	//
	// Reset function pointers
	{
		pRegister = shNULL;
		pUnRegister = shNULL;
	}

	//
	// Unload Library
	plugin_library.Release();
}

}
