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
	return "authors.txt"

def get_version():
	return "version.txt"

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
	my_module.add_depend(['ewol', 'bullet-physics'])
	my_module.add_flag('c++', [
		'-Wno-write-strings',
		'-Wmissing-field-initializers',
		'-Wall'])
	my_module.add_header_file([
		'ege/debug.hpp',
		'ege/AudioElement.hpp',
		'ege/AudioEngine.hpp',
		'ege/camera/Camera.hpp',
		'ege/camera/View.hpp',
		'ege/camera/FPS.hpp',
		'ege/CollisionShapeCreator.hpp',
		'ege/physics/Engine.hpp',
		'ege/elements/Element.hpp',
		'ege/elements/ElementBase.hpp',
		'ege/elements/ElementPhysic.hpp',
		'ege/Particule.hpp',
		'ege/ParticuleEngine.hpp',
		'ege/ParticuleSimple.hpp',
		'ege/widget/Mesh.hpp',
		'ege/widget/Scene.hpp',
		'ege/Environement.hpp',
		'ege/resource/Mesh.hpp',
		'ege/resource/ParticuleMesh.hpp',
		'ege/resource/tools/icoSphere.hpp',
		'ege/resource/tools/isoSphere.hpp',
		'ege/resource/tools/viewBox.hpp',
		'ege/resource/tools/Face.hpp',
		'ege/resource/tools/FaceIndexing.hpp',
		'ege/Light.hpp',
		'ege/Material.hpp',
		'ege/physicsShape/PhysicsShape.hpp',
		'ege/physicsShape/PhysicsBox.hpp',
		'ege/physicsShape/PhysicsCapsule.hpp',
		'ege/physicsShape/PhysicsCone.hpp',
		'ege/physicsShape/PhysicsConvexHull.hpp',
		'ege/physicsShape/PhysicsCylinder.hpp',
		'ege/physicsShape/PhysicsSphere.hpp',
		'ege/Ray.hpp',
		])
	my_module.add_path(tools.get_current_path(__file__))
	return my_module


