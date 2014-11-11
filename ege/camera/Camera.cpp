/**
 * @author Edouard DUPIN
 * 
 * @copyright 2013, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ege/camera/Camera.h>
#include <ege/debug.h>

#include <ewol/openGL/openGL.h>

#undef __class__
#define __class__ "Camera"

ege::Camera::Camera() :
  m_aspectRatio(0.5),
  m_angleView(M_PI/3.0),
  m_zFar(4000.0f),
  m_zNear(1.0f) {
	m_matrix.identity();
	updateProjectionMatrix();
}

void ege::Camera::setAspectRatio(float _ratio) {
	if (m_aspectRatio == _ratio) {
		return;
	}
	m_aspectRatio = _ratio;
	updateProjectionMatrix();
}

void ege::Camera::setXAngleView(float _angleRad) {
	if (m_angleView == _angleRad) {
		return;
	}
	m_angleView = _angleRad;
	updateProjectionMatrix();
}

void ege::Camera::setZFar(float _distance) {
	if (m_zFar == _distance) {
		return;
	}
	m_zFar = _distance;
	updateProjectionMatrix();
}

void ege::Camera::setZNear(float _distance) {
	if (m_zNear == _distance) {
		return;
	}
	m_zNear = _distance;
	updateProjectionMatrix();
}


void ege::Camera::updateProjectionMatrix() {
	m_matrixProjection = etk::matPerspective(m_angleView, m_aspectRatio, m_zNear, m_zFar);
}

void ege::Camera::configureOpenGL() {
	ewol::openGL::setCameraMatrix(getMatrixCamera());
	ewol::openGL::setMatrix(getMatrixProjection());
}


ege::Ray ege::Camera::getRayFromScreen(const vec2& _offset) {
	ege::Ray out;
	return out;
}

ege::Ray ege::Camera::getRayFromScreenPosition(const vec2& _position, const vec2& _size) {
	vec2 half = _size * 0.5f;
	return getRayFromScreen(_position/half - vec2(1,1));
}

