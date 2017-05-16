/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <etk/types.hpp>
#include <ege/Ray.hpp>
#include <ege/debug.hpp>
#include <ege/elements/Element.hpp>


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

std::pair<vec3,vec3> ege::Ray::testRay(ege::physics::Engine& _engine) {
	vec3 start = m_origin;
	vec3 stop = m_origin+m_direction*1000.0f;
	// Start and End are vectors
	/*
	btCollisionWorld::ClosestRayResultCallback rayCallback(start, stop);
	EGE_VERBOSE("Process Raycast :");
	// Perform raycast
	_engine.getDynamicWorld()->rayTest(start, stop, rayCallback);
	if(rayCallback.hasHit()) {
		vec3 end = rayCallback.m_hitPointWorld;
		vec3 normal = rayCallback.m_hitNormalWorld;
		EGE_VERBOSE("    hit at point=" << end << " normal=" << normal);
		return std::pair<vec3,vec3>(end,normal);
	}
	*/
	EGE_VERBOSE("    No Hit");
	return std::pair<vec3,vec3>(vec3(0,0,0),vec3(0,0,0));
}


std::pair<ememory::SharedPtr<ege::Element>, std::pair<vec3,vec3>> ege::Ray::testRayObject(ege::physics::Engine& _engine) {
	vec3 start = m_origin;
	vec3 stop = m_origin+m_direction*1000.0f;
	// Start and End are vectors
	/*
	btCollisionWorld::ClosestRayResultCallback rayCallback(start, stop);
	EGE_VERBOSE("Process Raycast :");
	// Perform raycast
	_engine.getDynamicWorld()->rayTest(start, stop, rayCallback);
	if(rayCallback.hasHit()) {
		vec3 end = rayCallback.m_hitPointWorld;
		vec3 normal = rayCallback.m_hitNormalWorld;
		ege::Element* elem = static_cast<ege::Element*>(rayCallback.m_collisionObject->getUserPointer());
		if (elem != nullptr) {
			EGE_VERBOSE("    hit at point=" << end << " normal=" << normal);
			return std::pair<ememory::SharedPtr<ege::Element>, std::pair<vec3,vec3>>(elem->sharedFromThis(), std::pair<vec3,vec3>(end,normal));
		}
		EGE_VERBOSE("    Can not get the element pointer");
		return std::pair<ememory::SharedPtr<ege::Element>, std::pair<vec3,vec3>>(nullptr, std::pair<vec3,vec3>(end,normal));
	}
	*/
	EGE_VERBOSE("    No Hit");
	return std::pair<ememory::SharedPtr<ege::Element>, std::pair<vec3,vec3>>(nullptr, std::pair<vec3,vec3>(vec3(0,0,0),vec3(0,0,0)));
}

vec3 ege::Ray::testRayZeroPlane() {
	float coef = m_origin.z() / m_direction.z();
	EGE_INFO("RAY : " << *this << " coef=" << coef);
	return m_origin - coef*m_direction;
}

