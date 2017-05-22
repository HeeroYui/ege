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
	if(strncmp(_line, "radius:", 7) == 0) {
		sscanf(&_line[7], "%f", &m_radius );
		EGE_VERBOSE("                radius=" << m_radius);
		return true;
	}
	if(strncmp(_line, "size:", 5) == 0) {
		sscanf(&_line[5], "%f", &m_size );
		EGE_VERBOSE("                size=" << m_size);
		return true;
	}
	return false;
}
