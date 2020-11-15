import os
import sys
import re
import platform
import zipfile
import codecs

def PrintInfo (message):
	print ('INFO: ' + message)

def PrintError (message):
	print ('ERROR: ' + message)

def GetVersionFromCMakeLists (cMakeListsPath):
	file = codecs.open (cMakeListsPath, 'r', 'utf-8')
	content = file.read ()
	file.close ()
	match1 = re.search (r'set \(VSE_VERSION_1 ([0-9]+)\)', content)
	match2 = re.search (r'set \(VSE_VERSION_2 ([0-9]+)\)', content)
	match3 = re.search (r'set \(VSE_VERSION_3 ([0-9]+)\)', content)
	return ".".join ([match1.group (1), match2.group (1), match3.group (1)])
	
def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)
	
	if len (argv) != 4:
		print ('usage: CreatePackage.py <devKitDir> <packageName> <buildType>')
		return 1

	devKitDir = argv[1]
	packageName = argv[2]
	buildType = argv[3]

	cMakeListsPath = os.path.abspath ('CMakeLists.txt')
	version = GetVersionFromCMakeLists (cMakeListsPath)

	buildDir = os.path.abspath ('Build')
	packagePath = os.path.join (buildDir, 'Package')
	if not os.path.exists (packagePath):
		os.makedirs (packagePath)

	zipPath = os.path.abspath (os.path.join (packagePath, 'VisualScriptEngine_V' + version + '_' + packageName + '_' + buildType + '_x64.zip'))
	requiredFolders = [
		os.path.abspath (os.path.join (devKitDir, 'include')),
		os.path.abspath (os.path.join (devKitDir, 'lib')),
		os.path.abspath (os.path.join (devKitDir, 'source')),
		os.path.abspath (os.path.join (devKitDir, 'localization'))
	]
	zip = zipfile.ZipFile (zipPath, mode = 'w', compression = zipfile.ZIP_DEFLATED)
	for folder in requiredFolders:
		for file in os.listdir (folder):
			zip.write (os.path.join (folder, file), os.path.join (os.path.basename (folder), file))	

	if platform.system () == 'Windows':
		if buildType in ['Debug', 'RelWithDebInfo']:
			libPath = os.path.abspath (os.path.join (devKitDir, 'lib'))
			for module in ['NodeEngine', 'NodeUIEngine', 'BuiltInNodes', 'WindowsAppSupport']:
				pdbPath = os.path.join (buildDir, module + '.dir', buildType, module + '.pdb')
				zip.write (pdbPath, os.path.join (os.path.basename (libPath), module + '.pdb'))
		outputPath = os.path.abspath (os.path.join (buildDir, buildType))
		for testFile in ['WindowsReferenceApp.exe']:
			zip.write (os.path.join (outputPath, testFile), os.path.join ('testapp', testFile))

	zip.close ()
	
	return 0
	
sys.exit (Main (sys.argv))
