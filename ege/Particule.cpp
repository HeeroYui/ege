/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/Particule.hpp>
#include <ege/ParticuleEngine.hpp>

ege::Particule::Particule(ege::ParticuleEngine* _particuleEngine, const char* _particuleType) :
  m_particuleEngine(_particuleEngine),
  m_particuleType(_particuleType) {
	m_particuleEngine->add(this);
}

