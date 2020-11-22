import os
import sys
import re
import subprocess
import codecs
from PIL import Image

def GetFileContent (fileName):
	file = codecs.open (fileName, 'r', 'utf-8')
	content = file.read ()
	file.close ()
	return content

def SetFileContent (fileName, content):
	file = codecs.open (fileName, 'w', 'utf-8')
	file.write (content)
	file.close ()
	
def GenerateColoredSvg (sourceIconPath, coloredSvgIconPath, color):
	sourceContent = GetFileContent (sourceIconPath)
	coloredSvgContent = sourceContent
	coloredSvgContent = re.sub (r'#[0-9a-fA-F]{6}', color, coloredSvgContent)
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

def GenerateColoredSvgsFromSvgs (sourceFolder, targetFolder, color):
	if not os.path.exists (targetFolder):
		os.makedirs (targetFolder)
	for svgName in os.listdir (sourceFolder):
		sourceIconPath = os.path.join (sourceFolder, svgName)
		coloredSvgIconPath = os.path.join (targetFolder, svgName)
		GenerateColoredSvg (sourceIconPath, coloredSvgIconPath, color)	

def GeneratePngsFromSvgs (inkscapePath, iconsPath, svgFolderName, pngFolderPrefix, sizes):
	svgIconsPath = os.path.join (iconsPath, svgFolderName)
	for svgName in os.listdir (svgIconsPath):
		for size in sizes:
			pngIconsPath = os.path.join (iconsPath, pngFolderPrefix + str (size))
			if not os.path.exists (pngIconsPath):
				os.makedirs (pngIconsPath)
			svgBaseName = os.path.splitext (svgName)[0]
			sourceIconPath = os.path.join (svgIconsPath, svgName)
			targetPngPath = os.path.join (pngIconsPath, svgBaseName + '.png')
			GeneratePng (inkscapePath, sourceIconPath, targetPngPath, size)
			
def GeneratePngsFromSvgs (inkscapePath, sourcePath, targetPath, size):
	if not os.path.exists (targetPath):
		os.makedirs (targetPath)
	for svgName in os.listdir (sourcePath):
		svgBaseName = os.path.splitext (svgName)[0]
		sourceIconPath = os.path.join (sourcePath, svgName)
		targetPngPath = os.path.join (targetPath, svgBaseName + '.png')
		GeneratePng (inkscapePath, sourceIconPath, targetPngPath, size)			

def GenerateBmpsFromPngs (sourcePath, targetPath):
	if not os.path.exists (targetPath):
		os.makedirs (targetPath)
	for pngName in os.listdir (sourcePath):
		pngBaseName = os.path.splitext (pngName)[0]
		sourcePngPath = os.path.join (sourcePath, pngName)
		targetBmpPath = os.path.join (targetPath, pngBaseName + '.bmp')
		image = Image.open (sourcePngPath)	
		image.save (targetBmpPath)

def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	if len (argv) != 2:
		print ('usage: GenerateIcons.py <inkscapePath>')
		print ('example: GenerateIcons.py "C:\Program Files\Inkscape\inkscape.com"')
		return 1

	inkscapePath = sys.argv[1]
	
	iconDescriptors = [
		{
			'path' : os.path.abspath (os.path.join ('..', 'Documentation', 'CommandIcons')),
			'settings' : {
				'colors' : [
					{ 'name' : 'gray', 'color' : '#BBBBBB' }
				],
				'pngs' : [
					{
						'color' : None,
						'sizes' : [18, 36],
						'bmp' : True
					},
					{
						'color' : 'gray',
						'sizes' : [18, 36],
						'bmp' : True
					}
				]
			}
		},
		{
			'path' : os.path.abspath (os.path.join ('..', 'Documentation', 'NodeIcons')),
			'settings' : {
				'colors' : [
					{ 'name' : 'white', 'color' : '#FAFAFA' }
				],
				'pngs' : [
					{
						'color' : None,
						'sizes' : [18, 36],
						'bmp' : True
					},
					{
						'color' : 'white',
						'sizes' : [18, 36],
						'bmp' : True
					}
				]
			}
		}
	]
	
	for iconDescriptor in iconDescriptors:
		svgIconsPath = os.path.join (iconDescriptor['path'], 'svg')
		for color in iconDescriptor['settings']['colors']:
			coloredSvgIconsPath = os.path.join (iconDescriptor['path'], 'svg_' + color['name'])
			GenerateColoredSvgsFromSvgs (svgIconsPath, coloredSvgIconsPath, color['color'])
		for png in iconDescriptor['settings']['pngs']:
			svgSourcePath = os.path.join (iconDescriptor['path'], 'svg')
			colorStr = ''
			if png['color'] != None:
				colorStr = '_' + png['color']
				svgSourcePath = os.path.join (iconDescriptor['path'], 'svg' + colorStr)
			for size in png['sizes']:
				pngTargetPath = os.path.join (iconDescriptor['path'], 'png' + colorStr + '_' + str (size))
				GeneratePngsFromSvgs (inkscapePath, svgSourcePath, pngTargetPath, size)
				if png['bmp']:
					bmpTargetPath = os.path.join (iconDescriptor['path'], 'bmp' + colorStr + '_' + str (size))
					GenerateBmpsFromPngs (pngTargetPath, bmpTargetPath)
	return 0
	
sys.exit (Main (sys.argv))
