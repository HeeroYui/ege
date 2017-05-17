/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/particule/Engine.hpp>
#include <ege/Environement.hpp>
#include <ege/particule/Particule.hpp>

ege::particule::Engine::Engine(ege::Environement* _env) :
  m_env(_env) {
	
}

ege::particule::Engine::~Engine() {
	clear();
}

void ege::particule::Engine::add(const ememory::SharedPtr<ege::particule::Component>& _particule) {
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

void ege::particule::Engine::addRemoved(const ememory::SharedPtr<ege::particule::Component>& _particule) {
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

ememory::SharedPtr<ege::particule::Component> ege::particule::Engine::respown(const char* _particuleType) {
	if (_particuleType == nullptr) {
		return nullptr;
	}
	for (size_t iii=0; iii<m_particuleRemoved.size(); ++iii) {
		if (m_particuleRemoved[iii] == nullptr) {
			continue;
		}
		if (m_particuleRemoved[iii]->getParticuleType() == _particuleType) {
			add(m_particuleRemoved[iii]);
			ememory::SharedPtr<ege::particule::Component> tmpParticule = m_particuleRemoved[iii];
			m_particuleRemoved[iii].reset();
			tmpParticule->init();
			return tmpParticule;
		}
	}
	return nullptr;
}

void ege::particule::Engine::update(float _deltaTime) {
	if (_deltaTime>(1.0f/60.0f)) {
		_deltaTime = (1.0f/60.0f);
	}
	EGE_WARNING("Update the Particule engine ... " << _deltaTime);
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
				m_particuleList[iii].reset();
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

void ege::particule::Engine::draw(const ege::Camera& _camera) {
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == nullptr) {
			continue;
		}
		m_particuleList[iii]->draw(_camera);
	}
}

void ege::particule::Engine::clear() {
	// clear element not removed
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == nullptr) {
			continue;
		}
		m_particuleList[iii].reset();
	}
	m_particuleList.clear();
	// clear element that are auto-removed
	for (size_t iii=0; iii<m_particuleRemoved.size(); ++iii) {
		if (m_particuleRemoved[iii] == nullptr) {
			continue;
		}
		m_particuleRemoved[iii].reset();
	}
	m_particuleRemoved.clear();
}
