/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/ParticuleSimple.h>



ege::ParticuleSimple::ParticuleSimple(ege::ParticuleEngine& _particuleEngine, const char* _particuleType) :
	Particule(_particuleEngine, _particuleType)
{
	Init();
}


void ege::ParticuleSimple::Init(void)
{
	m_life = 0;
	m_level = 0;
	m_pos = vec3(0,0,0);
	m_angle = vec4(0,0,0,0);
	m_speed = vec3(0,0,0);
}

bool ege::ParticuleSimple::NeedRemove(void)
{
	return m_life<0.0f;
}


void ege::ParticuleSimple::Update(float _delta)
{
	m_life -= _delta;
	m_pos += m_speed;
}

void ege::ParticuleSimple::SetLife(float _life)
{
	m_life = _life;
}

void ege::ParticuleSimple::SetLevel(float _level)
{
	m_level = _level;
}

void ege::ParticuleSimple::SetPosition(const vec3& _pos)
{
	m_pos = _pos;
}

void ege::ParticuleSimple::SetAngleSpeed(const vec4& _angle)
{
	m_angle = _angle;
}

void ege::ParticuleSimple::SetMoveSpeed(const vec3& _speed)
{
	m_speed = _speed;
}

