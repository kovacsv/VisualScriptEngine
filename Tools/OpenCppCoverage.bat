pushd ..\Build\Debug\
OpenCppCoverage.exe --sources=*Sources\NodeEngine --sources=*Sources\NodeUIEngine --excluded_sources=*Sources\NodeEngineTest NodeEngineTest.exe
popd
