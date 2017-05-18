/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/ia/Engine.hpp>

ege::ia::Engine::Engine(ege::Environement* _env) :
  ege::Engine(_env) {
	
}

const std::string& ege::ia::Engine::getType() const {
	static std::string tmp("ia");
	return tmp;
}

void ege::ia::Engine::componentRemove(const ememory::SharedPtr<ege::Component>& _ref) {
	
}

void ege::ia::Engine::componentAdd(const ememory::SharedPtr<ege::Component>& _ref) {
	
}
