import os
import sys
import re
import shutil
import subprocess
import datetime
import urllib
import urllib2

def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	sourcesDir = os.path.abspath (os.path.join ('..', 'Sources'))
	cppCheckPath = 'C:\\Program Files\\Cppcheck\\cppcheck.exe'
	command = [
		cppCheckPath,
		sourcesDir,
		'--enable=all',
		'--suppress=unusedFunction',
		'--suppress=noExplicitConstructor',
		'--suppress=cstyleCast',
		'--suppress=missingInclude',
		'-iNodeEngineTest',
		'-iwxWidgetsAppSupport',
		'-iwxWidgetsTestApp'
	]
	proc = subprocess.Popen (command, shell = True, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
	stdout, stderr = proc.communicate ()
	print stderr
	
	return 0
	
sys.exit (Main (sys.argv))
