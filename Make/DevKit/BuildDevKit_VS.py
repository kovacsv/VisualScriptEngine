import sys
import os
import shutil
import time

def RemoveDir (path):
	if os.path.exists (path):
		shutil.rmtree (path)	

def MakeDir (path):
	if not os.path.exists (path):
		os.mkdir (path)	

def RemoveAndMakeDir (path):
	RemoveDir (path)
	MakeDir (path)
		
def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	modules = [
		'NodeEngine',
		'NodeUIEngine',
		'BuiltInNodes',
		'WindowsAppSupport',
		'wxWidgetsAppSupport'
	]
	
	rootDir = os.path.abspath (os.path.join (currentDir, '..', '..'))
	devKitRootDir = os.path.join (rootDir, 'Make', 'DevKit')
	devKitIncludeDir = os.path.join (devKitRootDir, 'include')
	
	RemoveAndMakeDir (devKitIncludeDir)
	print 'Copy header files'
	for moduleName in modules:
		print '  Copy header files for ' + moduleName
		moduleHeadersDir = os.path.join (rootDir, 'Sources', moduleName, 'Headers')
		for headerFile in os.listdir (moduleHeadersDir):
			shutil.copy (os.path.join (moduleHeadersDir, headerFile), devKitIncludeDir)
	print '  SUCCESSFUL'
	
	for mode in ['Debug', 'Release']:
		print ''
		print 'Copy ' + mode + ' library files'
		devKitLibsDir = os.path.join (devKitRootDir, 'vs_' + mode.lower ())
		RemoveAndMakeDir (devKitLibsDir)
		success = True
		for moduleName in modules:
			print '  Copy library files for ' + moduleName
			moduleLibFile = os.path.join (rootDir, 'Make', 'VS', 'x64', mode, moduleName + '.lib')
			if not os.path.exists (moduleLibFile):
				print '  FAILED: ' + moduleLibFile + ' does not exist'
				success = False
			else:
				shutil.copy (moduleLibFile, devKitLibsDir)
		if success:
			print '  SUCCESSFUL'
		else:
			print '  FAILED: One ore more module files are missing'

	return 0
	
result = Main (sys.argv)
time.sleep (3)
sys.exit (result)
