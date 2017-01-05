/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/debug.hpp>
#include <ege/physicsShape/PhysicsShape.hpp>
#include <ege/physicsShape/PhysicsBox.hpp>
#include <ege/physicsShape/PhysicsCapsule.hpp>
#include <ege/physicsShape/PhysicsCone.hpp>
#include <ege/physicsShape/PhysicsConvexHull.hpp>
#include <ege/physicsShape/PhysicsCylinder.hpp>
#include <ege/physicsShape/PhysicsSphere.hpp>


ememory::SharedPtr<ege::PhysicsShape> ege::PhysicsShape::create(const std::string& _name) {
	ememory::SharedPtr<ege::PhysicsShape> tmpp = nullptr;
	std::string name = etk::tolower(_name);
	if (name == "box") {
		tmpp = ememory::makeShared<ege::PhysicsBox>();
	} else if (name == "sphere") {
		tmpp = ememory::makeShared<ege::PhysicsSphere>();
	} else if (name == "cone") {
		tmpp = ememory::makeShared<ege::PhysicsCone>();
	} else if (name == "cylinder") {
		tmpp = ememory::makeShared<ege::PhysicsCylinder>();
	} else if (name == "capsule") {
		tmpp = ememory::makeShared<ege::PhysicsCapsule>();
	} else if (name == "convexhull") {
		tmpp = ememory::makeShared<ege::PhysicsConvexHull>();
	} else {
		EGE_ERROR("Create an unknow element : '" << _name << "' availlable : [BOX,SPHERE,CONE,CYLINDER,CAPSULE,CONVEXHULL]");
		return nullptr;
	}
	if (tmpp == nullptr) {
		EGE_ERROR("Allocation error for physical element : '" << _name << "'");
	}
	return tmpp;
}


bool ege::PhysicsShape::parse(const char* _line) {
	if(strncmp(_line, "origin : ", 9) == 0) {
		sscanf(&_line[9], "%f %f %f", &m_origin.m_floats[0], &m_origin.m_floats[1], &m_origin.m_floats[2] );
		EGE_VERBOSE("                Origin=" << m_origin);
		return true;
	}
	if(strncmp(_line, "rotate : ", 9) == 0) {
		sscanf(&_line[9], "%f %f %f %f", &m_quaternion.m_floats[0], &m_quaternion.m_floats[1], &m_quaternion.m_floats[2], &m_quaternion.m_floats[3] );
		EGE_VERBOSE("                rotate=" << m_quaternion);
		return true;
	}
	return false;
}

