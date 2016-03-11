/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/widget/Mesh.h>
#include <ewol/widget/Manager.h>
#include <gale/resource/Manager.h>
#include <ege/debug.h>

#undef __class__
#define __class__ "Mesh"



ege::widget::Mesh::Mesh():
  signalPressed(this, "pressed", ""),
  m_position(0,0,0),
  m_angle(0,0,0),
  m_angleSpeed(0,0,0),
  m_cameraDistance(10.0) {
	addObjectType("ege::widget::Mesh");
}

void ege::widget::Mesh::init() {
	ewol::Widget::init();
	m_meshName = *propertyName;
	// Limit event at 1:
	setMouseLimit(1);
	if (*propertyName!="") {
		m_object = ege::resource::Mesh::create(m_meshName);
		if (nullptr == m_object) {
			EGE_ERROR("Can not load the resource : \"" << m_meshName << "\"");
		}
	}
}

ege::widget::Mesh::~Mesh() {
	
}

void ege::widget::Mesh::onDraw() {
	mat4 transformationMatrix =   etk::matTranslate(vec3(0,0,-m_cameraDistance))
	                            * etk::matTranslate(m_position)
	                            * etk::matRotate(vec3(1,0,0),m_angle.x())
	                            * etk::matRotate(vec3(0,1,0),m_angle.y())
	                            * etk::matRotate(vec3(0,0,1),m_angle.z());
	if (nullptr != m_object) {
		m_object->draw(transformationMatrix);
	}
}

void ege::widget::Mesh::systemDraw(const ewol::DrawProperty& _displayProp) {
	gale::openGL::push();
	// here we invert the reference of the standard openGl view because the reference in the common display is Top left and not buttom left
	gale::openGL::setViewPort(m_origin, m_size);
	float ratio = m_size.x() / m_size.y();
	//EGE_INFO("ratio : " << ratio);
	mat4 tmpProjection = etk::matPerspective(M_PI/3.0, ratio, 0.5, 100);
	//mat4 tmpMat = tmpProjection * m_camera.getMatrix();
	// set internal matrix system :
	//ewol::openGL::setMatrix(tmpMat);
	gale::openGL::setMatrix(tmpProjection);
	
	onDraw();
	gale::openGL::pop();
}

void ege::widget::Mesh::onRegenerateDisplay() {
	if (true == needRedraw()) {
		
	}
}

void ege::widget::Mesh::periodicCall(const ewol::event::Time& _event) {
	m_angle += m_angleSpeed*_event.getDeltaCall();
	markToRedraw();
}

bool ege::widget::Mesh::onEventInput(const ewol::event::Input& _event) {
	//EGE_DEBUG("Event on BT ...");
	if (_event.getId() == 1) {
		if(_event.getStatus() == gale::key::status_single) {
			signalPressed.emit();
			return true;
		}
	}
	return false;
}

void ege::widget::Mesh::setFile(const std::string& _filename) {
	if(    _filename != ""
	    && m_meshName != _filename ) {
		m_meshName = _filename;
		m_object = ege::resource::Mesh::create(m_meshName);
		if (nullptr == m_object) {
			EGE_ERROR("Can not load the resource : \"" << m_meshName << "\"");
		}
	}
	markToRedraw();
}

void ege::widget::Mesh::setPosition(const vec3& _pos) {
	m_position = _pos;
	markToRedraw();
}

void ege::widget::Mesh::setAngle(const vec3& _angle) {
	m_angle = _angle;
	markToRedraw();
}

void ege::widget::Mesh::setAngleSpeed(const vec3& _speed) {
	if (_speed!=vec3(0,0,0)) {
		periodicCallEnable();
	} else {
		periodicCallDisable();
	}
	m_angleSpeed = _speed;
	markToRedraw();
}

void ege::widget::Mesh::setDistance(float _distance) {
	m_cameraDistance = _distance;
	markToRedraw();
}
