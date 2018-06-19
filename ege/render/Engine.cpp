/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/render/Engine.hpp>

ege::render::Engine::Engine(ege::Environement* _env) :
  ege::Engine(_env),
  propertyDebugNormal(this, "debug-normal", false, "display the normal debug information") {
	m_debugDrawProperty = ewol::resource::Colored3DObject::create();
}

const etk::String& ege::render::Engine::getType() const {
	static etk::String tmp("render");
	return tmp;
}

void ege::render::Engine::componentRemove(const ememory::SharedPtr<ege::Component>& _ref) {
	ememory::SharedPtr<ege::render::Component> ref = ememory::dynamicPointerCast<ege::render::Component>(_ref);
	for (auto it = m_component.begin();
	     it != m_component.end();
	     ++it) {
		if (*it == ref) {
			it->reset();
			return;
		}
	}
}

void ege::render::Engine::componentAdd(const ememory::SharedPtr<ege::Component>& _ref) {
	ememory::SharedPtr<ege::render::Component> ref = ememory::dynamicPointerCast<ege::render::Component>(_ref);
	for (auto it=m_component.begin();
	     it != m_component.end();
	     ++it) {
		if (*it == null) {
			*it = ref;
			return;
		}
	}
	m_component.pushBack(ref);
}


void ege::render::Engine::getOrderedElementForDisplay(etk::Vector<ege::render::Engine::ResultNearestElement>& _resultList,
                                                      const vec3& _position,
                                                      const vec3& _direction) {
	// TODO : Set it back ... corrected...
	// remove all unneeded elements (old display...)
	_resultList.clear();
	// basic element result
	ege::render::Engine::ResultNearestElement result;
	result.dist = 99999999999.0f;
	result.element = null;
	// for all element in the game we chek if it is needed to display it ...
	for (auto &it: m_component) {
		// check null  pointer
		if (it == null) {
			// no pointer null are set in the output list ...
			continue;
		}
		result.element = it;
		// check distance ...
		vec3 destPosition = result.element->getTransform().getPosition();
		vec3 angleView = (destPosition - _position);
		angleView.safeNormalize();
		float dotResult = angleView.dot(_direction);
		//EGE_DEBUG("Dot position : " << destPosition << "  == > dot=" << dotResult);
		/*
		if (dotResult <= 0.85f) {
			// they are not in the camera angle view ...  == > no need to process display
			continue;
		}
		*/
		result.dist = (_position - destPosition).length();
		/*
		if (result.dist>500.0f) {
			// The element is realy too far ...  == > no need to display
			continue;
		}
		*/
		// try to add the element at the best positions:
		size_t jjj;
		for (jjj=0; jjj<_resultList.size(); jjj++) {
			if (_resultList[jjj].dist>result.dist) {
				_resultList.insert(_resultList.begin()+jjj, result);
				break;
			}
		}
		// add element at the end :
		if (jjj >= _resultList.size()) {
			_resultList.pushBack(result);
		}
	}
}
#define  NUMBER_OF_SUB_PASS (5)

void ege::render::Engine::render(const echrono::Duration& _delta, const ememory::SharedPtr<ege::Camera>& _camera) {
	/*
	for (auto &it : m_component) {
		if (it == null) {
			continue;
		}
		
	}
	*/
	//EGE_DEBUG("Draw (start)");
	mat4 tmpMatrix;
	getOrderedElementForDisplay(m_displayElementOrdered, _camera->getEye(), _camera->getViewVector());
	EGE_VERBOSE("DRAW : " << m_displayElementOrdered.size() << "/" << m_component.size() << " elements");
	
	// note :  the first pass is done at the reverse way to prevent multiple display od the same point in the screen 
	//         (and we remember that the first pass is to display all the non transparent elements)
	for (int32_t iii=m_displayElementOrdered.size()-1; iii >= 0; iii--) {
		m_displayElementOrdered[iii].element->draw(0);
	}
	// for the other pass the user can draw transparent elements ...
	for (int32_t pass=1; pass <= NUMBER_OF_SUB_PASS+1; pass++) {
		for (auto &it: m_displayElementOrdered) {
			it.element->draw(pass);
		}
	}
}

void ege::render::Engine::renderDebug(const echrono::Duration& _delta, const ememory::SharedPtr<ege::Camera>& _camera) {
	//EGE_DEBUG("Draw (start)");
	mat4 tmpMatrix;
	getOrderedElementForDisplay(m_displayElementOrdered, _camera->getEye(), _camera->getViewVector());
	EGE_VERBOSE("DRAW : " << m_displayElementOrdered.size() << "/" << m_component.size() << " elements");
	#if 0
	if (propertyDebugPhysic.get() == true) {
		// Draw debug ... (Object)
		for (int32_t iii=m_displayElementOrdered.size()-1; iii >= 0; iii--) {
			m_displayElementOrdered[iii].element->drawDebug(m_debugDrawProperty, camera);
		}
		// Draw debug ... (Camera)
		/*
		etk::Map<etk::String, ememory::SharedPtr<ege::Camera>> listCamera = m_env->getCameraList();
		for (auto &itCam : listCamera) {
			if (itCam.second != null) {
				itCam.second->drawDebug(m_debugDrawProperty, camera);
			}
		}
		*/
	}
	#endif
	if (propertyDebugNormal.get() == true) {
		// Draw debug ... (Object)
		for (int32_t iii=m_displayElementOrdered.size()-1; iii >= 0; iii--) {
			m_displayElementOrdered[iii].element->drawNormalDebug(m_debugDrawProperty);
		}
	}
	#if 0
	if (propertyDebugApplication.get() == true) {
		// Draw debug ... (User)
		signalDisplayDebug.emit(m_debugDrawProperty);
	}
	/* TODO: set it back ...
	if (camera != null) {
		m_env->getParticuleEngine().draw(*camera);
	}
	*/
	#endif
}