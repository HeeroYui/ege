/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/Component.hpp>

const etk::String& ege::Component::getType() const {
	static etk::String tmp("component");
	return tmp;
}

