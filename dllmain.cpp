// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>

// YYTK is in this now
#include "MyPlugin.h"
#include "Assets.h"
#include "LHSprites.h"
// Plugin functionality
#include <fstream>
#include <iterator>
#include "LHCore.h"
#define _CRT_SECURE_NO_WARNINGS

CallbackAttributes_t* CodeCallbackAttr;

// Unload function, remove callbacks here
YYTKStatus PluginUnload()
{
    LHCore::pUnregisterModule(gPluginName);
    return YYTK_OK;
}

bool CoreFoundCallback() // This function is ran once the core is resolved
                         // In this case, we want to wait with registering code callback until the core has registered itself to ensure safe calling
{
    // Change scheduler time
    std::string fileName = Filesys::GetCurrentDir() + "\\options.ini";
    std::string sectionName = "Windows";
    std::string keyName = "SleepMargin";
    std::string newValue = "1";

    // Change settings in "options"
    if (WritePrivateProfileStringA(sectionName.c_str(), keyName.c_str(), newValue.c_str(),  fileName.c_str())) {
        Misc::Print("Changed SleepMargin to value 1");
    }
    else {
        Misc::Print("Error changing SleepMargin");
    }

    
}

// Entry
DllExport YYTKStatus PluginEntry(
    YYTKPlugin* PluginObject // A pointer to the dedicated plugin object
)
{
    gThisPlugin = PluginObject;
    gThisPlugin->PluginUnload = PluginUnload;

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LHCore::ResolveCore, gThisPlugin, 0, NULL); // Check if the Callback Core Module is loaded, and wait for it to load
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CoreFoundCallback, NULL, 0, NULL); // Switch sprites in a different thread as soon as core is loaded
    return YYTK_OK; // Successful PluginEntry.
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        timeBeginPeriod(1);
        DllHandle = hModule; // save our module handle
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

