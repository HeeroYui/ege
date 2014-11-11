/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#include <ege/Ray.h>

#undef __class__
#define __class__ "Ray"

ege::Ray::Ray(const vec3& _origin, const vec3& _direction) :
  m_origin(_origin),
  m_direction(_direction) {
	m_direction.safeNormalize();
}

void ege::Ray::setOrigin(const vec3& _origin) {
	m_origin = _origin;
}

void ege::Ray::setDirection(const vec3& _direction) {
	m_direction = _direction;
	m_direction.safeNormalize();
}
