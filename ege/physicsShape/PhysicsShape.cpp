/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#include <ege/debug.h>
#include <ege/physicsShape/PhysicsShape.h>
#include <ege/physicsShape/PhysicsBox.h>
#include <ege/physicsShape/PhysicsCapsule.h>
#include <ege/physicsShape/PhysicsCone.h>
#include <ege/physicsShape/PhysicsConvexHull.h>
#include <ege/physicsShape/PhysicsCylinder.h>
#include <ege/physicsShape/PhysicsSphere.h>


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

