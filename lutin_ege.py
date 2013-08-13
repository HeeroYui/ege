#!/usr/bin/python
import lutinModule
import lutinTools

def Create(target):
	# module name is 'edn' and type binary.
	myModule = lutinModule.module(__file__, 'ege', 'LIBRARY')
	
	# add the file to compile:
	myModule.AddSrcFile([
		'ege/AudioElement.cpp',
		'ege/AudioEngine.cpp',
		'ege/Camera.cpp',
		'ege/ElementGame.cpp',
		'ege/ElementGameIA.cpp',
		'ege/Particule.cpp',
		'ege/ParticuleEngine.cpp',
		'ege/Scene.cpp',
		'ege/Environement.cpp'
		])
	
	# name of the dependency
	myModule.AddModuleDepend(['etk', 'ewol', 'bullet'])
	
	myModule.CompileFlags_CC([
		'-Wno-write-strings',
		'-Wall'])
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__))
	
	
	# add the currrent module at the 
	return myModule


