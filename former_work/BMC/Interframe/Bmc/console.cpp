#include "stdafx.h"
#include <windows.h>
#include <process.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

// iostream.h and fstream.h are no longer in VS 2003
// This makes it compatible in VisualStudio.2003
// 
// Modified by GangCh, Aug 26, 2003

#define VS_2003

#ifdef VS_2003
    #include <ios>
    using namespace std;
#else
    #include <iostream.h>
    #include <fstream.h>
#endif


#define MAX_CONSOLE_LINES 2500;
HANDLE  g_hConsoleOut; // Handle to debug console
void RedirectIOToConsole(const char title[]);
// This function dynamically creates a "Console" window and points stdout and stderr to it.
// It also hooks stdin to the window
// You must free it later with FreeConsole
void RedirectIOToConsole(const char title[])
{
    int  hConHandle;
    long lStdHandle;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    FILE   *fp;
    // allocate a console for this app
	AllocConsole();
	// set the screen buffer to be big enough to let us scroll text
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),	&coninfo);
    coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	// How many lines do you want to have in the console buffer
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),	coninfo.dwSize);
	// redirect unbuffered STDOUT to the console
    g_hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 ); 
	// redirect unbuffered STDIN to the console
    lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE); 
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "r" ); 
	*stdin = *fp;
    setvbuf( stdin, NULL, _IONBF, 0 );
    // redirect unbuffered STDERR to the console
    lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE); 
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    fp = _fdopen( hConHandle, "w" );
    *stderr = *fp;
    setvbuf( stderr, NULL, _IONBF, 0 );
	SetConsoleTitle(title); 
	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
	// Uncomment the next line if you are using c++ cio or comment if you don't
#ifdef VS_2003
    ios_base::sync_with_stdio();
#else
    ios::sync_with_stdio();
#endif

}
