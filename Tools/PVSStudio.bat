"C:\Program Files (x86)\PVS-Studio\PVS-Studio_Cmd.exe" --target ../Build/VisualScriptEngine.sln --excludeProjects NodeEngineTest --rulesConfig rules.pvsconfig --output ../Build/Debug/VisualScriptEngine.plog
mkdir ..\Build\Debug\PVSStudio
"C:\Program Files (x86)\PVS-Studio\PlogConverter.exe" ../Build/Debug/VisualScriptEngine.plog --outputDir ../Build/Debug/PVSStudio -t Html
start ..\Build\Debug\PVSStudio\VisualScriptEngine.plog.html
