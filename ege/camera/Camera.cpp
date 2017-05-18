/** @file
 * @author Edouard DUPIN
 * @copyright 2013, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <ege/camera/Camera.hpp>
#include <ege/debug.hpp>
#include <ege/Ray.hpp>

#include <gale/renderer/openGL/openGL.hpp>

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
	gale::openGL::setCameraMatrix(getMatrixCamera());
	gale::openGL::setMatrix(getMatrixProjection());
}


ege::Ray ege::Camera::getRayFromScreen(const vec2& _offset) {
	ege::Ray out;
	return out;
}

ege::Ray ege::Camera::getRayFromScreenPosition(const vec2& _position, const vec2& _size) {
	vec2 half = _size * 0.5f;
	return getRayFromScreen(_position/half - vec2(1,1));
}

vec2 ege::Camera::tansformPositionToAngle(vec3 _vect) {
	vec2 out(0.0f, 0.0f);
	float distance = _vect.length();
	if (distance == 0.0f) {
		return out;
	}
	out.setY(std::asin(_vect.z()/distance));
	_vect.setZ(0.0f);
	if (_vect.x() == 0 && _vect.y() == 0) {
		return out;
	}
	_vect.normalize();
	out.setX(std::asin(_vect.y()));
	if (_vect.x() < 0) {
		out.setX(out.x()*-1 - M_PI);
	}
	return out;
}