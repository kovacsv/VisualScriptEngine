"C:\Program Files (x86)\PVS-Studio\PVS-Studio_Cmd.exe" --target ../Build/VisualScriptEngine.sln --excludeProjects NodeEngineTest --rulesConfig rules.pvsconfig --output ../Build/Debug/VisualScriptEngine.plog
"C:\Program Files (x86)\PVS-Studio\PlogConverter.exe" ../Build/Debug/VisualScriptEngine.plog --outputDir ../Build/Debug -t FullHtml
start ..\Build\Debug\fullhtml\index.html
