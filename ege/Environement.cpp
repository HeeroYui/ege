/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/Environement.h>
#include <ege/ElementGame.h>
#include <ewol/object/Manager.h>

#include <ewol/openGL/openGL.h>
#include <etk/math/Matrix4.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <LinearMath/btIDebugDraw.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btConvexPolyhedron.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <LinearMath/btTransformUtil.h>
#include <LinearMath/btIDebugDraw.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

#undef __class__
#define __class__ "Environement"


std::shared_ptr<ege::ElementGame> ege::Environement::getElementNearest(std::shared_ptr<ege::ElementGame> _sourceRequest, float& _distance) {
	if (_sourceRequest == nullptr) {
		return nullptr;
	}
	vec3 sourcePosition = _sourceRequest->getPosition();
	std::shared_ptr<ege::ElementGame> result = nullptr;
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		// chack nullptr  pointer
		if (m_listElementGame[iii] == nullptr) {
			continue;
		}
		if (m_listElementGame[iii]->getGroup() <= 0) {
			continue;
		}
		// check if they are in the same group:
		if (m_listElementGame[iii]->getGroup() == _sourceRequest->getGroup()) {
			continue;
		}
		// check distance ...
		vec3 destPosition = m_listElementGame[iii]->getPosition();
		float distance = btDistance(sourcePosition, destPosition);
		//EGE_DEBUG("Distance : " << _distance << " >? " << distance << " id=" << iii);
		if (_distance>distance) {
			_distance = distance;
			result = m_listElementGame[iii];
		}
	}
	return result;
}


void ege::Environement::getElementNearest(const vec3& _sourcePosition,
                                          float _distanceMax,
                                          std::vector<ege::Environement::ResultNearestElement>& _resultList) {
	_resultList.clear();
	ege::Environement::ResultNearestElement result;
	result.dist = 99999999999.0f;
	result.element = nullptr;
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		// chack nullptr  pointer
		result.element = m_listElementGame[iii];
		if (nullptr == result.element) {
			continue;
		}
		// check distance ...
		vec3 destPosition = result.element->getPosition();
		if (_sourcePosition == destPosition) {
			continue;
		}
		result.dist = btDistance(_sourcePosition, destPosition);
		//EGE_DEBUG("Distance : " << _distance << " >? " << distance << " id=" << iii);
		if (_distanceMax>result.dist) {
			_resultList.push_back(result);
		}
	}
}

void ege::Environement::getElementNearestFixed(const vec3& _sourcePosition,
                                               float _distanceMax,
                                               std::vector<ege::Environement::ResultNearestElement>& _resultList) {
	_resultList.clear();
	ege::Environement::ResultNearestElement result;
	result.dist = 99999999999.0f;
	result.element = nullptr;
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		// chack nullptr  pointer
		result.element = m_listElementGame[iii];
		if (result.element == nullptr) {
			continue;
		}
		if (result.element->isFixed() == false) {
			continue;
		}
		// check distance ...
		vec3 destPosition = result.element->getPositionTheoric();
		result.dist = btDistance(_sourcePosition, destPosition);
		//EGE_DEBUG("Distance : " << _distance << " >? " << distance << " id=" << iii);
		if (_distanceMax <= result.dist) {
			continue;
		}
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
			_resultList.push_back(result);
		}
	}
}

static etk::Hash<ege::createElement_tf>& getHachTableCreating() {
	static etk::Hash<ege::createElement_tf> s_table;
	return s_table;
}

void ege::Environement::addCreator(const std::string& _type, ege::createElement_tf _creator) {
	if (_creator == nullptr) {
		EGE_ERROR("Try to add an empty CREATOR ...");
		return;
	}
	EGE_DEBUG("Add creator: " << _type);
	getHachTableCreating().add(_type, _creator);
	EGE_DEBUG("Add creator: " << _type << " (done)");
}

std::shared_ptr<ege::ElementGame> ege::Environement::createElement(const std::string& _type, bool _autoAddElement, enum ege::property _property, void* _value) {
	if (false == getHachTableCreating().exist(_type)) {
		EGE_ERROR("Request creating of an type that is not known '" << _type << "'");
		return nullptr;
	}
	ege::createElement_tf creatorPointer = getHachTableCreating()[_type];
	if (creatorPointer == nullptr) {
		EGE_ERROR("nullptr pointer creator  == > internal error... '" << _type << "'");
		return nullptr;
	}
	std::shared_ptr<ege::ElementGame> tmpElement = creatorPointer(std::dynamic_pointer_cast<ege::Environement>(shared_from_this()));
	if (tmpElement == nullptr) {
		EGE_ERROR("allocation error '" << _type << "'");
		return nullptr;
	}
	if (false == tmpElement->init(_property, _value)) {
		EGE_ERROR("Init error ... '" << _type << "'");
		return nullptr;
	}
	if (_autoAddElement == true) {
		addElementGame(tmpElement);
	}
	return tmpElement;
}

std::shared_ptr<ege::ElementGame> ege::Environement::createElement(const std::string& _type, std::string& _description, bool _autoAddElement) {
	return createElement(_type, _autoAddElement, ege::typeString, static_cast<void*>(&_description));
}

std::shared_ptr<ege::ElementGame> ege::Environement::createElement(const std::string& _type, ejson::Value* _value, bool _autoAddElement) {
	return createElement(_type, _autoAddElement, ege::typeJson, static_cast<void*>(_value));
}

std::shared_ptr<ege::ElementGame> ege::Environement::createElement(const std::string& _type, exml::Node* _node, bool _autoAddElement) {
	return createElement(_type, _autoAddElement, ege::typeXml, static_cast<void*>(_node));
}


void ege::Environement::addElementGame(std::shared_ptr<ege::ElementGame> _newElement) {
	// prevent memory allocation and un allocation ...
	if (_newElement == nullptr) {
		return;
	}
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		if (m_listElementGame[iii] == nullptr) {
			m_listElementGame[iii] = _newElement;
			m_listElementGame[iii]->dynamicEnable();
			return;
		}
	}
	m_listElementGame.push_back(_newElement);
	_newElement->dynamicEnable();
}

void ege::Environement::rmElementGame(std::shared_ptr<ege::ElementGame> _removeElement) {
	if (_removeElement == nullptr) {
		return;
	}
	// inform the element that an element has been removed  == > this permit to keep pointer on elements ...
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		if (m_listElementGame[iii] != nullptr) {
			m_listElementGame[iii]->elementIsRemoved(_removeElement);
		}
	}
	// ream remove on the element :
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		if (_removeElement == m_listElementGame[iii]) {
			m_listElementGame[iii]->onDestroy();
			m_listElementGame[iii]->dynamicDisable();
			m_listElementGame[iii]->unInit();
			m_listElementGame[iii].reset();
		}
	}
}


void ege::Environement::getOrderedElementForDisplay(std::vector<ege::Environement::ResultNearestElement>& _resultList,
                                                    const vec3& _position,
                                                    const vec3& _direction) {
	// remove all unneeded elements (old display...)
	_resultList.clear();
	// basic element result
	ege::Environement::ResultNearestElement result;
	result.dist = 99999999999.0f;
	result.element = nullptr;
	// for all element in the game we chek if it is needed to display it ...
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		// chack nullptr  pointer
		if (nullptr == m_listElementGame[iii]) {
			// no pointer null are set in the output list ...
			continue;
		}
		result.element = m_listElementGame[iii];
		// check distance ...
		vec3 destPosition = result.element->getPosition();
		vec3 angleView = (destPosition - _position).normalized();
		float dotResult=angleView.dot(_direction);
		//EGE_DEBUG("Dot position : " << destPosition << "  == > dot=" << dotResult);
		if (dotResult <= 0.85f) {
			// they are not in the camera angle view ...  == > no need to process display
			continue;
		}
		result.dist = btDistance(_position, destPosition);
		if (result.dist>500.0f) {
			// The element is realy too far ...  == > no need to display
			continue;
		}
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
			_resultList.push_back(result);
		}
	}
}


void ege::Environement::generateInteraction(ege::ElementInteraction& _event) {
	// inform the element that an element has been removed  == > this permit to keep pointer on elements ...
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		if (nullptr == m_listElementGame[iii]) {
			continue;
		}
		_event.applyEvent(*m_listElementGame[iii]);
		/*
		vec3 destPosition = m_listElementGame[iii]->getPosition();
		float dist = btDistance(sourcePosition, destPosition);
		if (dist == 0 || dist>decreasePower) {
			continue;
		}
		float inpact = (decreasePower-dist)/decreasePower * power;
		g_listElementGame[iii]->setFireOn(groupIdSource, type, -inpact, sourcePosition);
		*/
	}
}

ege::Environement::Environement() :
  signalPlayTimeChange(*this, "time-change"),
  m_dynamicsWorld(),
  m_listElementGame(),
  m_status(*this, "status", gameStart, "Satus of the activity of the Environement"),
  m_ratio(*this, "ratio", 1.0f, "game speed ratio"),
  m_particuleEngine(*this) {
	// nothing to do ...
	m_status.add(gameStart, "start", "Scene is started");
	m_status.add(gameStop, "stop", "Scene is stopped");
}

void ege::Environement::init() {
	ewol::Object::init();
	getObjectManager().periodicCall.bind(shared_from_this(), &ege::Environement::periodicCall);
	
}

void ege::Environement::clear() {
	m_listElementGame.clear();
}


void ege::Environement::periodicCall(const ewol::event::Time& _event) {
	float curentDelta = _event.getDeltaCall();
	// small hack to change speed ...
	curentDelta *= m_ratio;
	// check if the processing is availlable
	if (m_status.get() == gameStop) {
		return;
	}
	// update game time:
	int32_t lastGameTime = m_gameTime*0.000001f;
	m_gameTime += curentDelta;
	if (lastGameTime != (int32_t)(m_gameTime*0.000001f)) {
		signalPlayTimeChange.emit(m_gameTime*0.000001f);
	}
	
	//EWOL_DEBUG("Time: m_lastCallTime=" << m_lastCallTime << " deltaTime=" << deltaTime);
	
	// update camera positions:
	for (auto &it : m_listCamera) {
		if (it.second != nullptr) {
			it.second->periodicCall(curentDelta);
		}
	}
	//EGE_DEBUG("stepSimulation (start)");
	///step the simulation
	if (m_dynamicsWorld != nullptr) {
		m_dynamicsWorld->stepSimulation(curentDelta);
		//optional but useful: debug drawing
		m_dynamicsWorld->debugDrawWorld();
	}
	m_particuleEngine.update(curentDelta);
	// remove all element that requested it ...
	{
		int32_t numberEnnemyKilled=0;
		int32_t victoryPoint=0;
		auto it(m_listElementGame.begin());
		while (it != m_listElementGame.end()) {
			if(*it != nullptr) {
				if ((*it)->needToRemove() == true) {
					if ((*it)->getGroup() > 1) {
						numberEnnemyKilled++;
						victoryPoint++;
					}
					EGE_DEBUG("[" << (*it)->getUID() << "] element Removing ... " << (*it)->getType());
					rmElementGame((*it));
				}
			}
		}
		if (0 != numberEnnemyKilled) {
			//signalKillEnemy.emit(numberEnnemyKilled);
		}
	}
}


void ege::Environement::addCamera(const std::string& _name, const std::shared_ptr<ege::Camera>& _camera) {
	m_listCamera.insert(std::make_pair(_name, _camera));
}

std::shared_ptr<ege::Camera> ege::Environement::getCamera(const std::string& _name) {
	auto cameraIt = m_listCamera.find(_name);
	if (cameraIt != m_listCamera.end()) {
		return cameraIt->second;
	}
	return nullptr;
}
