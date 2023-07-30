#pragma once
#include <Windows.h>
#include <string>
#include <AclAPI.h>
#include <LM.h>
#include "resource.h"
#include <filesystem>
using namespace std;
extern bool IsFirstRun();
extern void user();
extern void all();
bool ExtractResource(int iId, LPCWSTR pDest);
extern void CrashORRebootWindows();