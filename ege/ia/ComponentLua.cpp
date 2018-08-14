/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/ia/ComponentLua.hpp>

ege::ia::ComponentLua::ComponentLua(const etk::String& _fileName) {
	// Load the current IA file interface ==> init...
	m_engine.executeFile(_fileName);
}


void ege::ia::ComponentLua::update(float _delta) {
	m_engine.callVoid("update", float(_delta));
}
