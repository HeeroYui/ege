/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/particule/Simple.hpp>

ege::particule::Simple::Simple(ege::particule::Engine* _particuleEngine, const char* _particuleType) :
  ege::particule::Component(_particuleEngine, _particuleType) {
	init();
}

void ege::particule::Simple::init() {
	m_lifeFull = 3;
	m_life = m_lifeFull;
	m_level = 0;
	m_pos = vec3(0,0,0);
	m_angle = 0;
	m_speed = vec3(0,0,0);
	m_scale = vec3(1,1,1);
	m_scaleExpand = vec3(0,0,0);
}

bool ege::particule::Simple::needRemove() {
	return m_life<0.0f;
}


void ege::particule::Simple::update(float _delta) {
	//EGE_DEBUG("Life : " << m_life << "-" << _delta);
	m_life -= _delta;
	m_pos += m_speed*_delta;
	m_scale += m_scaleExpand*_delta;
}

void ege::particule::Simple::setLife(float _life) {
	m_lifeFull = _life;
	m_life = m_lifeFull;
}

void ege::particule::Simple::setLevel(float _level) {
	m_level = _level;
}

void ege::particule::Simple::setPosition(const vec3& _pos) {
	m_pos = _pos;
}

void ege::particule::Simple::setAngle(float _angle) {
	m_angle = _angle;
}

void ege::particule::Simple::setMoveSpeed(const vec3& _speed) {
	m_speed = _speed;
}

void ege::particule::Simple::setScale(const vec3& _scale) {
	m_scale = _scale;
}

void ege::particule::Simple::setScaleExpend(const vec3& _scaleExpand) {
	m_scaleExpand=_scaleExpand;
}
