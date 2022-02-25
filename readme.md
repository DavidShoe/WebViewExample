# Setup
This is a test bed application for exercising the legacy javascript debugger plugin for Visual Sudio 2019 / 2022.  This is a dev tool and not designed for public release due to hardcover paths and dependencies.

## Visual Studio setup
    Load the webviewexample.sln file in Visual Studio
    Update the C++ include directory to point to the location of your edge build output
        IE: E:\ana\src\out\debug_x64\gen\edge_embedded_browser\client\win\current

    Build the webviewexample.exe

## Missing DLL Depencency
Copy icuuc.dll from your ana\src\out\debug_x64 directory to webviewexample\out\debug\x64\webviewexample this will fix a runtime dependency error resulting in a "webview2loader.dll was not found error".

