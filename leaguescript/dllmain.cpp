#include <thread>
#include <Windows.h>

#include "Core.h"

void OnAttach()
{
	if(Script::Native::Initialize())
	{
		
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    UNREFERENCED_PARAMETER(lpReserved);
    DisableThreadLibraryCalls(hModule);

    if(ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        std::thread(OnAttach).detach();
    }

    return TRUE;
}

