#include <stddef.h>

#ifndef __VstJuno6_H__
#include "VstJuno6.h"
#endif

#if MAC
#pragma export on
#endif

//------------------------------------------------------------------------
// Prototype of the export function main
//------------------------------------------------------------------------
#if BEOS
#define main main_plugin
extern "C" __declspec(dllexport) AEffect *main_plugin(audioMasterCallback audioMaster);

#elif MACX
#define main main_macho
extern "C" AEffect *main_macho(audioMasterCallback audioMaster);

#else
AEffect *main(audioMasterCallback audioMaster);
#endif

//------------------------------------------------------------------------
AEffect* __cdecl main(audioMasterCallback audioMaster)
{
	
	// Get VST Version
	if (!audioMaster(0, audioMasterVersion, 0, 0, 0, 0))
		return 0; // old version

	// Create the AudioEffect
	VstJuno6 *effect = new VstJuno6(audioMaster);
	if (!effect)
		return 0;

	return effect->getAeffect();
}

#if MAC
#pragma export off
#endif

//------------------------------------------------------------------------
#if WIN32
#include <windows.h>
void *hInstance;
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved)
{
	hInstance = hInst;
	return 1;
}
#endif
