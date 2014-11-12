#!/usr/bin/python
import lutinModule as module
import lutinTools as tools

def get_desc():
	return "EGE : Ewol Game engine (based on bullet lib)"


def create(target):
	# module name is 'edn' and type binary.
	myModule = module.Module(__file__, 'ege', 'LIBRARY')
	
	# add the file to compile:
	myModule.add_src_file([
		'ege/debug.cpp',
		'ege/AudioElement.cpp',
		'ege/AudioEngine.cpp',
		'ege/camera/Camera.cpp',
		'ege/camera/View.cpp',
		'ege/camera/FPS.cpp',
		'ege/CollisionShapeCreator.cpp',
		'ege/physics/Engine.cpp',
		'ege/elements/Element.cpp',
		'ege/elements/ElementBase.cpp',
		'ege/elements/ElementPhysic.cpp',
		'ege/Particule.cpp',
		'ege/ParticuleEngine.cpp',
		'ege/ParticuleSimple.cpp',
		'ege/widget/Mesh.cpp',
		'ege/widget/Scene.cpp',
		'ege/Environement.cpp',
		'ege/resource/Mesh.cpp',
		'ege/resource/MeshEmf.cpp',
		'ege/resource/MeshGird.cpp',
		'ege/resource/MeshObj.cpp',
		'ege/resource/ParticuleMesh.cpp',
		'ege/resource/tools/icoSphere.cpp',
		'ege/resource/tools/isoSphere.cpp',
		'ege/resource/tools/viewBox.cpp',
		'ege/Light.cpp',
		'ege/Material.cpp',
		'ege/physicsShape/PhysicsShape.cpp',
		'ege/physicsShape/PhysicsBox.cpp',
		'ege/physicsShape/PhysicsCapsule.cpp',
		'ege/physicsShape/PhysicsCone.cpp',
		'ege/physicsShape/PhysicsConvexHull.cpp',
		'ege/physicsShape/PhysicsCylinder.cpp',
		'ege/physicsShape/PhysicsSphere.cpp',
		'ege/Ray.cpp',
		])
	
	myModule.copy_folder('data/ParticuleMesh.*','')
	
	# name of the dependency
	myModule.add_module_depend(['ewol', 'bullet'])
	
	myModule.compile_flags_CC([
		'-Wno-write-strings',
		'-Wall'])
	
	myModule.add_export_path(tools.get_current_path(__file__))
	
	
	# add the currrent module at the 
	return myModule


