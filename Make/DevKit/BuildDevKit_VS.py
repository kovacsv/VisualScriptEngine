import sys
import os
import shutil

def RemoveDir (path):
	if os.path.exists (path):
		shutil.rmtree (path)	

def MakeDir (path):
	if not os.path.exists (path):
		os.mkdir (path)	

class DevKitModule:
	def __init__ (self, rootDir, moduleName):
		self.rootDir = rootDir
		self.moduleName = moduleName

	def IsAvailable (self, mode):
		if not os.path.exists (self.GetHeadersDir ()):
			return False
		if not os.path.exists (self.GetLibFilePath (mode)):
			return False
		return True
		
	def Copy (self, mode):
		devKitDir = self.GetDevKitDir (mode)
		MakeDir (devKitDir)
		devKitHeadersDir = os.path.join (devKitDir, 'include')
		devKitLibsDir = os.path.join (devKitDir, 'lib')
		MakeDir (devKitHeadersDir)	
		MakeDir (devKitLibsDir)
		for headerFile in os.listdir (self.GetHeadersDir ()):
			shutil.copy (os.path.join (self.GetHeadersDir (), headerFile), devKitHeadersDir)
		shutil.copy (self.GetLibFilePath (mode), devKitLibsDir)
	
	def GetDevKitDir (self, mode):
		return os.path.join (self.rootDir, 'Make', 'DevKit', 'VS_' + mode)
	
	def GetHeadersDir (self):
		return os.path.join (self.rootDir, 'Sources', self.moduleName, 'Headers')
	
	def GetLibFilePath (self, mode):
		return os.path.join (self.rootDir, 'Make', 'VS', 'x64', mode, self.moduleName + '.lib')
		
def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	rootDir = os.path.abspath (os.path.join (currentDir, '..', '..'))

	modules = [
		DevKitModule (rootDir, 'NodeEngine'),
		DevKitModule (rootDir, 'NodeUIEngine'),
		DevKitModule (rootDir, 'BuiltInNodes'),
		DevKitModule (rootDir, 'WindowsAppSupport'),
		DevKitModule (rootDir, 'wxWidgetsAppSupport')
	]
	
	for mode in ['Debug', 'Release']:
		print 'Building ' + mode + ' DevKit...'
		isAvailable = True
		for module in modules:
			if not module.IsAvailable (mode):
				isAvailable = False
				print '  ' + module.moduleName + ': FAILED'
			else:
				module.Copy (mode)
				print '  ' + module.moduleName + ': SUCCESSFUL'
		if not isAvailable:
			print '  FAILED: One or more modules are missing.'
		else:
			print '  SUCCESSFUL'
		print ''
	return 0
	
sys.exit (Main (sys.argv))
