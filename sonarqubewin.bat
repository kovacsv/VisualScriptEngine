cd /D "%~dp0\Build"
SonarQube.Scanner.MSBuild.exe begin /k:"kovacsv_VisualScriptEngine" /d:sonar.organization="kovacsv-github" /d:sonar.cfamily.build-wrapper-output=bw-output /d:sonar.host.url="https://sonarcloud.io" /d:sonar.login="e500043bc7e5e882c710a025dcaf2eee77822d5f" /d:sonar.projectBaseDir="%~dp0\Sources"
build-wrapper-win-x86-64.exe --out-dir bw-output "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe" "VisualScriptEngine.sln" /p:Configuration=Debug /t:Rebuild
SonarQube.Scanner.MSBuild.exe end /d:sonar.login="e500043bc7e5e882c710a025dcaf2eee77822d5f"
cd /D "%~dp0"
