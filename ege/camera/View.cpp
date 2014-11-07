/**
 * @author Edouard DUPIN
 * 
 * @copyright 2013, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ege/camera/View.h>
#include <ege/debug.h>

#undef __class__
#define __class__ "camera::View"

void ege::camera::View::update() {
	m_matrix = etk::matLookAt(m_eye, m_target, m_up);
	//m_matrix.translate(m_eye);
}

ege::camera::View::View(const vec3& _eye, const vec3& _target, const vec3& _up) :
  m_eye(_eye),
  m_target(_target),
  m_up(_up) {
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

void ege::camera::View::setUp(const vec3& _up) {
	m_up = _up;
	update();
}

vec3 ege::camera::View::getViewVector() const {
	return m_eye-m_target;
}

