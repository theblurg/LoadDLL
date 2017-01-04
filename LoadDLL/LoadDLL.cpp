// LoadDLL.cpp : Load a DLL for debugging in your favourite debugger. A lot of malware will put their logic into
//				 a DLL to add an extra step in order to debug it. OllyDbg comes with a DLL loader but not for x64
//				 based binaries. This solution will work with both ia32 and ia64.
// Author:		 theblurg
// Date:		 6/12/2016
// License:		 LGPL V3.0 GNU Lesser General Public License v3 https://www.tldrlegal.com/l/lgpl-3.0
// Full credit for the multi-byte getopt library : Ludvik Jerabek - https://www.codeproject.com/Articles/157001/Full-getopt-Port-for-Unicode-and-Multibyte-Microso
// Modifications to the getopt port involved placing the c file in the static library project to ensure it compiles. No further modifications were made.

#include "LoadDLL.h"


int _tmain(int argc, _TCHAR*  argv[])
{
	wchar_t *fileName;

	BOOL sleepAfterLoad = FALSE;
	BOOL trapToDebugger = FALSE;

	wprintf(L"LoadDLL - Load a DLL in preperation for dynamic analysis.\n");
	wprintf(L"Author: theblurg\n");
	wprintf(L"2016\n");
	wprintf(L"=========================================================\n\n");
	static int verbose_flag;
	int c;

	while (TRUE) {

		static struct option long_options[] =
		{
			{ _T("help"),     ARG_NONE, 0, _T('h') },
			{ _T("trap"),  ARG_NONE, 0, _T('t') },
			{ _T("sleep"),  ARG_NONE,  0, _T('s') },
			{ _T("file"),    ARG_REQ, 0 , _T('f') },
			{ ARG_NULL , ARG_NULL , ARG_NULL , ARG_NULL }
		};

		int option_index = 0;
		c = getopt_long(argc, argv, _T("htsf:"), long_options, &option_index);

		// Check for end of operation or error
		if (c == -1)
			break;
		// Handle options
		switch (c)
		{
		case 0:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0)
				break;
			wprintf(_T("option %s"), long_options[option_index].name);
			if (optarg)
				wprintf(_T(" with arg %s"), optarg);
			_tprintf(_T("\n"));
			break;

		case _T('h'):
			displayHelp();
			break;

		case _T('s'):
			if (!trapToDebugger) {
				sleepAfterLoad = TRUE;
			}
			break;

		case _T('t'):
			if (!sleepAfterLoad) {
				trapToDebugger = TRUE;
			}
			break;

		case _T('f'):
			fileName = optarg;
			wprintf(L"File to load: %s\n", fileName);
			break;

		case '?':
			/* getopt_long already printed an error message. */
			displayHelp();
			break;

		default:
			displayHelp();
		}
	}

	if (fileName != NULL) {
		loadDLLFromPath(fileName);
		ErrorExit(L"loadDLLFromPath");
	}

	if (trapToDebugger) {
		wprintf(L"Trap to debugger....\n");
		// This is hot. No stack frame gets created by this macro.
		// That means our symbols in the debugger won't be mucked up.
		__debugbreak();
	}

	if (sleepAfterLoad) {
		wprintf(L"Sleep after load set\n");
		wprintf(L"Going to sleep ....\n");
		sleepAfterLoadFunc();
	}

	if (optind < argc)
	{
		wprintf(_T("non-option ARGV-elements: "));
		while (optind < argc) _tprintf(_T("%s "), argv[optind++]);
		wprintf(_T("\n"));
	}


	return 0;
}

void displayHelp(void) {
	wprintf(L"Options are:\n");
	wprintf(L"s - Sleep after loading\n");
	wprintf(L"t - Trap to debugger\n");
	wprintf(L"f <filename or absolute path> - file to debug\n");
	wprintf(L"Note that options 't' and 's' are mutually exclusive.\n\n");
}

BOOL loadDLLFromPath(wchar_t * filePath) {
	if (!PathFileExists(filePath)) {
		wprintf(L"Can't find file path \"%s\". Exiting...\n", filePath);
		return FALSE;
	}

	loadedImage = LoadLibrary(filePath);

	if (!loadedImage) {
		ErrorExit(_T("loadDLLFromPath"));
		return FALSE;
	}
	return TRUE;
}
void trapToDebugger(void) {

}
void sleepAfterLoadFunc(void) {
	SleepEx(INFINITE, FALSE);
}

void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	wprintf((LPCTSTR)lpDisplayBuf);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}