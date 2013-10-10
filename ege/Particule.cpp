/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/Particule.h>

#undef __class__
#define __class__	"Particule"

ege::Particule::Particule(ege::ParticuleEngine& _particuleEngine, const char* _particuleType) :
  m_particuleEngine(_particuleEngine),
  m_particuleType(_particuleType) {
	m_particuleEngine.add(this);
}

