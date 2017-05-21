/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <ege/debug.hpp>
#include <ege/elements/Element.hpp>
#include <ege/Environement.hpp>


const std::string& ege::Element::getType() const {
	static const std::string nameType("----");
	return nameType;
}


ege::Element::Element(const ememory::SharedPtr<ege::Environement>& _env) :
  m_env(_env),
  m_uID(0),
  m_life(100),
  m_lifeMax(100),
  m_group(0),
  m_radius(0) {
	static uint32_t unique=0;
	m_uID = unique;
	EGE_DEBUG("Create element: uId=" << m_uID);
	//m_debugText.setFontSize(12);
	unique++;
}

ege::Element::~Element() {
	EGE_DEBUG("Destroy element: uId=" << m_uID);
}

void ege::Element::addComponent(const ememory::SharedPtr<ege::Component>& _ref) {
	if (_ref == nullptr) {
		EGE_ERROR("try to add an empty component");
		return;
	}
	ememory::SharedPtr<ege::Component> componentRemoved;
	int32_t findId = -1;
	// check if not exist
	for (int32_t iii=0; iii<m_component.size(); ++iii) {
		if (m_component[iii] == nullptr) {
			continue;
		}
		if (m_component[iii]->getType() == _ref->getType()) {
			componentRemoved = m_component[iii];
			m_component[iii] = _ref;
			findId = iii;
			break;
		}
	}
	// try to add in an empty slot
	if (findId == -1) {
		for (int32_t iii=0; iii<m_component.size(); ++iii) {
			if (m_component[iii] != nullptr) {
				continue;
			}
			findId = iii;
			m_component[iii] = _ref;
			break;
		}
	}
	// add it at the end ...
	if (findId == -1) {
		findId = m_component.size();
		m_component.push_back(_ref);
	}
	for (int32_t iii=0; iii<m_component.size(); ++iii) {
		if (m_component[iii] == nullptr) {
			continue;
		}
		if (componentRemoved != nullptr) {
			m_env->engineComponentRemove(componentRemoved);
			m_component[iii]->removeFriendComponent(componentRemoved);
		}
		m_env->engineComponentAdd(_ref);
		m_component[iii]->addFriendComponent(_ref);
	}
	// notify new component of all previously added component:
	componentRemoved = _ref;
	for (int32_t iii=0; iii<m_component.size(); ++iii) {
		if (m_component[iii] == nullptr) {
			continue;
		}
		if (m_component[iii] == _ref) {
			continue;
		}
		componentRemoved->addFriendComponent(m_component[iii]);
	}
}

void ege::Element::rmComponent(const ememory::SharedPtr<ege::Component>& _ref) {
	if (_ref == nullptr) {
		EGE_ERROR("try to remove an empty component");
		return;
	}
	int32_t findId = -1;
	// check if not exist
	for (int32_t iii=0; iii<m_component.size(); ++iii) {
		if (m_component[iii] == nullptr) {
			continue;
		}
		if (m_component[iii] == _ref) {
			m_component[iii] = nullptr;
			findId = iii;
			break;
		}
	}
	if (findId == -1) {
		EGE_ERROR("try to remove an unexisting component");
		return;
	}
	for (int32_t iii=0; iii<m_component.size(); ++iii) {
		if (m_component[iii] == nullptr) {
			continue;
		}
		m_env->engineComponentRemove(_ref);
		m_component[iii]->removeFriendComponent(_ref);
	}
}

void ege::Element::rmComponent(const std::string& _type) {
	int32_t findId = -1;
	ememory::SharedPtr<ege::Component> componentRemoved;
	// check if not exist
	for (int32_t iii=0; iii<m_component.size(); ++iii) {
		if (m_component[iii] == nullptr) {
			continue;
		}
		if (m_component[iii]->getType() == _type) {
			componentRemoved = m_component[iii];
			m_component[iii] = nullptr;
			findId = iii;
			break;
		}
	}
	if (findId == -1) {
		EGE_ERROR("try to remove an unexisting component type : '" << _type << "'");
		return;
	}
	for (int32_t iii=0; iii<m_component.size(); ++iii) {
		if (m_component[iii] == nullptr) {
			continue;
		}
		m_env->engineComponentRemove(componentRemoved);
		m_component[iii]->removeFriendComponent(componentRemoved);
	}
}





bool ege::Element::init() {
	EGE_WARNING("init() not implemented: uId=" << m_uID);
	return false;
}
bool ege::Element::initString(const std::string& _description) {
	EGE_WARNING("String Init not implemented: uId=" << m_uID);
	return false;
}
bool ege::Element::initXML(const exml::Node& _node) {
	EGE_WARNING("xml Init not implemented: uId=" << m_uID);
	return false;
}
bool ege::Element::initJSON(const ejson::Value& _value) {
	EGE_WARNING("JSON Init not implemented: uId=" << m_uID);
	return false;
}
bool ege::Element::initVoid(void* _value) {
	EGE_WARNING("joid* Init not implemented: uId=" << m_uID);
	return false;
}
bool ege::Element::unInit() {
	return true;
}



float ege::Element::getLifeRatio() {
	if (0 >= m_life) {
		return 0;
	}
	return m_life/m_lifeMax;
}

void ege::Element::setFireOn(int32_t _groupIdSource, int32_t _type, float _power, const vec3& _center) {
	float previousLife = m_life;
	m_life += _power;
	m_life = std::avg(0.0f, m_life, m_lifeMax);
	if (m_life <= 0) {
		EGE_DEBUG("[" << getUID() << "] element is killed ..." << getType());
	}
	if (m_life != previousLife) {
		onLifeChange();
	}
}

const float lifeBorder = 0.1f;
const float lifeHeight = 0.3f;
const float lifeWidth = 2.0f;
const float lifeYPos = 1.7f;

#if 0
void ege::Element::drawLife(ememory::SharedPtr<ewol::resource::Colored3DObject> _draw, ememory::SharedPtr<ege::Camera> _camera) {
	if (_draw == nullptr) {
		return;
	}
	float ratio = getLifeRatio();
	if (ratio == 1.0f) {
		return;
	}
	#if 0
	mat4 transformationMatrix =   etk::matTranslate(getPosition())
	                            * etk::matRotate(vec3(0,0,1),_camera.getAngleZ())
	                            * etk::matRotate(vec3(1,0,0),(M_PI/2.0f-_camera.getAngleTeta()));
	std::vector<vec3> localVertices;
	localVertices.push_back(vec3(-lifeWidth/2.0-lifeBorder,lifeYPos           -lifeBorder,0));
	localVertices.push_back(vec3(-lifeWidth/2.0-lifeBorder,lifeYPos+lifeHeight+lifeBorder,0));
	localVertices.push_back(vec3( lifeWidth/2.0+lifeBorder,lifeYPos+lifeHeight+lifeBorder,0));
	localVertices.push_back(vec3(-lifeWidth/2.0-lifeBorder,lifeYPos           -lifeBorder,0));
	localVertices.push_back(vec3( lifeWidth/2.0+lifeBorder,lifeYPos+lifeHeight+lifeBorder,0));
	localVertices.push_back(vec3( lifeWidth/2.0+lifeBorder,lifeYPos           -lifeBorder,0));
	etk::Color<float> myColor(0x0000FF99);
	_draw->draw(localVertices, myColor, transformationMatrix, false, false);
	localVertices.clear();
	/** Bounding box  == > model shape **/
	localVertices.push_back(vec3(-lifeWidth/2.0                ,lifeYPos,0));
	localVertices.push_back(vec3(-lifeWidth/2.0                ,lifeYPos + lifeHeight,0));
	localVertices.push_back(vec3(-lifeWidth/2.0+lifeWidth*ratio,lifeYPos + lifeHeight,0));
	localVertices.push_back(vec3(-lifeWidth/2.0                ,lifeYPos,0));
	localVertices.push_back(vec3(-lifeWidth/2.0+lifeWidth*ratio,lifeYPos + lifeHeight,0));
	localVertices.push_back(vec3(-lifeWidth/2.0+lifeWidth*ratio,lifeYPos,0));
	myColor =0x00FF00FF;
	if (ratio < 0.2f) {
		myColor = 0xFF0000FF;
	} else if (ratio < 0.4f) {
		myColor = 0xDA7B00FF;
	}
	_draw->draw(localVertices, myColor, transformationMatrix, false, false);
	#endif
}
#endif

void ege::Element::drawDebug(ememory::SharedPtr<ewol::resource::Colored3DObject> _draw, ememory::SharedPtr<ege::Camera> _camera) {
	/*
	m_debugText.clear();
	m_debugText.setColor(etk::Color<>(0x00, 0xFF, 0x00, 0xFF));
	m_debugText.setPos(vec3(-20,32,0));
	m_debugText.print(getType());
	m_debugText.setPos(vec3(-20,20,0));
	m_debugText.print("life=("+etk::to_string(getLifeRatio()));
	*/
	//m_debugText.print(std::string("Axe=(")+std::string(m_tmpAxe.x())+std::string(",")+etk::UString(m_tmpAxe.y())+etk::UString(",")+etk::UString(m_tmpAxe.z())+etk::UString(")"));
	/*
	// TODO : Keep this it can be usefull to print something in direction of the camera ...
	m_debugText.draw(   etk::matTranslate(getPosition())
	                  * etk::matRotate(vec3(0,0,1),_camera.getAngleZ())
	                  * etk::matRotate(vec3(1,0,0),(M_PI/2.0f-_camera.getAngleTeta()))
	                  * etk::matScale(vec3(0.05,0.05,0.05)));
	*/
}




