#include <Windows.h>
#include <iostream>

int main(int argc, char* argv[])
{
	PVOID redirection;
	Wow64DisableWow64FsRedirection(&redirection);

	HANDLE hFile = CreateFile("C:\\Windows\\System32\\oobe\\info\\backgrounds\\BackgroundDefault.jpg", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	Wow64RevertWow64FsRedirection(redirection);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		OVERLAPPED overlapped;
		memset(&overlapped, 0, sizeof(overlapped));

		DWORD dwFileOffsetHigh;
		DWORD dwFileOffsetLow = GetFileSize(hFile, &dwFileOffsetHigh);

		if (LockFileEx(hFile, LOCKFILE_FAIL_IMMEDIATELY, 0, dwFileOffsetLow, dwFileOffsetHigh, &overlapped))
		{
			for (;;)
				Sleep(10000);

			UnlockFileEx(hFile, 0, dwFileOffsetLow, dwFileOffsetHigh, &overlapped);
			return 0;
		}
	}

	DWORD err = GetLastError();
	std::cerr << "Error: " << err << std::endl;
	MessageBoxA(0, "Cannot lock logon background :(", "", MB_OK | MB_ICONERROR);
	return -1;
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	return main(__argc, __argv);
}
