/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/ia/Component.hpp>

const etk::String& ege::ia::Component::getType() const {
	static etk::String tmp("ia");
	return tmp;
}

void ege::ia::Component::update(float _delta) {
	
}
