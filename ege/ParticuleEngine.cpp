/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/ParticuleEngine.hpp>
#include <ege/Environement.hpp>
#include <ege/Particule.hpp>

ege::ParticuleEngine::ParticuleEngine(ege::Environement* _env) :
  m_env(_env) {
	
}

ege::ParticuleEngine::~ParticuleEngine() {
	clear();
}

void ege::ParticuleEngine::add(Particule* _particule) {
	if (_particule == nullptr) {
		EGE_ERROR("Try to add particule nullptr");
		return;
	}
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] != nullptr) {
			continue;
		}
		m_particuleList[iii] = _particule;
		return;
	}
	// Just add it at the end ...
	m_particuleList.push_back(_particule);
}

void ege::ParticuleEngine::addRemoved(Particule* _particule) {
	if (_particule == nullptr) {
		return;
	}
	for (size_t iii=0; iii<m_particuleRemoved.size(); ++iii) {
		if (m_particuleRemoved[iii] != nullptr) {
			continue;
		}
		m_particuleRemoved[iii] = _particule;
		return;
	}
	// Just add it at the end ...
	m_particuleRemoved.push_back(_particule);
}

ege::Particule* ege::ParticuleEngine::respown(const char* _particuleType) {
	if (_particuleType == nullptr) {
		return nullptr;
	}
	for (size_t iii=0; iii<m_particuleRemoved.size(); ++iii) {
		if (m_particuleRemoved[iii] == nullptr) {
			continue;
		}
		if (m_particuleRemoved[iii]->getParticuleType() == _particuleType) {
			add(m_particuleRemoved[iii]);
			ege::Particule* tmpParticule = m_particuleRemoved[iii];
			m_particuleRemoved[iii]=nullptr;
			tmpParticule->init();
			return tmpParticule;
		}
	}
	return nullptr;
}

void ege::ParticuleEngine::update(float _deltaTime) {
	if (_deltaTime>(1.0f/60.0f)) {
		_deltaTime = (1.0f/60.0f);
	}
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == nullptr) {
			continue;
		}
		m_particuleList[iii]->update(_deltaTime);
	}
	// check removing elements
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == nullptr) {
			continue;
		}
		if (m_particuleList[iii]->needRemove()) {
			m_particuleList[iii]->onEnd();
			if (m_particuleList[iii]->getParticuleType() == nullptr) {
				// Real remove particule ...
				delete (m_particuleList[iii]);
			} else {
				addRemoved(m_particuleList[iii]);
			}
			m_particuleList[iii] = nullptr;
		}
	}
	/*
	int32_t nbParticule = 0;
	for (int32_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == nullptr) {
			continue;
		}
		nbParticule++;
	}
	EGE_DEBUG("number of particule : " << nbParticule);
	*/
}

void ege::ParticuleEngine::draw(const ege::Camera& _camera) {
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == nullptr) {
			continue;
		}
		m_particuleList[iii]->draw(_camera);
	}
}

void ege::ParticuleEngine::clear() {
	// clear element not removed
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == nullptr) {
			continue;
		}
		delete m_particuleList[iii];
		m_particuleList[iii] = nullptr;
	}
	m_particuleList.clear();
	// clear element that are auto-removed
	for (size_t iii=0; iii<m_particuleRemoved.size(); ++iii) {
		if (m_particuleRemoved[iii] == nullptr) {
			continue;
		}
		delete m_particuleRemoved[iii];
		m_particuleRemoved[iii] = nullptr;
	}
	m_particuleRemoved.clear();
}
