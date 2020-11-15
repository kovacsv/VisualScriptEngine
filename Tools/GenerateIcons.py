import os
import sys
import re
import subprocess
import codecs

def GetFileContent (fileName):
	file = codecs.open (fileName, 'r', 'utf-8')
	content = file.read ()
	file.close ()
	return content

def SetFileContent (fileName, content):
	file = codecs.open (fileName, 'w', 'utf-8')
	file.write (content)
	file.close ()
	
def GenerateColoredIcon (sourceIconPath, coloredSvgIconPath, color):
	rgbColor = None
	if color == 'white':
		rgbColor = '#FAFAFA'
	elif color == 'gray':
		rgbColor = '#BBBBBB'
	else:
		return
	
	sourceContent = GetFileContent (sourceIconPath)
	coloredSvgContent = sourceContent
	coloredSvgContent = re.sub (r'#[0-9a-fA-F]{6}', rgbColor, coloredSvgContent)
	SetFileContent (coloredSvgIconPath, coloredSvgContent)

def GeneratePng (inkscapePath, sourceSvgPath, resultPngPath, size):
	command = [
		inkscapePath,
		'--export-png=' + resultPngPath,
		'--export-width=' + str (size),
		'--export-height=' + str (size),
		sourceSvgPath
	]
	subprocess.call (command)

def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	if len (argv) != 2:
		print ('usage: GenerateIcons.py <inkscapePath>')
		return 1

	inkscapePath = sys.argv[1] # "C:\Program Files\Inkscape\inkscape.com"
	
	iconsPathList = [
		{
			'path' : os.path.abspath (os.path.join ('..', 'Documentation', 'CommandIcons')),
			'generateWhite' : False,
			'generateGray' : True
		},
		{
			'path' : os.path.abspath (os.path.join ('..', 'Documentation', 'NodeIcons')),
			'generateWhite' : True,
			'generateGray' : False
		}
	]
	
	for iconsPath in iconsPathList:
		svgIconsPath = os.path.join (iconsPath['path'], 'svg')

		png18IconsPath = os.path.join (iconsPath['path'], 'png_18')
		if not os.path.exists (png18IconsPath):
			os.makedirs (png18IconsPath)
		png36IconsPath = os.path.join (iconsPath['path'], 'png_36')
		if not os.path.exists (png36IconsPath):
			os.makedirs (png36IconsPath)

		for svgName in os.listdir (svgIconsPath):
			svgBaseName = os.path.splitext (svgName)[0]
			sourceIconPath = os.path.join (svgIconsPath, svgName)
			png18Path = os.path.join (png18IconsPath, svgBaseName + '.png')
			GeneratePng (inkscapePath, sourceIconPath, png18Path, 18)
			png36Path = os.path.join (png36IconsPath, svgBaseName + '.png')
			GeneratePng (inkscapePath, sourceIconPath, png36Path, 36)

		colors = []
		if iconsPath['generateWhite']:
			colors.append ('white')
		if iconsPath['generateGray']:
			colors.append ('gray')
		
		for color in colors:
			svgColoredIconsPath = os.path.join (iconsPath['path'], 'svg_' + color)
			if not os.path.exists (svgColoredIconsPath):
				os.makedirs (svgColoredIconsPath)
			pngColored18IconsPath = os.path.join (iconsPath['path'], 'png_' + color + '_18')
			if not os.path.exists (pngColored18IconsPath):
				os.makedirs (pngColored18IconsPath)
			pngColored36IconsPath = os.path.join (iconsPath['path'], 'png_' + color + '_36')
			if not os.path.exists (pngColored36IconsPath):
				os.makedirs (pngColored36IconsPath)

			for svgName in os.listdir (svgIconsPath):
				svgBaseName = os.path.splitext (svgName)[0]
				sourceIconPath = os.path.join (svgIconsPath, svgName)
				coloredSvgIconPath = os.path.join (svgColoredIconsPath, svgName)
				GenerateColoredIcon (sourceIconPath, coloredSvgIconPath, color)
				pngColored18Path = os.path.join (pngColored18IconsPath, svgBaseName + '.png')
				GeneratePng (inkscapePath, coloredSvgIconPath, pngColored18Path, 18)
				pngColored36Path = os.path.join (pngColored36IconsPath, svgBaseName + '.png')
				GeneratePng (inkscapePath, coloredSvgIconPath, pngColored36Path, 36)

	return 0
	
sys.exit (Main (sys.argv))
