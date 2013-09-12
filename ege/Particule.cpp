/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/Particule.h>

ege::Particule::Particule(ege::ParticuleEngine& _particuleEngine) :
	m_particuleEngine(_particuleEngine)
{
	m_particuleEngine.Add(this);
}

