import os
import sys
import re
import subprocess

def GeneratePng (inkscapePath, sourceSvgPath, resultPngPath, height):
	command = [
		inkscapePath,
		'--export-png=' + resultPngPath,
		'--export-height=' + str (height),
		sourceSvgPath
	]
	subprocess.call (command)

def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	if len (argv) != 2:
		print ('usage: GenerateLogoPngs.py <inkscapePath>')
		return 1

	inkscapePath = sys.argv[1] # "C:\Program Files\Inkscape\inkscape.com"
	
	logoFolder = os.path.abspath (os.path.join ('..', 'Documentation', 'Logo'))
	for logoName in ['vse_logo', 'vse_logo_light', 'vse_logo_dark', 'vse_logo_text_dark']:
		logoSvgPath = os.path.join (logoFolder, 'svg', logoName + '.svg')
		for height in [16, 24, 32, 48, 64, 128, 256, 512]:
			heightStr = str (height)
			while len (heightStr) < 3:
				heightStr = '0' + heightStr
			pngPath = os.path.join (logoFolder, 'png', logoName + '_' + heightStr + '.png')
			GeneratePng (inkscapePath, logoSvgPath, pngPath, height)

	return 0
	
sys.exit (Main (sys.argv))
