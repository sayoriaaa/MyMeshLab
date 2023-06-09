//
// Created by sayori on 2023/4/24.
//

#ifndef MYMESHLAB_UI_UTILS_H
#define MYMESHLAB_UI_UTILS_H

#include "header.h"

//--------------------
// file read for windows
#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#include "commdlg.h"
#define MAX_PATH 260
TCHAR* open_file_dialog()
{
    OPENFILENAME ofn;
    //WCHAR file_name[MAX_PATH] = {0};//file name
    TCHAR* szFile = new TCHAR[MAX_PATH];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = TEXT("all files\0*.*\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        return szFile;
    }
}
#endif

//--------------------
//TO DO: FILE READ FOR LINUX

#endif //MYMESHLAB_UI_UTILS_H