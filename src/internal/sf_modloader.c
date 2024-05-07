#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sf_utility.h"
#include "../api/sfsf.h"
#include "sf_modloader.h"

extern SpellforceSpellFramework frameworkAPI;

typedef void (*InitModuleFunc)(void*);

void cleanup(void* modHandle) {
    // Free resources (unload mod library using FreeLibrary)
    FreeLibrary((HMODULE)modHandle);
}

// Function to extract filename from the path
const char* get_filename(const char* path) {
    const char* filename = strrchr(path, '\\'); // Find the last occurrence of '\' in the path
    if (filename == NULL) {
        // If '\' is not found, return the entire path
        return path;
    } else {
        // Otherwise, return the substring after the last '\' (excluding '\')
        return filename + 1;
    }
}

void load_mod(const char* modPath, void* pFrameworkAPI) {
    // Using same method for loading a library we load the mods other people make
    HMODULE modHandle = LoadLibrary(modPath);
    if (modHandle) {
        // Obtain the address of the initializeModule function within the mod
        InitModuleFunc initModule = (InitModuleFunc)GetProcAddress(modHandle, "InitModule");
        if (initModule) {
            // Execute the initializeModule function with the framework API
            initModule(pFrameworkAPI);

            char infomsg[256] = "Initialized Mod: ";
            const char* filename = get_filename(modPath);
            strcat(infomsg, filename);
            logInfo(infomsg);
        } else {
            logError("Failed to get address of InitModule");
            cleanup(modHandle);
        }
    } else {
        logError("Failed to load mod library");
    }
}

void load_all_mods(const char* subfolder, void* pFrameworkAPI) {
    // Grab the current working directory
    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);

    // Construct the directory path with subfolder
    char modDirectory[MAX_PATH];
    sprintf(modDirectory, "%s\\%s", currentDir, subfolder);

    // Find all .sfm (DLL) files in the specified directory
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(strcat(modDirectory, "\\*.sfm"), &findFileData);
    OutputDebugStringA("LOADING ALL MODS!!!!");
    char msgbuf[MAX_PATH];
    sprintf(msgbuf, "From Dir: %d\n", modDirectory);
    OutputDebugStringA(modDirectory);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // Load each mod found
            char modPath[MAX_PATH];
            sprintf(modDirectory, "%s\\%s", currentDir, subfolder);
            sprintf(modPath, "%s\\%s", modDirectory, findFileData.cFileName);
            load_mod(modPath, pFrameworkAPI);
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    } else {
        logError("Failed to find mods in specified directory");
        char msgbuf[MAX_PATH];
        sprintf(msgbuf, "From Dir: %d\n", modDirectory);
        logError(strcat(msgbuf, modDirectory));
    }
}


void initMods(){
    // load all the mods, we'll also need to at some point use this section to provide framework api functions to the mod as well?
    load_all_mods("sfsf", &frameworkAPI);
}