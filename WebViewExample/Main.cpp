
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <wrl.h>
#include <algorithm>
#include <vector>
#include "WebView2.h"

using namespace Microsoft::WRL;

static wchar_t szWindowClass[] = L"DesktopApp";
static wchar_t szTitle[] = L"WebView sample";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;
std::vector<ComPtr<ICoreWebView2Controller>> m_webviewWindows;
HWND m_hwndMain;
std::wstring m_exe_path;
std::wstring m_url;
HMENU IDE_CREATE = (HMENU)102;
DWORD m_webviewTop = 25;

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
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
    m_hwndMain = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        1100, 200,
        1200, 900,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (!m_hwndMain)
    {
        return E_FAIL;
    }

    // Show the window
    ShowWindow(m_hwndMain, nCmdShow);
    UpdateWindow(m_hwndMain);

    // Get the url that we want to display in the webview from our current path
    wchar_t system_buffer[MAX_PATH];
    system_buffer[0] = 0;
    DWORD len = ::GetModuleFileName(hInstance, system_buffer, MAX_PATH);
    if (len == 0 || len > MAX_PATH)
        return E_FAIL;
    std::wstring fullPath(system_buffer, len);
    size_t found = fullPath.find_last_of(L"/\\");
    std::wstring path = fullPath.substr(0, found);
    m_exe_path = path;
    std::replace(path.begin(), path.end(), L'\\', L'/');
    std::wstring url(L"file:///");
    url += path;
    url += L"/index.html";
    m_url = url;
    OutputDebugString(L"\r\n");
    OutputDebugString(m_url.c_str());
    OutputDebugString(L"\r\n");

    // Create a single WebView within the parent window
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    HWND createButtonHWND = CreateWindow(
        L"button", L"Create WebView",
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, 0,
        0, 150, m_webviewTop, m_hwndMain,
        IDE_CREATE, nullptr, 0);

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void LayoutWebViews()
{
    RECT bounds;
    GetClientRect(m_hwndMain, &bounds);

    size_t s = m_webviewWindows.size();
    size_t w = bounds.right - bounds.left;

    size_t width = (w / s) - (s - 1);

    for (size_t i = 0; i < m_webviewWindows.size(); i++)
    {
        RECT win = bounds;
        win.top = m_webviewTop;
        win.left = (LONG)(i * width + i * 2);
        win.right = (LONG)(i * width + width);
        m_webviewWindows[i]->put_Bounds(win);
    }
}

void CreateWebView()
{
    std::wstring folder = m_exe_path;
    folder.append(L"\\webview_");
    folder.append(std::to_wstring(m_webviewWindows.size()));
    // Locate the browser and set up the environment for WebView
    OutputDebugString(L"\r\nFolder: ");
    OutputDebugString(folder.c_str());
    OutputDebugString(L"\r\n");

    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
        L"E:\\ana\\src\\out\\debug_x64",        // Browser folder
        folder.c_str(),                         // User environment folder
        nullptr,                                // Options
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [&](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                // Create a WebView, whose parent is the main window hWnd
                result = env->CreateCoreWebView2Controller(m_hwndMain, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [&](HRESULT result, ICoreWebView2Controller* webview) -> HRESULT {
                        if (webview == nullptr)
                        {
                            result = E_FAIL;
                        }
                        else
                        {
                            m_webviewWindows.push_back(webview);

                            // Add a few settings for the webview
                            ICoreWebView2* cwv2 = nullptr;
                            ICoreWebView2Settings* Settings;

                            webview->get_CoreWebView2(&cwv2);
                            cwv2->get_Settings(&Settings);
                            Settings->put_IsScriptEnabled(TRUE);
                            Settings->put_AreDefaultScriptDialogsEnabled(TRUE);
                            Settings->put_IsWebMessageEnabled(TRUE);

                            // Resize WebView to fit the bounds of the parent window
                            LayoutWebViews();

                            // Load our html content
                            std::wstring url = m_url;
                            url.append(L"?i=");
                            url.append(std::to_wstring(m_webviewWindows.size() - 1));

                            result = cwv2->Navigate(url.c_str());
                        }
                        return result;
                    })
                    .Get());
                return result;
            })
        .Get());
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_SIZE:
        if (!m_webviewWindows.empty())
        {
            LayoutWebViews();
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == LOWORD(IDE_CREATE))
        {
            CreateWebView();
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}
