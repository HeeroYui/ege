#!/usr/bin/python
import lutin.debug as debug
import lutin.tools as tools


def get_type():
	return "LIBRARY"

def get_desc():
	return "Ewol Game engine"

def get_licence():
	return "MPL-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return "authors.txt"

def get_version():
	return "version.txt"

def configure(target, my_module):
	my_module.add_extra_flags()
	my_module.add_src_file([
	    'ege/debug.cpp',
	    'ege/Engine.cpp',
	    'ege/Component.cpp',
	    'ege/Environement.cpp',
	    'ege/camera/Camera.cpp',
	    'ege/camera/View.cpp',
	    'ege/camera/FPS.cpp',
	    'ege/camera/ControlBase.cpp',
	    'ege/position/Component.cpp',
	    'ege/physics/Component.cpp',
	    'ege/physics/Engine.cpp',
	    'ege/Entity.cpp',
	    #'ege/elements/ElementBase.cpp',
	    #'ege/elements/ElementPhysic.cpp',
	    'ege/particule/Component.cpp',
	    'ege/particule/Engine.cpp',
	    'ege/particule/Simple.cpp',
	    'ege/ia/Component.cpp',
	    'ege/ia/ComponentLua.cpp',
	    'ege/ia/Engine.cpp',
	    'ege/render/Component.cpp',
	    'ege/render/Engine.cpp',
	    'ege/widget/Mesh.cpp',
	    'ege/widget/Scene.cpp',
	    'ege/resource/Mesh.cpp',
	    'ege/resource/MeshEmf.cpp',
	    'ege/resource/MeshGird.cpp',
	    'ege/resource/MeshCube.cpp',
	    'ege/resource/MeshSphere.cpp',
	    'ege/resource/MeshCylinder.cpp',
	    'ege/resource/MeshCapsule.cpp',
	    'ege/resource/MeshCone.cpp',
	    'ege/resource/MeshObj.cpp',
	    'ege/resource/ParticuleMesh.cpp',
	    'ege/resource/tools/icoSphere.cpp',
	    'ege/resource/tools/isoSphere.cpp',
	    'ege/resource/tools/viewBox.cpp',
	    'ege/Light.cpp',
	    'ege/Material.cpp',
	    'ege/physics/shape/Shape.cpp',
	    'ege/physics/shape/Box.cpp',
	    'ege/physics/shape/Capsule.cpp',
	    'ege/physics/shape/Cone.cpp',
	    'ege/physics/shape/ConvexHull.cpp',
	    'ege/physics/shape/Cylinder.cpp',
	    'ege/physics/shape/Sphere.cpp',
	    'ege/physics/shape/Concave.cpp',
	    'ege/Ray.cpp',
	    ])
	my_module.copy_path('data/ParticuleMesh.*')
	my_module.copy_path('data/material*')
	my_module.add_depend([
	    'ewol',
	    'ephysics',
	    'eproperty',
	    'echrono',
	    'luaWrapper',
	    ])
	my_module.add_flag('c++', [
	    '-Wno-write-strings',
	    '-Wmissing-field-initializers',
	    '-Wall'])
	my_module.add_header_file([
	    'ege/debug.hpp',
	    'ege/Engine.hpp',
	    'ege/Component.hpp',
	    'ege/Environement.hpp',
	    'ege/camera/Camera.hpp',
	    'ege/camera/View.hpp',
	    'ege/camera/FPS.hpp',
	    'ege/camera/ControlBase.hpp',
	    'ege/position/Component.hpp',
	    'ege/physics/Engine.hpp',
	    'ege/physics/Component.hpp',
	    'ege/Entity.hpp',
	    #'ege/elements/ElementBase.hpp',
	    #'ege/elements/ElementPhysic.hpp',
	    'ege/particule/Component.hpp',
	    'ege/particule/Engine.hpp',
	    'ege/particule/Simple.hpp',
	    'ege/ia/Component.hpp',
	    'ege/ia/ComponentLua.hpp',
	    'ege/ia/Engine.hpp',
	    'ege/render/Component.hpp',
	    'ege/render/Engine.hpp',
	    'ege/widget/Mesh.hpp',
	    'ege/widget/Scene.hpp',
	    'ege/resource/Mesh.hpp',
	    'ege/resource/ParticuleMesh.hpp',
	    'ege/resource/tools/icoSphere.hpp',
	    'ege/resource/tools/isoSphere.hpp',
	    'ege/resource/tools/viewBox.hpp',
	    'ege/resource/tools/Face.hpp',
	    'ege/resource/tools/FaceIndexing.hpp',
	    'ege/Light.hpp',
	    'ege/Material.hpp',
	    'ege/physics/shape/Shape.hpp',
	    'ege/physics/shape/Box.hpp',
	    'ege/physics/shape/Capsule.hpp',
	    'ege/physics/shape/Cone.hpp',
	    'ege/physics/shape/ConvexHull.hpp',
	    'ege/physics/shape/Cylinder.hpp',
	    'ege/physics/shape/Sphere.hpp',
	    'ege/physics/shape/Concave.hpp',
	    'ege/Ray.hpp',
	    ])
	# TODO: Remove this ...
	my_module.add_flag('c++', "-Wno-unused-variable")
	my_module.add_flag('c++', "-Wno-overloaded-virtual")
	return True


