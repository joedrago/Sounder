// WinCoreTest

#include <windows.h>

#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <vector>



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


class Keyboard
{
public:
	Keyboard(int num)
	{
		m_keys.resize(num);
	}

	void Register(int key, DWORD vk)
	{
		m_keys[key].Init(vk);
	}

	void Update()
	{
		for (size_t i=0; i<m_keys.size(); ++i)
			m_keys[i].Update();
	}

	bool KeyWentUp(int key) const  { return m_keys[key].WentUp(); }
	bool KeyWentDown(int key) const  { return m_keys[key].WentDown(); }

private:
	class Key
	{
	public:
		Key() : m_vk(0), m_down(false), m_changed(false)  { }

		void Init(DWORD vk)
		{
			m_vk = vk;
		}

		void Update()
		{
			if (!m_vk)
				return;

			typedef SHORT StateType;
			static const StateType s_downMask = (StateType)(1U << (sizeof(StateType) * 8U - 1));

			const StateType state = GetAsyncKeyState(m_vk);
			const bool down = ((state & s_downMask) != 0);

			m_changed = (down != m_down);
			m_down = down;
		}

		bool WentDown() const { return m_changed && m_down; }
		bool WentUp() const { return m_changed && !m_down; }

	private:
		DWORD m_vk;
		bool m_down;
		bool m_changed;
	};

	std::vector<Key> m_keys;
};


int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
	)
{
	if (!InitConsole())
		return 0;

	enum Keys
	{
		Key_One,
		Key_Two,
		Key_Exit,

		NumKeys
	};

	Keyboard keybd(NumKeys);

	keybd.Register(Key_One, 'A');
	keybd.Register(Key_Two, 'B');
	keybd.Register(Key_Exit, VK_ESCAPE);

	for (;;)
	{
		keybd.Update();

		if (keybd.KeyWentDown(Key_One))
			printf("One down\n");
		else if (keybd.KeyWentUp(Key_One))
			printf("One up\n");

		if (keybd.KeyWentDown(Key_Two))
			printf("Two down\n");
		else if (keybd.KeyWentUp(Key_Two))
			printf("Two up\n");

		if (keybd.KeyWentDown(Key_Exit))
			printf("Exiting...\n");
		else if (keybd.KeyWentUp(Key_Exit))
		{
			printf("...Now\n");
			break;
		}
	}

	printf("\n\nDone.\n\n");
	Sleep(5000);

	return 0;
}



