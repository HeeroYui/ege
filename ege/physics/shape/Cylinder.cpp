/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/debug.hpp>
#include <ege/physics/shape/Cylinder.hpp>


bool ege::physics::shape::Cylinder::parse(const char* _line) {
	if (ege::physics::Shape::parse(_line) == true) {
		return true;
	}
	if(strncmp(_line, "half-extents:", 13) == 0) {
		sscanf(&_line[13], "%f %f %f", &m_size.m_floats[0], &m_size.m_floats[1], &m_size.m_floats[2] );
		EGE_VERBOSE("                halfSize=" << m_size);
		return true;
	}
	return false;
}
