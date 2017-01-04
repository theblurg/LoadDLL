#pragma once
// LoadDLL.h   : Load a DLL for debugging in your favourite debugger. A lot of malware will put their logic into
//				 a DLL to add an extra step in order to debug it. OllyDbg comes with a DLL loader but not for x64
//				 based binaries. This solution will work with both ia32 and ia64.
// Author:		 theblurg
// Date:		 6/12/2016
// License:		 LGPL V3.0 GNU Lesser General Public License v3 https://www.tldrlegal.com/l/lgpl-3.0
// Full credit for the multi-byte getopt library : Ludvik Jerabek - https://www.codeproject.com/Articles/157001/Full-getopt-Port-for-Unicode-and-Multibyte-Microso
// Modifications to the getopt port involved placing the c file in the static library project to ensure it compiles. No further modifications were made.

#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <tchar.h>
#include <windows.h>
#include <strsafe.h>
#include "getopt.h"
#include "Shlwapi.h"


void displayHelp(void);
BOOL loadDLLFromPath(wchar_t * filePath);
void sleepAfterLoadFunc(void);
void ErrorExit(LPTSTR lpszFunction);

HMODULE loadedImage = NULL;