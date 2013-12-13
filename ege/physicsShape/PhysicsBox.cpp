/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#include <ege/debug.h>
#include <ege/physicsShape/PhysicsBox.h>



bool ege::PhysicsBox::parse(const char* _line) {
	if (true == ege::PhysicsShape::parse(_line)) {
		return true;
	}
	if(0 == strncmp(_line, "half-extents : ", 15) ) {
		sscanf(&_line[15], "%f %f %f", &m_size.m_floats[0], &m_size.m_floats[1], &m_size.m_floats[2] );
		EGE_VERBOSE("                halfSize=" << m_size);
		return true;
	}
	return false;
}
