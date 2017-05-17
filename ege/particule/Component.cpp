/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/particule/Component.hpp>
#include <ege/debug.hpp>
#include <ege/particule/Engine.hpp>

const std::string& ege::particule::Component::getType() const {
	static std::string tmp("particule");
	return tmp;
}

ege::particule::Component::Component(ege::particule::Engine* _particuleEngine, const char* _particuleType) :
  m_particuleEngine(_particuleEngine),
  m_particuleType(_particuleType) {
	m_particuleEngine->add(ememory::staticPointerCast<ege::particule::Component>(sharedFromThis()));
}

