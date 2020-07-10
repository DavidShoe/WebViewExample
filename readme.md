# Setup
This is a test bed application for exercising the two javascript debugger plugins.  This is a dev tool and not designed for public release due to hardcover paths and dependencies.

## Visual Studio setup
    Load the webviewexample.sln file in Visual Studio
    Update the C++ include directory to point to the location of your edge build output
        IE: E:\ana\src\out\debug_x64\gen\edge_embedded_browser\client\win\current

    Build the webviewexample.exe

    You are now down with Visual Studio
## Missing DLL Depencency
Copy icuuc.dll from your ana\src\out\debug_x64 directory to webviewexample\out\debug\x64\webviewexample this will fix a runtime dependency error resulting in a "webview2loader.dll was not found error".

## Visual Code setup
### Update the launch.json
"runtimeExecutable" change this to the path of the webviewexaple.exe you just built
"path" update the path to match your build envrionment;
    IE: "Path": "%path%;e://ana//src//out//debug_x64;E:/github/WebViewExample/out/Debug/x64/WebViewExample/WebViewExample"

### Install Debugger add ins
 Install the old debug adapter: "Debugger for Microsoft Edge"
 Install the new debug adapter: "JavaScript Debugger" 

## Usage
Scenario 0 is c++ debugging of the test application
Scenarios 1-3 are using the old debug adapter
Scenarios 4-6 are using the new debug adapter
### Scenario 1
    To Test basic debugger functionality of the old debugger add in.  This will attach to the first webview created.
### Scenario 2
    To test targeted webview debugger functionality.  This will attach to the first webview that navigates to a URL that matches the value set in "UrlFilter".
### Scenario 3
    To test basic debugger functionality of the new debugger add in.  This will attach to the first webview created
### Scenario 4
    To test targeted webview debugger functionality.  This will attach to the first webview that navigates to a URL that matches the value set in "UrlFilter".
    NOTE: Currently not working
