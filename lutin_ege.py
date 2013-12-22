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
		'ege/widget/Mesh.cpp',
		'ege/widget/Scene.cpp',
		'ege/Environement.cpp',
		'ege/resource/Mesh.cpp',
		'ege/resource/ParticuleMesh.cpp',
		'ege/Light.cpp',
		'ege/Material.cpp',
		'ege/physicsShape/PhysicsShape.cpp',
		'ege/physicsShape/PhysicsBox.cpp',
		'ege/physicsShape/PhysicsCapsule.cpp',
		'ege/physicsShape/PhysicsCone.cpp',
		'ege/physicsShape/PhysicsConvexHull.cpp',
		'ege/physicsShape/PhysicsCylinder.cpp',
		'ege/physicsShape/PhysicsSphere.cpp',
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


