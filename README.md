## VisualScriptEngine

[![Build Status](https://travis-ci.org/kovacsv/VisualScriptEngine.svg?branch=master)](https://travis-ci.org/kovacsv/VisualScriptEngine)

VisualScriptEngine is an experimental visual scripting engine designed for embedding in another applications.

![Screenshot](Documentation/Screenshots/WindowsTestApp01.png?raw=true "Windows Test Application")

### Building on Windows with Visual Studio 2015

- Build wxWidgets:
  - Download wxWidgets 3.1.0 installer from [here](https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxMSW-3.1.0-Setup.exe).
  - Install wxWidgets to the default location (C:\wxWidgets-3.1.0).
  - Open the solution from C:\wxWidgets-3.1.0\build\msw\wx_vc12.sln.
  - Build with Release/x64 configuration.
- Build VisualScriptEngine:
  - Open the solution from VisualScriptEngine\Make\VS\VisualScript.sln.
  - Build the solution.
