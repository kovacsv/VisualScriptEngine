import os
import sys
import re
import subprocess

def GeneratePng (inkscapePath, sourceSvgPath, resultPngPath, size, onlyDrawing):
	exportAreaStr = '--export-area-page'
	if onlyDrawing:
		exportAreaStr = '--export-area-drawing'
	command = [
		inkscapePath,
		'--export-png=' + resultPngPath,
		'--export-width=' + str (size),
		'--export-height=' + str (size),
		exportAreaStr,
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
	
	logoFolder = os.path.abspath (os.path.join ('..', 'Documentation', 'Logo'))
	for logoName in ['vse_logo']:
		logoSvgPath = os.path.join (logoFolder, 'svg', logoName + '.svg')
		for size in [16, 32, 64, 128, 256, 512]:
			sizeStr = str (size)
			while len (sizeStr) < 3:
				sizeStr = '0' + sizeStr
			pngPath = os.path.join (logoFolder, 'png', logoName + '_' + sizeStr + '.png')
			onlyDrawing = (size <= 32)
			GeneratePng (inkscapePath, logoSvgPath, pngPath, size, onlyDrawing)

	return 0
	
sys.exit (Main (sys.argv))
