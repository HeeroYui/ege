/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/debug.hpp>
#include <ege/physics/shape/Concave.hpp>



bool ege::physics::shape::Concave::parse(const char* _line) {
	if (ege::physics::Shape::parse(_line) == true) {
		return true;
	}
	return false;
}
