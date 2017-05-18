/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/Engine.hpp>
#include <ege/Environement.hpp>

ege::Engine::Engine(ege::Environement* _env) :
  m_env(_env) {
	
}

const std::string& ege::Engine::getType() const {
	static std::string tmp("engine");
	return tmp;
}

void ege::Engine::componentRemove(const ememory::SharedPtr<ege::Component>& _ref) {
	
}

void ege::Engine::componentAdd(const ememory::SharedPtr<ege::Component>& _ref) {
	
}

void ege::Engine::update(const echrono::Duration& _delta) {
	
}

void ege::Engine::render(const echrono::Duration& _delta, const ememory::SharedPtr<ege::Camera>& _camera) {
	
}