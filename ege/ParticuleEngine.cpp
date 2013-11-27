/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/ParticuleEngine.h>

#undef __class__
#define __class__ "ParticuleEngine"

ege::ParticuleEngine::ParticuleEngine(ege::Environement& _env) :
  m_env(_env) {
	
}

ege::ParticuleEngine::~ParticuleEngine(void) {
	clear();
}

void ege::ParticuleEngine::add(Particule* _particule) {
	if (_particule == NULL) {
		EGE_ERROR("Try to add particule NULL");
		return;
	}
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] != NULL) {
			continue;
		}
		m_particuleList[iii] = _particule;
		return;
	}
	// Just add it at the end ...
	m_particuleList.push_back(_particule);
}

void ege::ParticuleEngine::addRemoved(Particule* _particule) {
	if (_particule == NULL) {
		return;
	}
	for (size_t iii=0; iii<m_particuleRemoved.size(); ++iii) {
		if (m_particuleRemoved[iii] != NULL) {
			continue;
		}
		m_particuleRemoved[iii] = _particule;
		return;
	}
	// Just add it at the end ...
	m_particuleRemoved.push_back(_particule);
}

ege::Particule* ege::ParticuleEngine::respown(const char* _particuleType) {
	if (_particuleType == NULL) {
		return NULL;
	}
	for (size_t iii=0; iii<m_particuleRemoved.size(); ++iii) {
		if (m_particuleRemoved[iii] == NULL) {
			continue;
		}
		if (m_particuleRemoved[iii]->getParticuleType() == _particuleType) {
			add(m_particuleRemoved[iii]);
			ege::Particule* tmpParticule = m_particuleRemoved[iii];
			m_particuleRemoved[iii]=NULL;
			tmpParticule->init();
			return tmpParticule;
		}
	}
	return NULL;
}

void ege::ParticuleEngine::update(float _deltaTime) {
	if (_deltaTime>(1.0f/60.0f)) {
		_deltaTime = (1.0f/60.0f);
	}
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == NULL) {
			continue;
		}
		m_particuleList[iii]->update(_deltaTime);
	}
	// check removing elements
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == NULL) {
			continue;
		}
		if (m_particuleList[iii]->needRemove()) {
			m_particuleList[iii]->onEnd();
			if (m_particuleList[iii]->getParticuleType() == NULL) {
				// Real remove particule ...
				delete (m_particuleList[iii]);
			} else {
				addRemoved(m_particuleList[iii]);
			}
			m_particuleList[iii] = NULL;
		}
	}
	/*
	int32_t nbParticule = 0;
	for (int32_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == NULL) {
			continue;
		}
		nbParticule++;
	}
	EGE_DEBUG("number of particule : " << nbParticule);
	*/
}

void ege::ParticuleEngine::draw(const ege::Camera& _camera) {
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == NULL) {
			continue;
		}
		m_particuleList[iii]->draw(_camera);
	}
}

void ege::ParticuleEngine::clear(void) {
	// clear element not removed
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == NULL) {
			continue;
		}
		delete m_particuleList[iii];
		m_particuleList[iii] = NULL;
	}
	m_particuleList.clear();
	// clear element that are auto-removed
	for (size_t iii=0; iii<m_particuleRemoved.size(); ++iii) {
		if (m_particuleRemoved[iii] == NULL) {
			continue;
		}
		delete m_particuleRemoved[iii];
		m_particuleRemoved[iii] = NULL;
	}
	m_particuleRemoved.clear();
}
