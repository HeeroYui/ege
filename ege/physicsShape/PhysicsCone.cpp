/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/debug.hpp>
#include <ege/physicsShape/PhysicsCone.hpp>



bool ege::PhysicsCone::parse(const char* _line) {
	if (ege::PhysicsShape::parse(_line) == true) {
		return true;
	}
	if(strncmp(_line, "radius:", 7) == 0) {
		sscanf(&_line[7], "%f", &m_radius );
		EGE_VERBOSE("                radius=" << m_radius);
		return true;
	}
	if(strncmp(_line, "height:", 7) == 0) {
		sscanf(&_line[7], "%f", &m_height );
		EGE_VERBOSE("                height=" << m_height);
		return true;
	}
	return false;
}
