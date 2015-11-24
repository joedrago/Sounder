// SoundTest

#include <windows.h>

#include <fcntl.h>
#include <io.h>
#include <stdio.h>



bool InitConsole()
{
	if (!AllocConsole())
		return false;

	// redirect unbuffered STDOUT to the console
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int fileDescriptor = _open_osfhandle((intptr_t)consoleHandle, _O_TEXT);
	FILE *fp = _fdopen(fileDescriptor, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	return true;
}


void PrintLastWinError()
{
	char buff[2048];

	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), 0, buff, sizeof(buff), nullptr);
	printf("%s", buff);
}


int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
	)
{
	if (!InitConsole())
		return 0;

	printf("Printing\n");
	printf("Printing\n");
	printf("Printing\n");

	printf("Done!\n\n");
	Sleep(5000);

	return 0;
}



