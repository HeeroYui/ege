/**
 * @author Edouard DUPIN
 * 
 * @copyright 2013, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ege/camera/View.h>
#include <ege/debug.h>
#include <etk/math/Vector3D.h>

#undef __class__
#define __class__ "camera::View"

void ege::camera::View::update() {
	//m_matrix = etk::matLookAt(m_eye, m_target, m_up);
	//m_matrix.translate(m_eye);
	m_matrix.identity();
	m_matrix.rotate(vec3(0,0,1), -m_angle);
	vec3 pos = m_eye-m_target;
	float distance = pos.length();
	float psy = std::asin(pos.z()/distance);
	pos.setZ(0.0f);
	pos.normalize();
	float tetha = std::asin(pos.y());
	if (pos.x() < 0) {
		tetha *= -1;
		tetha += M_PI;
	}
	m_matrix.translate(vec3(0,0,-distance));
	m_matrix.rotate(vec3(1,0,0), -M_PI*0.5f + psy);
	m_matrix.rotate(vec3(0,0,1), tetha);
	m_matrix.translate(-m_target);
	
	EGE_DEBUG("Camera properties : distance=" << distance );
	EGE_DEBUG("                         psy=" << psy);
	EGE_DEBUG("                       Tetha=" << tetha);
	EGE_DEBUG("                       m_eye=" << etk::to_string(m_eye));
}

ege::camera::View::View(const vec3& _eye, const vec3& _target, float _angle) :
  m_eye(_eye),
  m_target(_target),
  m_angle(_angle) {
	update();
}

void ege::camera::View::setEye(const vec3& _eye) {
	m_eye = _eye;
	update();
}

void ege::camera::View::setTarget(const vec3& _target) {
	m_target = _target;
	update();
}

void ege::camera::View::setAngle(float _angle) {
	m_angle = _angle;
	update();
}

vec3 ege::camera::View::getViewVector() const {
	return m_eye-m_target;
}

