/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/Component.hpp>

const std::string& ege::Component::getType() const {
	static std::string tmp("component");
	return tmp;
}

