/**
 * @author Edouard DUPIN
 * 
 * @copyright 2013, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ege/Camera.h>
#include <ege/debug.h>

#undef __class__
#define __class__	"Camera"

void ege::Camera::update(void)
{
	// Note the view axes of the basic camera is (0,0,-1)
	// clean matrix :
	m_matrix.identity();
	// move the world from the camera distance
	m_matrix.translate(vec3(0,0,-m_distance));
	// teta represent the angle from the ground to the axes then 90-teta represent the angle to apply (inverted for the world)
	m_matrix.rotate(vec3(1,0,0), -((M_PI/2.0f-m_angleTeta)*m_offsetFactor) );
	m_matrix.rotate(vec3(0,0,1), -m_angleZ );
	// move the camera to the end point of view
	m_matrix.translate(-m_eye);
	#if 1
		m_calculatedViewVector = vec3(0,0,-1);
		m_calculatedViewVector = m_calculatedViewVector.rotate(vec3(1,0,0), -((M_PI/2.0f-m_angleTeta)*m_offsetFactor) );
		m_calculatedViewVector = m_calculatedViewVector.rotate(vec3(0,0,1), -m_angleZ );
		m_calculatedViewVector *= vec3(1,-1,1);
		m_calculatedOrigin = m_eye - m_calculatedViewVector*m_distance;
	#else
		float onGround = cosf(m_angleTeta)*m_distance;
		m_calculatedOrigin.setValue(sinf(m_angleZ)*onGround,
		                            -cosf(m_angleZ)*onGround,
		                            sinf(m_angleTeta)*m_distance);
		m_calculatedViewVector= -m_calculatedOrigin.normalized();
		m_calculatedOrigin += m_eye;
	#endif
	
	#if 0
		EGE_DEBUG("Camera properties : eye=" << m_eye );
		EGE_DEBUG("                      Z=" << RAD_TO_DEG(m_angleZ) << "");
		EGE_DEBUG("                   Teta=" << RAD_TO_DEG(m_angleTeta) << "" );
		EGE_DEBUG("               distance=" << m_distance);
		EGE_DEBUG("                 origin=" << m_calculatedOrigin );
		EGE_DEBUG(" m_calculatedViewVector=" << m_calculatedViewVector);
	#endif
}

vec3 ege::Camera::projectOnZGround(const vec2& _cameraDeltaAngle, float _zValue)
{
	vec3 viewVector(0,0,-1);
	viewVector = viewVector.rotate(vec3(0,1,0), -_cameraDeltaAngle.x());
	viewVector = viewVector.rotate(vec3(1,0,0), -_cameraDeltaAngle.y());
	viewVector = viewVector.rotate(vec3(1,0,0), -((M_PI/2.0f-m_angleTeta)*m_offsetFactor) );
	viewVector = viewVector.rotate(vec3(0,0,1), -m_angleZ );
	viewVector *= vec3(1,-1,1);
	// get intersection with the plane : z=0 
	// equation : pointIntersect = viewVector * alpha + origin;
	float alpha = (_zValue - m_calculatedOrigin.z()) / viewVector.z();
	return vec3(viewVector.x()*alpha + m_calculatedOrigin.x(),
	            viewVector.y()*alpha + m_calculatedOrigin.y(),
	            _zValue);
}

ege::Camera::Camera(vec3 _eye, float _angleZ, float _angleTeta, float _distance) : 
	m_eye(_eye),
	m_angleZ(_angleZ),
	m_angleTeta(_angleTeta),
	m_distance(_distance),
	m_offsetFactor(1.0f),
	m_forceViewTop(false)
{
	setEye(_eye);
	setAngleZ(_angleZ);
	setDistance(_distance);
	setAngleTeta(_angleTeta);
	update();
}

void ege::Camera::setEye(vec3 _eye)
{
	m_eye = _eye;
	if (m_eye.x() < -1000) {
		m_eye.setX(-1000);
	}
	if (m_eye.x() > 1000) {
		m_eye.setX(1000);
	}
	if (m_eye.y() < -1000) {
		m_eye.setY(-1000);
	}
	if (m_eye.y() > 1000) {
		m_eye.setY(1000);
	}
	if (m_eye.z() < -10) {
		m_eye.setZ(-10);
	}
	if (m_eye.z() > 10) {
		m_eye.setZ(10);
	}
	update();
}

void ege::Camera::setAngleZ(float _angleZ)
{
	if (_angleZ == NAN) {
		EGE_CRITICAL("try to set NAN value for the angle ...");
	} else if (_angleZ == INFINITY) {
		EGE_CRITICAL("try to set INFINITY value for the angle ...");
	} else {
		m_angleZ = _angleZ;
	}
	update();
}

void ege::Camera::setAngleTeta(float _angleTeta)
{
	m_angleTeta = etk_avg(M_PI/10.0f, _angleTeta, M_PI/2.0f);
	update();
}

void ege::Camera::setDistance(float _distance)
{
	m_distance = etk_avg(5, _distance, 150);
	update();
}

const float localFactor = 2.0;

void ege::Camera::periodicCall(float _step)
{
	//Note we need to view to the top in 500ms
	if (true == m_forceViewTop) {
		if (0.0f != m_offsetFactor) {
			m_offsetFactor -= _step*localFactor;
			m_offsetFactor = etk_avg(0,m_offsetFactor,1);
			update();
		}
	} else {
		if (1.0f != m_offsetFactor) {
			m_offsetFactor += _step*localFactor;
			m_offsetFactor = etk_avg(0,m_offsetFactor,1);
			update();
		}
	}
}
