/** @file
 * @author Edouard DUPIN
 * @copyright 2013, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */


#include <ewol/object/Object.hpp>
#include <ewol/object/Manager.hpp>
#include <ege/camera/ControlBase.hpp>
#include <ege/debug.hpp>


ege::camera::ControlBase::ControlBase() :
  m_destinationCameraOffset(0,0,0),
  m_angleTetha(0.0f),
  m_anglePsy(0.0f),
  m_distance(1.0f) {
	
}

void ege::camera::ControlBase::setCamera(const ememory::SharedPtr<ege::camera::View>& _camera) {
	m_camera.reset();
	m_PCH.disconnect();
	if (_camera == nullptr) {
		return;
	}
	m_camera = _camera;
	m_camera->setTarget(vec3(0,0,0));
	m_camera->setEye(vec3(100*std::sin(m_angleTetha),80*std::cos(m_anglePsy),100*std::cos(m_angleTetha))*m_distance);
	m_PCH = ewol::Object::getObjectManager().periodicCall.connect(this, &ege::camera::ControlBase::periodicCall);
}



bool ege::camera::ControlBase::onEventEntry(const ewol::event::Entry& _event) {
	if (_event.getType() == gale::key::keyboard::left) {
		if (_event.getStatus() == gale::key::status::down) {
			m_destinationCameraOffset += vec3(1,0,0);
		} else if (_event.getStatus() == gale::key::status::up) {
			m_destinationCameraOffset -= vec3(1,0,0);
		}
		return true;
	}
	if (_event.getType() == gale::key::keyboard::right) {
		if (_event.getStatus() == gale::key::status::down) {
			m_destinationCameraOffset -= vec3(1,0,0);
		} else if (_event.getStatus() == gale::key::status::up) {
			m_destinationCameraOffset += vec3(1,0,0);
		}
		return true;
	}
	if (_event.getType() == gale::key::keyboard::up) {
		if (_event.getStatus() == gale::key::status::down) {
			m_destinationCameraOffset += vec3(0,0,1);
		} else if (_event.getStatus() == gale::key::status::up) {
			m_destinationCameraOffset -= vec3(0,0,1);
		}
		return true;
	}
	if (_event.getType() == gale::key::keyboard::down) {
		if (_event.getStatus() == gale::key::status::down) {
			m_destinationCameraOffset -= vec3(0,0,1);
		} else if (_event.getStatus() == gale::key::status::up) {
			m_destinationCameraOffset += vec3(0,0,1);
		}
		return true;
	}
	if (_event.getType() == gale::key::keyboard::pageUp) {
		if (_event.getStatus() == gale::key::status::down) {
			m_destinationCameraOffset += vec3(0,1,0);
		} else if (_event.getStatus() == gale::key::status::up) {
			m_destinationCameraOffset -= vec3(0,1,0);
		}
		return true;
	}
	if (_event.getType() == gale::key::keyboard::pageDown) {
		if (_event.getStatus() == gale::key::status::down) {
			m_destinationCameraOffset -= vec3(0,1,0);
		} else if (_event.getStatus() == gale::key::status::up) {
			m_destinationCameraOffset += vec3(0,1,0);
		}
		return true;
	}
	if (_event.getType() == gale::key::keyboard::start) {
		if (_event.getStatus() == gale::key::status::down) {
			m_camera->setAngle(m_camera->getAngle() + 0.01f);
		}
		return true;
	}
	if (_event.getType() == gale::key::keyboard::end) {
		if (_event.getStatus() == gale::key::status::down) {
			m_camera->setAngle(m_camera->getAngle() - 0.01f);
		}
		return true;
	}
	if (_event.getType() == gale::key::keyboard::insert) {
		if (_event.getStatus() == gale::key::status::down) {
			m_camera->setXAngleView(m_camera->getXAngleView() + 0.01f);
		}
		return true;
	}
	if (    _event.getType() == gale::key::keyboard::character
	     && _event.getChar() == u32char::Suppress) {
		if (_event.getStatus() == gale::key::status::down) {
			m_camera->setXAngleView(m_camera->getXAngleView() - 0.01f);
		}
		return true;
	}
	return false;
}

bool ege::camera::ControlBase::onEventInput(const ewol::event::Input& _event, const vec2& _relativePosition) {
	if (m_camera == nullptr) {
		return false;
	}
	if (_event.getId() == 4) {
		// scrool button ==> zoom in
		m_distance += 0.01f;
		m_camera->setEye(vec3(100*std::sin(m_angleTetha),80*std::cos(m_anglePsy),100*std::cos(m_angleTetha))*m_distance);
		return true;
	} else if (_event.getId() == 5) {
		// scrool button ==> zoom OUT
		m_distance -= 0.01f;
		if (m_distance <= 0.05f) {
			m_distance = 0.05f;
		}
		m_camera->setEye(vec3(100*std::sin(m_angleTetha),80*std::cos(m_anglePsy),100*std::cos(m_angleTetha))*m_distance);
		return true;
	} else if (_event.getId() == 3) {
		// Middle button ==> move around the target position
		if (_event.getStatus() == gale::key::status::down) {
			m_oldScreenPos = _relativePosition;
		} else if (_event.getStatus() == gale::key::status::move) {
			vec2 pos = _relativePosition;
			m_angleTetha += (m_oldScreenPos.x()-pos.x())*0.02f;
			m_anglePsy += (m_oldScreenPos.y()-pos.y())*0.01f;
			m_camera->setEye(m_camera->getTarget() + vec3(100*std::sin(m_angleTetha),80*std::cos(m_anglePsy),100*std::cos(m_angleTetha))*m_distance);
			m_oldScreenPos = _relativePosition;
		}
		return true;
	} else if (_event.getId() == 2) {
		// Middle button ==> move the camera view axis
		if (_event.getStatus() == gale::key::status::down) {
			m_oldScreenPos = _relativePosition;
		} else if (_event.getStatus() == gale::key::status::move) {
			vec2 pos = _relativePosition*0.2;
			pos -= m_oldScreenPos*0.2;
			float cameraAngle = m_camera->getTetha();
			vec3 newPos = vec3(std::sin(cameraAngle)*pos.x() + std::cos(cameraAngle)*pos.y(),
			                   std::cos(cameraAngle)*pos.x() + std::sin(cameraAngle)*pos.y(),
			                   0);
			EGE_ERROR("apply offset = " << newPos << " from pos=" << pos << " angle=" << cameraAngle);
			newPos += m_camera->getTarget();
			newPos.setMin(vec3(200,200,200));
			newPos.setMax(vec3(-200,-200,-200));
			m_camera->setTarget(newPos);
			m_oldScreenPos = _relativePosition;
		}
		return true;
	}
	return false;
}

void ege::camera::ControlBase::periodicCall(const ewol::event::Time& _event) {
	if (m_camera == nullptr) {
		return;
	}
	if (    m_destinationCameraOffset.x() < 0.7f
	     && m_destinationCameraOffset.x() > -0.7f) {
		m_destinationCameraOffset.setX(0.0f);
	}
	if (    m_destinationCameraOffset.y() < 0.7f
	     && m_destinationCameraOffset.y() > -0.7f) {
		m_destinationCameraOffset.setY(0.0f);
	}
	if (    m_destinationCameraOffset.z() < 0.7f
	     && m_destinationCameraOffset.z() > -0.7f) {
		m_destinationCameraOffset.setZ(0.0f);
	}
	float delta = _event.getDeltaCall();
	vec3 tmp = m_destinationCameraOffset * delta * 30.0f;
	m_camera->setTarget(m_camera->getTarget() + tmp);
	m_camera->setEye(m_camera->getEye() + tmp);
}

