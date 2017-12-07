import sys
import os
import shutil
import subprocess

class DevKitBuilder:
	def __init__ (self, rootDirectory):
		self.rootDirectory = rootDirectory
		self.solutionDir = os.path.join (self.rootDirectory, 'Make', 'VS')
		
	def Build (self):
		msBuildPath = os.path.join (os.environ['ProgramFiles(x86)'], 'MSBuild', '14.0', 'Bin', 'MSBuild.exe')
		solutionPath = os.path.join (self.solutionDir, 'VisualScript.sln')
		buildResult = subprocess.call ([
			msBuildPath,
			solutionPath,
			'/property:Configuration=Release',
			'/property:Platform=x64'
		])
		return buildResult
	
	def Test (self):
		binaryFolder = os.path.join (self.solutionDir, 'x64', 'Release')
		testPath = os.path.join (binaryFolder, 'NodeEngineTest.exe')
		return subprocess.call ([testPath])
		
	def Publish (self):
		devKitFolder = os.path.join (self.rootDirectory, 'Make', 'DevKit', 'VS')
		devKitHeaderFolder = os.path.join (devKitFolder, 'Headers')
		devKitLibFolder = os.path.join (devKitFolder, 'Libs')
		
		if os.path.exists (devKitFolder):
			shutil.rmtree (devKitFolder)
		os.makedirs (devKitFolder)
		os.makedirs (devKitHeaderFolder)
		os.makedirs (devKitLibFolder)
		
		binaryFolder = os.path.join (self.solutionDir, 'x64', 'Release')
		sourcesFolder = os.path.join (self.rootDirectory, 'Sources')

		folderNames = ['NodeEngine', 'NodeUIEngine', 'BuiltInNodes', 'WindowsAppSupport']
		for folderName in folderNames:
			shutil.copy (os.path.join (binaryFolder, folderName + '.lib'), devKitLibFolder)
			shutil.copytree (os.path.join (sourcesFolder, folderName, 'Headers'), os.path.join (devKitHeaderFolder, folderName))
		return True

def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)
	if os.name == 'nt':
		builder = DevKitBuilder (os.path.dirname (os.path.dirname (currentDir)))
		print 'Build Solution'
		if builder.Build () != 0:
			print 'ERROR: Build Failed'
			return 1
		print 'Run Tests'
		if builder.Test () != 0:
			print 'ERROR: Test Failed'
			return 1
		print 'Publish DevKit'
		if not builder.Publish ():
			print 'ERROR: Publish Failed'
			return 1
		print 'Success'
	return 0
	
sys.exit (Main (sys.argv))
