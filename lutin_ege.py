#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools


def get_type():
	return "LIBRARY"

def get_desc():
	return "Ewol Game engine (based on bullet lib)"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def get_version():
	return [0,1,0]

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_src_file([
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
		'ege/resource/MeshCube.cpp',
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
	my_module.copy_path('data/ParticuleMesh.*')
	my_module.add_module_depend(['ewol', 'bullet-physics'])
	my_module.compile_flags('c++', [
		'-Wno-write-strings',
		'-Wmissing-field-initializers',
		'-Wall'])
	my_module.add_header_file([
		'ege/debug.h',
		'ege/AudioElement.h',
		'ege/AudioEngine.h',
		'ege/camera/Camera.h',
		'ege/camera/View.h',
		'ege/camera/FPS.h',
		'ege/CollisionShapeCreator.h',
		'ege/physics/Engine.h',
		'ege/elements/Element.h',
		'ege/elements/ElementBase.h',
		'ege/elements/ElementPhysic.h',
		'ege/Particule.h',
		'ege/ParticuleEngine.h',
		'ege/ParticuleSimple.h',
		'ege/widget/Mesh.h',
		'ege/widget/Scene.h',
		'ege/Environement.h',
		'ege/resource/Mesh.h',
		'ege/resource/ParticuleMesh.h',
		'ege/resource/tools/icoSphere.h',
		'ege/resource/tools/isoSphere.h',
		'ege/resource/tools/viewBox.h',
		'ege/resource/tools/Face.h',
		'ege/resource/tools/FaceIndexing.h',
		'ege/Light.h',
		'ege/Material.h',
		'ege/physicsShape/PhysicsShape.h',
		'ege/physicsShape/PhysicsBox.h',
		'ege/physicsShape/PhysicsCapsule.h',
		'ege/physicsShape/PhysicsCone.h',
		'ege/physicsShape/PhysicsConvexHull.h',
		'ege/physicsShape/PhysicsCylinder.h',
		'ege/physicsShape/PhysicsSphere.h',
		'ege/Ray.h',
		])
	my_module.add_path(tools.get_current_path(__file__))
	return my_module


