# LoadDLL
Load a DLL for debugging in your favourite debugger. A lot of malware will put their logic into a DLL to add an extra step in order to debug it. OllyDbg comes with a DLL loader but not for x64 based binaries. This solution will work with both ia32 and ia64.

This project uses has two main mechanisms for debugging. Either loading the DLL and then calling break-to-debugger. Alternatively you can have it load and then go to sleep so you can attach to the process. With the first option if you have the AEDebug registry key set correctly once the DLL loads Windows will trap to the debugger so you can begin dynamic analyis. 

License:		 LGPL V3.0 GNU Lesser General Public License v3 https://www.tldrlegal.com/l/lgpl-3.0

Full credit for the multi-byte getopt library : Ludvik Jerabek - https://www.codeproject.com/Articles/157001/Full-getopt-Port-for-Unicode-and-Multibyte-Microso

Modifications to the getopt port involved placing the c file in the static library project to ensure it compiles. No further modifications were made.
