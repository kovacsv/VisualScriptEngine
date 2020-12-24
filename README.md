# Visual Script Engine

A visual scripting engine designed for embedding. The engine is written in modern C++ and compiles on several platforms with no external dependencies. Some platform dependent user interface utilities are also provided for Windows and MacOS platforms.

[![Build Status](https://travis-ci.com/kovacsv/VisualScriptEngine.svg?branch=master)](https://travis-ci.com/kovacsv/VisualScriptEngine)
[![Build Status](https://ci.appveyor.com/api/projects/status/c0qxxixyhymfxfdf?svg=true)](https://ci.appveyor.com/project/kovacsv/visualscriptengine)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/kovacsv/VisualScriptEngine.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/kovacsv/VisualScriptEngine/context:cpp)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/kovacsv/VisualScriptEngine.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/kovacsv/VisualScriptEngine/alerts/)
[![codecov](https://codecov.io/gh/kovacsv/VisualScriptEngine/branch/master/graph/badge.svg?token=LAB72UD38J)](https://codecov.io/gh/kovacsv/VisualScriptEngine)

## Documentation

Documentation pages in the [Wiki](https://github.com/kovacsv/VisualScriptEngine/wiki):
* [Building the Engine](https://github.com/kovacsv/VisualScriptEngine/wiki/Building-the-Engine)
* [Source Structure](https://github.com/kovacsv/VisualScriptEngine/wiki/Source-Structure)
* [Embedding Tutorial](https://github.com/kovacsv/VisualScriptEngine/wiki/Embedding-Tutorial)
* [Anatomy of a Node](https://github.com/kovacsv/VisualScriptEngine/wiki/Anatomy-of-a-Node)

## Screenshots

Windows demo application:

![Screenshot](Documentation/WindowsEmbeddingDemo01.png?raw=true "Windows Embedding Demo")

MacOS demo application:

![Screenshot](Documentation/MacOSEmbeddingDemo01.png?raw=true "MacOS Embedding Demo")

Running in a browser with [VisualScriptEngineWeb](https://github.com/kovacsv/VisualScriptEngineWeb):

![Screenshot](Documentation/WebEmbeddingDemo01.png?raw=true "Web Embedding Demo")

## References

[PARAM-O](https://graphisoft.com/param-o), the visual scripting solution in [Archicad](https://graphisoft.com/solutions/products/archicad) is powered by VisualScriptEngine.

## Other

- [VisualScriptEngineWeb](https://github.com/kovacsv/VisualScriptEngineWeb) is an Emscripten port of the engine running in a browser.
- [VisualScriptEngineWxWidgets](https://github.com/kovacsv/VisualScriptEngineWxWidgets) is an implementation of the user inteface elements for wxWidgets.
