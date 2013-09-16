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
#define __class__	"ParticuleEngine"

ege::ParticuleEngine::ParticuleEngine(ege::Environement& _env) :
	m_env(_env)
{
	
}

ege::ParticuleEngine::~ParticuleEngine(void)
{
	Clear();
}

void ege::ParticuleEngine::Add(Particule* _particule)
{
	if (_particule==NULL) {
		EGE_ERROR("Try to add particule NULL");
		return;
	}
	for (esize_t iii=0; iii<m_particuleList.Size(); ++iii) {
		if (m_particuleList[iii] != NULL) {
			continue;
		}
		m_particuleList[iii] = _particule;
		return;
	}
	// Just add it at the end ...
	m_particuleList.PushBack(_particule);
}

void ege::ParticuleEngine::AddRemoved(Particule* _particule)
{
	if (_particule==NULL) {
		return;
	}
	for (esize_t iii=0; iii<m_particuleRemoved.Size(); ++iii) {
		if (m_particuleRemoved[iii] != NULL) {
			continue;
		}
		m_particuleRemoved[iii] = _particule;
		return;
	}
	// Just add it at the end ...
	m_particuleRemoved.PushBack(_particule);
}

ege::Particule* ege::ParticuleEngine::Respown(const char* _particuleType)
{
	if (_particuleType == NULL) {
		return NULL;
	}
	for (esize_t iii=0; iii<m_particuleRemoved.Size(); ++iii) {
		if (m_particuleRemoved[iii] == NULL) {
			continue;
		}
		if (m_particuleRemoved[iii]->GetParticuleType()==_particuleType) {
			Add(m_particuleRemoved[iii]);
			ege::Particule* tmpParticule = m_particuleRemoved[iii];
			m_particuleRemoved[iii]=NULL;
			tmpParticule->Init();
			return tmpParticule;
		}
	}
	return NULL;
}

void ege::ParticuleEngine::Update(float _deltaTime)
{
	if (_deltaTime>(1.0f/60.0f)) {
		_deltaTime = (1.0f/60.0f);
	}
	for (esize_t iii=0; iii<m_particuleList.Size(); ++iii) {
		if (m_particuleList[iii] == NULL) {
			continue;
		}
		m_particuleList[iii]->Update(_deltaTime);
	}
	// check removing elements
	for (esize_t iii=0; iii<m_particuleList.Size(); ++iii) {
		if (m_particuleList[iii] == NULL) {
			continue;
		}
		if (m_particuleList[iii]->NeedRemove()) {
			m_particuleList[iii]->OnEnd();
			if (m_particuleList[iii]->GetParticuleType()==NULL) {
				// Real remove particule ...
				delete (m_particuleList[iii]);
			} else {
				AddRemoved(m_particuleList[iii]);
			}
			m_particuleList[iii] = NULL;
		}
	}
	/*
	int32_t nbParticule = 0;
	for (esize_t iii=0; iii<m_particuleList.Size(); ++iii) {
		if (m_particuleList[iii] == NULL) {
			continue;
		}
		nbParticule++;
	}
	EGE_DEBUG("number of particule : " << nbParticule);
	*/
}

void ege::ParticuleEngine::Draw(const ege::Camera& _camera)
{
	for (esize_t iii=0; iii<m_particuleList.Size(); ++iii) {
		if (m_particuleList[iii] == NULL) {
			continue;
		}
		m_particuleList[iii]->Draw(_camera);
	}
}

void ege::ParticuleEngine::Clear(void)
{
	// clear element not removed
	for (esize_t iii=0; iii<m_particuleList.Size(); ++iii) {
		if (m_particuleList[iii] == NULL) {
			continue;
		}
		delete m_particuleList[iii];
		m_particuleList[iii] = NULL;
	}
	m_particuleList.Clear();
	// clear element that are auto-removed
	for (esize_t iii=0; iii<m_particuleRemoved.Size(); ++iii) {
		if (m_particuleRemoved[iii] == NULL) {
			continue;
		}
		delete m_particuleRemoved[iii];
		m_particuleRemoved[iii] = NULL;
	}
	m_particuleRemoved.Clear();
}
