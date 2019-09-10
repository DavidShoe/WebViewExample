
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <wrl.h>
#include <algorithm>
#include "WebView2.h"

using namespace Microsoft::WRL;

static wchar_t szWindowClass[] = L"DesktopApp";
static wchar_t szTitle[] = L"WebView sample";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;
static ComPtr<IWebView2WebView> webviewWindow;

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    // Create the desktop window class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        return E_FAIL;
    }

    hInst = hInstance;

    // Create the window
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        1100, 200,
        1200, 900,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        return E_FAIL;
    }

    // Show the window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Get the url that we want to display in the webview from our current path
    wchar_t system_buffer[MAX_PATH];
    system_buffer[0] = 0;
    DWORD len = ::GetModuleFileName(hInstance, system_buffer, MAX_PATH);
    if (len == 0 || len > MAX_PATH)
        return E_FAIL;
    std::wstring fullPath(system_buffer, len);
    size_t found = fullPath.find_last_of(L"/\\");
    std::wstring path = fullPath.substr(0, found);
    std::replace(path.begin(), path.end(), L'\\', L'/');
    std::wstring url(L"file:///");
    url += path;
    url += L"/src/index.html";

    // Create a single WebView within the parent window
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    // Locate the browser and set up the environment for WebView
    CreateWebView2EnvironmentWithDetails(L"F:\\git\\Edge\\src\\out\\debug_x64", nullptr, nullptr,
        Callback<IWebView2CreateWebView2EnvironmentCompletedHandler>(
            [hWnd, url](HRESULT result, IWebView2Environment* env) -> HRESULT {

                // Create a WebView, whose parent is the main window hWnd
                env->CreateWebView(hWnd, Callback<IWebView2CreateWebViewCompletedHandler>(
                    [hWnd, url](HRESULT result, IWebView2WebView* webview) -> HRESULT {
                        if (webview != nullptr)
                        {
                            webviewWindow = webview;
                        }

                        // Add a few settings for the webview
                        IWebView2Settings* Settings;
                        webviewWindow->get_Settings(&Settings);
                        Settings->put_IsScriptEnabled(TRUE);
                        Settings->put_AreDefaultScriptDialogsEnabled(TRUE);
                        Settings->put_IsWebMessageEnabled(TRUE);

                        // Resize WebView to fit the bounds of the parent window
                        RECT bounds;
                        GetClientRect(hWnd, &bounds);
                        webviewWindow->put_Bounds(bounds);

                        // Load our html content
                        webviewWindow->Navigate(url.c_str());

                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
        if (webviewWindow != nullptr)
        {
            RECT bounds;
            GetClientRect(hWnd, &bounds);
            webviewWindow->put_Bounds(bounds);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}
