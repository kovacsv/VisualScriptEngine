import os
import sys
import re
import urllib2
import subprocess

def DownloadFile (url, targetPath):
	print ('Downloading ' + url)
	response = urllib2.urlopen (url)
	content = response.read ()
	file = open (targetPath, 'wb')
	file.write (content)
	file.close ()
	
def UnzipFile (zipFile, targetFolder):
	print ('Unzipping ' + zipFile)
	subprocess.call (['7z', 'x', zipFile, '-o' + targetFolder])

def BuildSolution (msBuildPath, solutionPath, configuration):
	print ('Building ' + solutionPath)
	subprocess.call ([msBuildPath, solutionPath, '/property:Configuration=' + configuration, '/property:Platform=x64'])
	pass

def Main (argv):
	if len (argv) != 3:
		print 'usage: installdepswin.py <targetFolder> <msBuildPath>'
		return 1
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	targetFolder = sys.argv[1]
	msBuildPath = sys.argv[2] # "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe"

	if not os.path.exists (targetFolder):
		os.makedirs (targetFolder)
	
	wxWidgetsName = 'wxWidgets-3.1.2'
	wxWidgetsZipUrl = 'https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.2/wxWidgets-3.1.2.7z'
	wxWidgetsZipPath = os.path.join (targetFolder, wxWidgetsName + '.7z')
	wxWidgetsFolderPath = os.path.join (targetFolder, wxWidgetsName)
	wxWidgetsIncludeFolderPath = os.path.join (wxWidgetsFolderPath, 'include')
	wxWidgetsLibFolderPath = os.path.join (wxWidgetsFolderPath, 'lib', 'vc_x64_lib')
	if not os.path.exists (wxWidgetsIncludeFolderPath) or not os.path.exists (wxWidgetsLibFolderPath):
		DownloadFile (wxWidgetsZipUrl, wxWidgetsZipPath)
		UnzipFile (wxWidgetsZipPath, wxWidgetsFolderPath)
		solutionPath = os.path.join (wxWidgetsFolderPath, 'build', 'msw', 'wx_vc15.sln')
		BuildSolution (msBuildPath, solutionPath, 'Debug')
		BuildSolution (msBuildPath, solutionPath, 'Release')
	return 0
	
sys.exit (Main (sys.argv))
