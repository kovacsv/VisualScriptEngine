import os
import sys
import re
import subprocess
import zipfile

def PrintInfo (message):
	print 'INFO: ' + message

def PrintError (message):
	print 'ERROR: ' + message

def GetVersionFromCMakeLists (cMakeListsPath):
	file = open (cMakeListsPath, 'rb')
	content = file.read ()
	file.close ()
	match1 = re.search (r'set \(VSE_VERSION_1 ([0-9]+)\)', content)
	match2 = re.search (r'set \(VSE_VERSION_2 ([0-9]+)\)', content)
	match3 = re.search (r'set \(VSE_VERSION_3 ([0-9]+)\)', content)
	return ".".join ([match1.group (1), match2.group (1), match3.group (1)])
	
def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)
	
	if len (argv) != 2:
		print 'usage: CreatePackage.py <buildIdentifier>'
		return 1

	buildIdentifier = argv[1]

	cMakeListsPath = os.path.abspath ('CMakeLists.txt')
	version = GetVersionFromCMakeLists (cMakeListsPath)

	packagePath = os.path.abspath (os.path.join ('Build', 'Package'))
	if not os.path.exists (packagePath):
		os.makedirs (packagePath)

	zipPath = os.path.abspath (os.path.join (packagePath, 'VisualScriptEngine_V' + version + '_' + buildIdentifier + '_x64.zip'))
	requiredFolders = [
		os.path.abspath (os.path.join ('Build', 'DevKit', 'include')),
		os.path.abspath (os.path.join ('Build', 'DevKit', 'lib')),
		os.path.abspath (os.path.join ('Build', 'DevKit', 'source')),
		os.path.abspath (os.path.join ('Build', 'DevKit', 'localization'))
	]
	zip = zipfile.ZipFile (zipPath, mode = 'w', compression = zipfile.ZIP_DEFLATED)
	for folder in requiredFolders:
		for file in os.listdir (folder):
			zip.write (os.path.join (folder, file), os.path.join (os.path.basename (folder), file))	
	zip.close ()
	
	return 0
	
sys.exit (Main (sys.argv))
