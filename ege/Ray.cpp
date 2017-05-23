/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <etk/types.hpp>
#include <ege/Ray.hpp>
#include <ege/debug.hpp>
#include <ege/Entity.hpp>
#include <etk/math/Vector3D.hpp>


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


vec3 ege::Ray::testRayZeroPlane() {
	float coef = m_origin.z() / m_direction.z();
	EGE_INFO("RAY : " << *this << " coef=" << coef);
	return m_origin - coef*m_direction;
}

