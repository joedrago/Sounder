// SoundTest

#include <windows.h>

#include <Audioclient.h>
#include <Mmdeviceapi.h>
#include <Objbase.h>

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


void PrintWinError(DWORD err)
{
	char buff[2048];

	FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, err, 0, buff, sizeof(buff), nullptr);
	printf("%s", buff);
}

void PrintLastWinError()
{
	PrintWinError(GetLastError());
}

class MyHResult
{
public:
	MyHResult() : m_hr(0)  { }

	void operator= (HRESULT hr) { m_hr = hr; }
	bool Failed() const { return FAILED(m_hr); }
	void Print(const char* prefix = nullptr)
	{
		if (prefix)
			printf("%s:  ", prefix);
		printf("HResult = 0x%08X:  ", (unsigned)m_hr);
		PrintWinError(m_hr);
	}

private:
	HRESULT m_hr;
};

MyHResult myhr;

////////////////////////////////////////////////////////////////////////////////

class SoundSys
{
public:
	SoundSys();

	bool InitSucceeded() const { return m_initSucceeded; }

private:
	bool m_initSucceeded;
};


#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

SoundSys::SoundSys()
: m_initSucceeded(false)
{
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	const IID IID_IAudioClient = __uuidof(IAudioClient);
	const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
//	REFERENCE_TIME hnsActualDuration;
	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioClient *pAudioClient = NULL;
	IAudioRenderClient *pRenderClient = NULL;
	WAVEFORMATEX *pwfx = NULL;
//	UINT32 bufferFrameCount;
//	UINT32 numFramesAvailable;
//	UINT32 numFramesPadding;
//	BYTE *pData;
//	DWORD flags = 0;

	myhr = CoInitializeEx(
				nullptr,
				COINIT_MULTITHREADED | COINIT_SPEED_OVER_MEMORY);
	if (myhr.Failed())
	{
		myhr.Print("CoInitializeEx");
		return;
	}

	myhr = CoCreateInstance(
				CLSID_MMDeviceEnumerator, NULL,
				CLSCTX_ALL, IID_IMMDeviceEnumerator,
				(void**)&pEnumerator);
	if (myhr.Failed())
	{
		myhr.Print("CoCreateInstance");
		return;
	}



	m_initSucceeded = true;
}



////////////////////////////////////////////////////////////////////////////////

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
	)
{
	if (!InitConsole())
		return 0;

	SoundSys sndSys;
	if (sndSys.InitSucceeded())
		printf("SoundSys Init succeeded!\n\n");
	else
		printf("SoundSys Init failed\n\n");

	Sleep(5000);

	return 0;
}



