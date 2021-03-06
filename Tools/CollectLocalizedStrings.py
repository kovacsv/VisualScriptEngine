import os
import sys
import re
import codecs

def GetFileContent (fileName):
	file = codecs.open (fileName, 'r', 'utf-8')
	content = file.read ()
	file.close ()
	return content

def GetFilesInDirectory (path):
	fileList = []
	for folder, subs, files in os.walk (path):
		for fileName in files:
			fileList.append (os.path.join (folder, fileName))
	return fileList

def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)
	
	argCount = len (argv)
	if argCount < 3:
		print ('usage: CollectLocalizedStrings.py <sourceFolder>* <resultPoFile>')
		return 1
		
	searchCriterias = [
		'LocString\s*\(L"([^"]+)"\)',
		'LocalizeString\s*\(L"([^"]+)"\)'
	]
	
	sourceFolders = []
	for i in range (1, argCount - 1):
		sourceFolders.append (argv[i])
	resultPoFile = os.path.abspath (argv[argCount - 1])
	
	stringEscapeChar = '\\"'
	stringEscapePlaceholder = '$$$'
	locStrings = []
	for sourceFolder in sourceFolders:
		sourceFiles = GetFilesInDirectory (sourceFolder)
		for sourceFile in sourceFiles:
			fileContent = GetFileContent (sourceFile)
			fileContent = fileContent.replace (stringEscapeChar, stringEscapePlaceholder)
			for searchCriteria in searchCriterias:
				currLocStrings = re.findall (searchCriteria, fileContent)
				for locString in currLocStrings:
					locString = locString.replace (stringEscapePlaceholder, stringEscapeChar);
					if not locString in locStrings:
						locStrings.append (locString)
	
	resultFileDir = os.path.dirname (resultPoFile)
	if not os.path.exists (resultFileDir):
		os.makedirs (resultFileDir)
	poFile = codecs.open (resultPoFile, 'w', 'utf-8')
	poFile.write ('\ufeff')
	for locString in locStrings:
		poFile.write ('msgid "' + locString + '"\n')
		poFile.write ('msgstr "' + locString + '"\n\n')
	poFile.close ()

	return 0
	
sys.exit (Main (sys.argv))
