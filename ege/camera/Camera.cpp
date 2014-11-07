/**
 * @author Edouard DUPIN
 * 
 * @copyright 2013, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ege/camera/Camera.h>
#include <ege/debug.h>

#undef __class__
#define __class__ "Camera"

ege::Camera::Camera() {
	m_matrix.identity();
}
