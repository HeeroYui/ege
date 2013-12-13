/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#include <ege/debug.h>
#include <ege/physicsShape/PhysicsCapsule.h>



bool ege::PhysicsCapsule::parse(const char* _line) {
	if (true == ege::PhysicsShape::parse(_line)) {
		return true;
	}
	if(0 == strncmp(_line, "radius : ", 9) ) {
		sscanf(&_line[9], "%f", &m_radius );
		EGE_VERBOSE("                radius=" << m_radius);
		return true;
	}
	if(0 == strncmp(_line, "height : ", 9) ) {
		sscanf(&_line[9], "%f", &m_height );
		EGE_VERBOSE("                height=" << m_height);
		return true;
	}
	return false;
}
