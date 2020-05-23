pushd ..\Build\Debug\
OpenCppCoverage.exe --sources=*Sources\NodeEngine --sources=*Sources\NodeUIEngine --sources=*Sources\BuiltInNodes --excluded_sources=*Sources\NodeEngineTest NodeEngineTest.exe
popd
