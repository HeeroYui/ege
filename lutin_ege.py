#!/usr/bin/python
import lutinModule
import lutinTools

def Create(target):
	# module name is 'edn' and type binary.
	myModule = lutinModule.module(__file__, 'ege', 'LIBRARY')
	
	# add the file to compile:
	myModule.AddSrcFile([
		'ege/debug.cpp',
		'ege/AudioElement.cpp',
		'ege/AudioEngine.cpp',
		'ege/Camera.cpp',
		'ege/CollisionShapeCreator.cpp',
		'ege/ElementGame.cpp',
		'ege/Particule.cpp',
		'ege/ParticuleEngine.cpp',
		'ege/ParticuleSimple.cpp',
		'ege/Scene.cpp',
		'ege/Environement.cpp',
		'ege/resource/ParticuleMesh.cpp'
		])
	
	myModule.CopyFolder('data/ParticuleMesh.*','')
	
	# name of the dependency
	myModule.AddModuleDepend(['ewol', 'bullet'])
	
	myModule.CompileFlags_CC([
		'-Wno-write-strings',
		'-Wall'])
	
	myModule.AddExportPath(lutinTools.GetCurrentPath(__file__))
	
	
	# add the currrent module at the 
	return myModule


