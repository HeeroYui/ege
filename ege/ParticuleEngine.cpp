/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/ParticuleEngine.h>

ege::ParticuleEngine::ParticuleEngine(ege::Environement& _env) :
	m_env(_env)
{
	
}

ege::ParticuleEngine::~ParticuleEngine(void)
{
	
}

void ege::ParticuleEngine::Add(Particule* _particule, const char* _particuleType)
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
	m_particuleList.PushBack();
}

void ege::ParticuleEngine::Update(float _deltaTime)
{
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
			delete m_particuleList[iii];
			m_particuleList[iii] = NULL;
		}
	}
}

void ege::ParticuleEngine::Draw(void)
{
	for (esize_t iii=0; iii<m_particuleList.Size(); ++iii) {
		if (m_particuleList[iii] == NULL) {
			continue;
		}
		m_particuleList[iii]->Draw();
	}
}

void ege::ParticuleEngine::Clear(void)
{
	for (esize_t iii=0; iii<m_particuleList.Size(); ++iii) {
		if (m_particuleList[iii] == NULL) {
			continue;
		}
		delete m_particuleList[iii];
		m_particuleList[iii] = NULL;
	}
	m_particuleList.Clear();
}
