/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#include <etk/types.h>
#include <ege/Ray.h>
#include <ege/debug.h>

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



std::ostream& ege::operator <<(std::ostream& _os, const ege::Ray& _obj) {
	_os << "{ori=";
	_os << _obj.getOrigin();
	_os << " dir=";
	_os << _obj.getDirection();
	_os << "}";
	return _os;
}