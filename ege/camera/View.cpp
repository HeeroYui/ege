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
		tetha -= M_PI;
	}
	m_matrix.translate(vec3(0,0,-distance));
	m_matrix.rotate(vec3(1,0,0), -M_PI*0.5f + psy);
	m_matrix.rotate(vec3(0,0,1), -tetha-M_PI/2.0f);
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
	return m_target - m_eye;
}


ege::Ray ege::camera::View::getRayFromScreen(const vec2& _offset) {
	vec2 cameraAngleOffset(m_angleView/2*_offset.x(), 2*_offset.y()*m_aspectRatio/m_angleView);
	#if 0
		mat4 inverse = m_matrix.invert();
		vec3 screenOffset(0,0,-1);
		screenOffset.rotate(vec3(1,0,0), cameraAngleOffset.x());
		screenOffset.rotate(vec3(0,1,0), cameraAngleOffset.y());
		vec3 direction = inverse*screenOffset;
	#else
		vec3 direction = getViewVector();
		//mat4 transformation;
		//transformation.indentity();
		//transformation.rotate(vec3(1,0,0), cameraAngleOffset.x());
		direction.rotate(vec3(1,0,0), cameraAngleOffset.x());
		direction.rotate(vec3(0,1,0), cameraAngleOffset.y());
	#endif
	direction.safeNormalize();
	ege::Ray out(m_eye, direction);
	EGE_WARNING("request ray from : " << _offset);
	EGE_WARNING("    camera offset = " << cameraAngleOffset);
	
	EGE_WARNING("return ray : " << out);
	// TODO : Use offset...
	return out;
}
