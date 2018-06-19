/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/particule/Engine.hpp>
#include <ege/Environement.hpp>
#include <ege/particule/Component.hpp>

ege::particule::Engine::Engine(ege::Environement* _env) :
  ege::Engine(_env) {
	
}

ege::particule::Engine::~Engine() {
	clear();
}

const etk::String& ege::particule::Engine::getType() const {
	static etk::String tmp("particule");
	return tmp;
}

void ege::particule::Engine::componentRemove(const ememory::SharedPtr<ege::Component>& _ref) {
	
}

void ege::particule::Engine::componentAdd(const ememory::SharedPtr<ege::Component>& _ref) {
	
}



void ege::particule::Engine::add(const ememory::SharedPtr<ege::particule::Component>& _particule) {
	if (_particule == null) {
		EGE_ERROR("Try to add particule null");
		return;
	}
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] != null) {
			continue;
		}
		m_particuleList[iii] = _particule;
		return;
	}
	// Just add it at the end ...
	m_particuleList.pushBack(_particule);
}

void ege::particule::Engine::addRemoved(const ememory::SharedPtr<ege::particule::Component>& _particule) {
	if (_particule == null) {
		return;
	}
	for (size_t iii=0; iii<m_particuleRemoved.size(); ++iii) {
		if (m_particuleRemoved[iii] != null) {
			continue;
		}
		m_particuleRemoved[iii] = _particule;
		return;
	}
	// Just add it at the end ...
	m_particuleRemoved.pushBack(_particule);
}

ememory::SharedPtr<ege::particule::Component> ege::particule::Engine::respown(const char* _particuleType) {
	if (_particuleType == null) {
		return null;
	}
	for (size_t iii=0; iii<m_particuleRemoved.size(); ++iii) {
		if (m_particuleRemoved[iii] == null) {
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
	return null;
}

void ege::particule::Engine::update(const echrono::Duration& _delta) {
	float deltaTime = _delta.toSeconds();
	if (deltaTime>(1.0f/60.0f)) {
		deltaTime = (1.0f/60.0f);
	}
	EGE_DEBUG("Update the Particule engine ... " << deltaTime);
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == null) {
			continue;
		}
		m_particuleList[iii]->update(deltaTime);
	}
	// check removing elements
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == null) {
			continue;
		}
		if (m_particuleList[iii]->needRemove()) {
			m_particuleList[iii]->onEnd();
			if (m_particuleList[iii]->getParticuleType() == null) {
				// Real remove particule ...
				m_particuleList[iii].reset();
			} else {
				addRemoved(m_particuleList[iii]);
			}
			m_particuleList[iii] = null;
		}
	}
	/*
	int32_t nbParticule = 0;
	for (int32_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == null) {
			continue;
		}
		nbParticule++;
	}
	EGE_DEBUG("number of particule : " << nbParticule);
	*/
}

void ege::particule::Engine::render(const echrono::Duration& _delta, const ememory::SharedPtr<ege::Camera>& _camera) {
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == null) {
			continue;
		}
		m_particuleList[iii]->draw(*_camera);
	}
}

void ege::particule::Engine::clear() {
	// clear element not removed
	for (size_t iii=0; iii<m_particuleList.size(); ++iii) {
		if (m_particuleList[iii] == null) {
			continue;
		}
		m_particuleList[iii].reset();
	}
	m_particuleList.clear();
	// clear element that are auto-removed
	for (size_t iii=0; iii<m_particuleRemoved.size(); ++iii) {
		if (m_particuleRemoved[iii] == null) {
			continue;
		}
		m_particuleRemoved[iii].reset();
	}
	m_particuleRemoved.clear();
}
