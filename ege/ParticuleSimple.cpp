/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/ParticuleSimple.h>


#undef __class__
#define __class__	"ParticuleSimple"

ege::ParticuleSimple::ParticuleSimple(ege::ParticuleEngine& _particuleEngine, const char* _particuleType) :
	Particule(_particuleEngine, _particuleType)
{
	Init();
}


void ege::ParticuleSimple::Init(void)
{
	m_lifeFull = 3;
	m_life = m_lifeFull;
	m_level = 0;
	m_pos = vec3(0,0,0);
	m_angle = 0;
	m_speed = vec3(0,0,0);
	m_scale = vec3(1,1,1);
	m_scaleExpand = vec3(0,0,0);
}

bool ege::ParticuleSimple::NeedRemove(void)
{
	return m_life<0.0f;
}


void ege::ParticuleSimple::Update(float _delta)
{
	//EGE_DEBUG("Life : " << m_life << "-" << _delta);
	m_life -= _delta;
	m_pos += m_speed*_delta;
	m_scale += m_scaleExpand*_delta;
}

void ege::ParticuleSimple::SetLife(float _life)
{
	m_lifeFull = _life;
	m_life = m_lifeFull;
}

void ege::ParticuleSimple::SetLevel(float _level)
{
	m_level = _level;
}

void ege::ParticuleSimple::SetPosition(const vec3& _pos)
{
	m_pos = _pos;
}

void ege::ParticuleSimple::SetAngle(float _angle)
{
	m_angle = _angle;
}

void ege::ParticuleSimple::SetMoveSpeed(const vec3& _speed)
{
	m_speed = _speed;
}

void ege::ParticuleSimple::SetScale(const vec3& _scale)
{
	m_scale = _scale;
}

void ege::ParticuleSimple::SetScaleExpend(const vec3& _scaleExpand)
{
	m_scaleExpand=_scaleExpand;
}
