import os
import sys
import re
import subprocess

def GetFileContent (fileName):
	file = open (fileName, 'rb')
	content = file.read ()
	file.close ()
	return content

def SetFileContent (fileName, content):
	file = open (fileName, 'wb')
	file.write (content)
	file.close ()
	
def GenerateWhiteIcon (sourceIconPath, whiteSvgIconPath):
	sourceContent = GetFileContent (sourceIconPath)
	whiteSvgContent = sourceContent
	whiteSvgContent = re.sub (r'#[0-9a-fA-F]{6}', '#FAFAFA', whiteSvgContent)
	SetFileContent (whiteSvgIconPath, whiteSvgContent)

def GeneratePngIcon (inkscapePath, sourceIconPath, pngIconPath, dpi):
	command = [
		inkscapePath,
		'--export-png=' + pngIconPath,
		'--export-dpi=' + str (dpi),
		sourceIconPath
	]
	subprocess.call (command)

def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	if len (argv) != 2:
		print ('usage: GenerateIcons.py <inkscapePath>')
		return 1

	inkscapePath = sys.argv[1] # "C:\Program Files\Inkscape\inkscape.com"
	
	nodeIconsPath = os.path.abspath (os.path.join ('..', 'Documentation', 'NodeIcons'))
	svgIconsPath = os.path.join (nodeIconsPath, 'svg')
	svgWhiteIconsPath = os.path.join (nodeIconsPath, 'svg_white')
	if not os.path.exists (svgWhiteIconsPath):
		os.makedirs (svgWhiteIconsPath)
	png18IconsPath = os.path.join (nodeIconsPath, 'png_18')
	if not os.path.exists (png18IconsPath):
		os.makedirs (png18IconsPath)
	pngWhite18IconsPath = os.path.join (nodeIconsPath, 'png_white_18')
	if not os.path.exists (pngWhite18IconsPath):
		os.makedirs (pngWhite18IconsPath)
	png36IconsPath = os.path.join (nodeIconsPath, 'png_36')
	if not os.path.exists (png36IconsPath):
		os.makedirs (png36IconsPath)
	pngWhite36IconsPath = os.path.join (nodeIconsPath, 'png_white_36')
	if not os.path.exists (pngWhite36IconsPath):
		os.makedirs (pngWhite36IconsPath)
	
	for svgName in os.listdir (svgIconsPath):
		svgBaseName = os.path.splitext (svgName)[0]
		sourceIconPath = os.path.join (svgIconsPath, svgName)
		whiteSvgIconPath = os.path.join (svgWhiteIconsPath, svgName)
		GenerateWhiteIcon (sourceIconPath, whiteSvgIconPath)
		png18Path = os.path.join (png18IconsPath, svgBaseName + '.png')
		GeneratePngIcon (inkscapePath, sourceIconPath, png18Path, 96)
		pngWhite18Path = os.path.join (pngWhite18IconsPath, svgBaseName + '.png')
		GeneratePngIcon (inkscapePath, whiteSvgIconPath, pngWhite18Path, 96)
		png36Path = os.path.join (png36IconsPath, svgBaseName + '.png')
		GeneratePngIcon (inkscapePath, sourceIconPath, png36Path, 192)
		pngWhite36Path = os.path.join (pngWhite36IconsPath, svgBaseName + '.png')
		GeneratePngIcon (inkscapePath, whiteSvgIconPath, pngWhite36Path, 192)

	return 0
	
sys.exit (Main (sys.argv))
