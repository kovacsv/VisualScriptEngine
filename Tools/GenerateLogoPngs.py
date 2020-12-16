import os
import sys
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
	logoSvgFolder = os.path.join (logoFolder, 'svg')
	logoPngFolder = os.path.join (logoFolder, 'png')
	if not os.path.exists (logoPngFolder):
		os.makedirs (logoPngFolder)
	
	for logoName in ['vse_logo', 'vse_logo_simple_dark', 'vse_logo_simple_light', 'vse_logo_simple_text']:
		logoSvgPath = os.path.join (logoSvgFolder, logoName + '.svg')
		for height in [16, 24, 32, 48, 64, 128, 256, 512]:
			heightStr = str (height)
			while len (heightStr) < 3:
				heightStr = '0' + heightStr
			logoPngPath = os.path.join (logoPngFolder, logoName + '_' + heightStr + '.png')
			GeneratePng (inkscapePath, logoSvgPath, logoPngPath, height)
	GeneratePng (
		inkscapePath,
		os.path.join (logoSvgFolder, 'vse_logo_sm.svg'),
		os.path.join (logoPngFolder, 'vse_logo_sm.png'),
		640)

	return 0
	
sys.exit (Main (sys.argv))
