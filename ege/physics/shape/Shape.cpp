/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/debug.hpp>
#include <ege/physics/shape/Shape.hpp>
#include <ege/physics/shape/Box.hpp>
#include <ege/physics/shape/Capsule.hpp>
#include <ege/physics/shape/Cone.hpp>
#include <ege/physics/shape/ConvexHull.hpp>
#include <ege/physics/shape/Cylinder.hpp>
#include <ege/physics/shape/Sphere.hpp>
#include <ege/physics/shape/Concave.hpp>


ememory::SharedPtr<ege::physics::Shape> ege::physics::Shape::create(const etk::String& _name) {
	ememory::SharedPtr<ege::physics::Shape> tmpp = nullptr;
	etk::String name = etk::tolower(_name);
	if (name == "box") {
		tmpp = ememory::makeShared<ege::physics::shape::Box>();
	} else if (name == "sphere") {
		tmpp = ememory::makeShared<ege::physics::shape::Sphere>();
	} else if (name == "cone") {
		tmpp = ememory::makeShared<ege::physics::shape::Cone>();
	} else if (name == "cylinder") {
		tmpp = ememory::makeShared<ege::physics::shape::Cylinder>();
	} else if (name == "capsule") {
		tmpp = ememory::makeShared<ege::physics::shape::Capsule>();
	} else if (name == "convexhull") {
		tmpp = ememory::makeShared<ege::physics::shape::ConvexHull>();
	} else if (name == "autoconcave") {
		tmpp = ememory::makeShared<ege::physics::shape::Concave>();
	} else {
		EGE_ERROR("Create an unknow element : '" << _name << "' availlable : [BOX,SPHERE,CONE,CYLINDER,CAPSULE,CONVEXHULL,autoConcave]");
		return nullptr;
	}
	if (tmpp == nullptr) {
		EGE_ERROR("Allocation error for physical element : '" << _name << "'");
	}
	return tmpp;
}


bool ege::physics::Shape::parse(const char* _line) {
	if(strncmp(_line, "origin:", 7) == 0) {
		sscanf(&_line[7], "%f %f %f", &m_origin.m_floats[0], &m_origin.m_floats[1], &m_origin.m_floats[2] );
		EGE_VERBOSE("                Origin=" << m_origin);
		return true;
	}
	if(strncmp(_line, "rotate:", 7) == 0) {
		sscanf(&_line[7], "%f %f %f %f", &m_quaternion.m_floats[0], &m_quaternion.m_floats[1], &m_quaternion.m_floats[2], &m_quaternion.m_floats[3] );
		EGE_VERBOSE("                rotate=" << m_quaternion);
		return true;
	}
	if(strncmp(_line, "mass:", 5) == 0) {
		sscanf(&_line[5], "%f", &m_mass );
		EGE_VERBOSE("                mass=" << m_mass);
		return true;
	}
	return false;
}

