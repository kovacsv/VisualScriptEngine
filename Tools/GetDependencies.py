import os
import sys
import urllib2
import zipfile
import time

os.chdir (os.path.dirname (os.path.abspath (__file__)))

dependenciesFolder = os.path.join ('..', 'Dependencies')
dependencies = [
	{
		'url' : 'https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.1/wxWidgets-3.1.1.zip',
		'target' : 'wxWidgets-3.1.1'
	}
]

class ProgressBar:
	def __init__ (self, full):
		self.full = full
	
	def Progress (self, completed):
		percent = int (float (completed) / float (self.full) * 100.0)
		sys.stdout.write ('\r')
		sys.stdout.write ('[%-20s] %d%%' % ('=' * (percent / 5), percent))
		sys.stdout.flush ()
	
	def Finish (self):
		self.Progress (self.full)
		sys.stdout.write ('\n')
		sys.stdout.flush ()

def DownloadFile (url, targetFileName):
	fileRequest = urllib2.urlopen (url)
	targetFile = open (targetFileName, 'wb')
	sourceFileSize = int (fileRequest.info ().getheader ('Content-Length'))
	chunkSize = 8192
	progressBar = ProgressBar (float (sourceFileSize))
	downloadedBytes = 0
	while True:
		buffer = fileRequest.read (chunkSize)
		if not buffer:
			progressBar.Finish ()
			break
		downloadedBytes += len (buffer)
		progressBar.Progress (downloadedBytes)
		targetFile.write (buffer)
	targetFile.close ()

class Dependency:
	def Get (self):
		pass
	
	def Install (self):
		pass

def Main ():
	if not os.path.exists (dependenciesFolder):
		os.mkdir (dependenciesFolder)
	for dependency in dependencies:
		sourceFileUrl = dependency['url']
		targetZipFileName = os.path.join (dependenciesFolder, os.path.split (sourceFileUrl)[1])
		targetFolder = os.path.join (dependenciesFolder, dependency['target'])
		if not os.path.exists (targetZipFileName):
			print 'Downloading ' + sourceFileUrl
			DownloadFile (sourceFileUrl, targetZipFileName)
		if not os.path.exists (targetFolder):
			print 'Unzipping ' + targetZipFileName
			zipFile = zipfile.ZipFile (targetZipFileName, 'r')
			zipFile.extractall (targetFolder)
			zipFile.close ()
	return 0

sys.exit (Main ())
