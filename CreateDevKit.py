import os
import sys
import subprocess

def PrintInfo (message):
	print ('INFO: ' + message)

def PrintError (message):
	print ('ERROR: ' + message)
	
def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)
	
	if len (argv) != 3:
		print ('usage: CreateDevKit.py <msBuildPath> <msBuildConfiguration>')
		return 1

	msBuildPath = argv[1]
	msBuildConfiguration = argv[2]

	installProjectPath = os.path.join ('Build', 'INSTALL.vcxproj')
	buildResult = subprocess.call ([
		msBuildPath,
		installProjectPath,
		'-verbosity:m',
		'/property:Configuration=' + msBuildConfiguration
	])
	if buildResult != 0:
		PrintError ('Failed to build install project.')
		return 1
	
	return 0
	
sys.exit (Main (sys.argv))
