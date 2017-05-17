/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/ia/Component.hpp>

const std::string& ege::ia::Component::getType() const {
	static std::string tmp("ia");
	return tmp;
}

