/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/Environement.hpp>
#include <ege/Entity.hpp>
#include <ewol/object/Manager.hpp>

#include <ege/particule/Engine.hpp>
#include <ege/render/Engine.hpp>
#include <ege/ia/Engine.hpp>
#include <ege/physics/Engine.hpp>

#include <gale/renderer/openGL/openGL.hpp>
#include <etk/math/Matrix4x4.hpp>
#include <etk/typeInfo.hpp>

ETK_DECLARE_TYPE(ege::gameStatus);

void ege::Environement::addEngine(const ememory::SharedPtr<ege::Engine>& _ref) {
	if (_ref == null) {
		EGE_ERROR("try to add an empty Engine");
		return;
	}
	// check if not exist
	for (auto &it: m_engine) {
		if (it == null) {
			continue;
		}
		if (it->getType() == _ref->getType()) {
			it = _ref;
			return;
		}
	}
	// try to add in an empty slot
	for (auto &it: m_engine) {
		if (it != null) {
			continue;
		}
		it = _ref;
		return;
	}
	// add it at the end ...
	m_engine.pushBack(_ref);
}

void ege::Environement::rmEngine(const ememory::SharedPtr<ege::Engine>& _ref) {
	if (_ref == null) {
		EGE_ERROR("try to remove an empty engine");
		return;
	}
	// check if not exist
	for (auto &it: m_engine) {
		if (it == null) {
			continue;
		}
		if (it == _ref) {
			it = null;
			return;
		}
	}
	EGE_ERROR("try to remove an unexisting engine");
}

void ege::Environement::rmEngine(const etk::String& _type) {
	// check if not exist
	for (auto &it: m_engine) {
		if (it == null) {
			continue;
		}
		if (it->getType() == _type) {
			it = null;
			return;
		}
	}
	EGE_ERROR("try to remove an unexisting engine type : '" << _type << "'");
	return;
}

ememory::SharedPtr<ege::Engine> ege::Environement::getEngine(const etk::String& _type) {
	// check if not exist
	for (auto &it: m_engine) {
		if (it == null) {
			continue;
		}
		if (it->getType() == _type) {
			return it;
		}
	}
	EGE_ERROR("try to get an unexisting engine type : '" << _type << "'");
	return null;
}

void ege::Environement::engineComponentRemove(const ememory::SharedPtr<ege::Component>& _ref) {
	for (auto &it: m_engine) {
		if (it == null) {
			continue;
		}
		if (it->getType() == _ref->getType()) {
			it->componentRemove(_ref);
			return;
		}
	}
}
void ege::Environement::engineComponentAdd(const ememory::SharedPtr<ege::Component>& _ref) {
	for (auto &it: m_engine) {
		if (it == null) {
			continue;
		}
		if (it->getType() == _ref->getType()) {
			it->componentAdd(_ref);
			return;
		}
	}
}

/*
ememory::SharedPtr<ege::Entity> ege::Environement::getEntityNearest(ememory::SharedPtr<ege::Entity> _sourceRequest, float& _distance) {
	if (_sourceRequest == null) {
		return null;
	}
	vec3 sourcePosition = _sourceRequest->getPosition();
	ememory::SharedPtr<ege::Entity> result = null;
	for (size_t iii=0; iii<m_listEntity.size() ; iii++) {
		// chack null  pointer
		if (m_listEntity[iii] == null) {
			continue;
		}
		if (m_listEntity[iii]->getGroup() <= 0) {
			continue;
		}
		// check if they are in the same group:
		if (m_listEntity[iii]->getGroup() == _sourceRequest->getGroup()) {
			continue;
		}
		// check distance ...
		vec3 destPosition = m_listEntity[iii]->getPosition();
		float distance = (sourcePosition - destPosition).length();
		//EGE_DEBUG("Distance : " << _distance << " >? " << distance << " id=" << iii);
		if (_distance>distance) {
			_distance = distance;
			result = m_listEntity[iii];
		}
	}
	return result;
}


void ege::Environement::getEntityNearest(const vec3& _sourcePosition,
                                          float _distanceMax,
                                          etk::Vector<ege::Environement::ResultNearestEntity>& _resultList) {
	_resultList.clear();
	ege::Environement::ResultNearestEntity result;
	result.dist = 99999999999.0f;
	result.entity = null;
	for (size_t iii=0; iii<m_listEntity.size() ; iii++) {
		// chack null  pointer
		result.entity = m_listEntity[iii];
		if (result.entity == null) {
			continue;
		}
		// check distance ...
		vec3 destPosition = result.entity->getPosition();
		if (_sourcePosition == destPosition) {
			continue;
		}
		result.dist = (_sourcePosition - destPosition).length();
		//EGE_DEBUG("Distance : " << _distance << " >? " << distance << " id=" << iii);
		if (_distanceMax>result.dist) {
			_resultList.pushBack(result);
		}
	}
}

void ege::Environement::getEntityNearestFixed(const vec3& _sourcePosition,
                                               float _distanceMax,
                                               etk::Vector<ege::Environement::ResultNearestEntity>& _resultList) {
	_resultList.clear();
	ege::Environement::ResultNearestEntity result;
	result.dist = 99999999999.0f;
	result.entity = null;
	for (size_t iii=0; iii<m_listEntity.size() ; iii++) {
		// chack null  pointer
		result.entity = m_listEntity[iii];
		if (result.entity == null) {
			continue;
		}
		if (result.entity->isFixed() == false) {
			continue;
		}
		// check distance ...
		vec3 destPosition = result.entity->getPositionTheoric();
		result.dist = (_sourcePosition - destPosition).length();
		//EGE_DEBUG("Distance : " << _distance << " >? " << distance << " id=" << iii);
		if (_distanceMax <= result.dist) {
			continue;
		}
		// try to add the entity at the best positions:
		size_t jjj;
		for (jjj=0; jjj<_resultList.size(); jjj++) {
			if (_resultList[jjj].dist>result.dist) {
				_resultList.insert(_resultList.begin()+jjj, result);
				break;
			}
		}
		// add entity at the end :
		if (jjj >= _resultList.size()) {
			_resultList.pushBack(result);
		}
	}
}
*/

static etk::Map<etk::String,ege::createEntity_tf>& getHachTableCreating() {
	static etk::Map<etk::String,ege::createEntity_tf> s_table(0,false);
	return s_table;
}

void ege::Environement::addCreator(const etk::String& _type, ege::createEntity_tf _creator) {
	if (_creator == null) {
		EGE_ERROR("Try to add an empty CREATOR ...");
		return;
	}
	EGE_DEBUG("Add creator: " << _type);
	getHachTableCreating().add(_type, _creator);
	EGE_DEBUG("Add creator: " << _type << " (done)");
}


ememory::SharedPtr<ege::Entity> ege::Environement::createEntity(const etk::String& _type, const ejson::Value& _value, bool _autoAddEntity) {
	if (getHachTableCreating().exist(_type) == false) {
		EGE_ERROR("Request creating of an type that is not known '" << _type << "'");
		return null;
	}
	ege::createEntity_tf creatorPointer = getHachTableCreating()[_type];
	if (creatorPointer == null) {
		EGE_ERROR("null pointer creator  == > internal error... '" << _type << "'");
		return null;
	}
	ememory::SharedPtr<ege::Entity> tmpEntity = creatorPointer(ememory::dynamicPointerCast<ege::Environement>(sharedFromThis()), _value);
	if (tmpEntity == null) {
		EGE_ERROR("allocation error '" << _type << "'");
		return null;
	}
	if (_autoAddEntity == true) {
		addEntity(tmpEntity);
	}
	return tmpEntity;
}

ememory::SharedPtr<ege::Entity> ege::Environement::createEntity(const etk::String& _type, bool _autoAddEntity) {
	ejson::Value empty;
	return createEntity(_type, empty, _autoAddEntity);
}

void ege::Environement::addEntity(ememory::SharedPtr<ege::Entity> _newEntity) {
	// prevent memory allocation and un allocation ...
	if (_newEntity == null) {
		return;
	}
	for (size_t iii=0; iii<m_listEntity.size() ; iii++) {
		if (m_listEntity[iii] == null) {
			m_listEntity[iii] = _newEntity;
			m_listEntity[iii]->dynamicEnable();
			return;
		}
	}
	m_listEntity.pushBack(_newEntity);
	_newEntity->dynamicEnable();
}

void ege::Environement::rmEntity(ememory::SharedPtr<ege::Entity> _removeEntity) {
	if (_removeEntity == null) {
		return;
	}
	// inform the entity that an entity has been removed  == > this permit to keep pointer on entitys ...
	for (size_t iii=0; iii<m_listEntity.size() ; iii++) {
		if (m_listEntity[iii] != null) {
			m_listEntity[iii]->entityIsRemoved(_removeEntity);
		}
	}
	// ream remove on the entity :
	for (size_t iii=0; iii<m_listEntity.size() ; iii++) {
		if (_removeEntity == m_listEntity[iii]) {
			m_listEntity[iii]->onDestroy();
			m_listEntity[iii]->dynamicDisable();
			m_listEntity[iii]->unInit();
			m_listEntity[iii].reset();
		}
	}
}

void ege::Environement::generateInteraction(ege::EntityInteraction& _event) {
	// inform the entity that an entity has been removed  == > this permit to keep pointer on entitys ...
	for (size_t iii=0; iii<m_listEntity.size() ; iii++) {
		if (m_listEntity[iii] == null) {
			continue;
		}
		_event.applyEvent(*m_listEntity[iii]);
		/*
		vec3 destPosition = m_listEntity[iii]->getPosition();
		float dist = (sourcePosition - destPosition).length;
		if (dist == 0 || dist>decreasePower) {
			continue;
		}
		float inpact = (decreasePower-dist)/decreasePower * power;
		g_listEntity[iii]->setFireOn(groupIdSource, type, -inpact, sourcePosition);
		*/
	}
}

ege::Environement::Environement() :
  signalPlayTimeChange(this, "time-change", ""),
  propertyStatus(this, "status",
                       gameStop,
                       "Satus of the activity of the Environement",
                       &ege::Environement::onChangePropertyStatus),
  propertyRatio(this, "ratio",
                      1.0f,
                      "game speed ratio"),
  m_listEntity() {
	// nothing to do ...
	propertyStatus.add(gameStart, "start", "Scene is started");
	propertyStatus.add(gamePause, "pause", "Scene is paused");
	propertyStatus.add(gameStop, "stop", "Scene is stopped");
	// we add the 4 classical engines (the order is used to the global rendering cycle ...
	addEngine(ememory::makeShared<ege::physics::Engine>(this));
	addEngine(ememory::makeShared<ege::ia::Engine>(this));
	addEngine(ememory::makeShared<ege::render::Engine>(this));
	addEngine(ememory::makeShared<ege::particule::Engine>(this));
}

void ege::Environement::clear() {
	m_listEntity.clear();
}


void ege::Environement::render(const echrono::Duration& _delta, const etk::String& _camera) {
	// get the correct camera:
	ememory::SharedPtr<ege::Camera> camera = getCamera(_camera);
	if (camera == null) {
		EGE_ERROR("Render: Can not get camera named: '" << _camera << "'");
		return;
	}
	for (auto &it: m_engine) {
		if(it == null) {
			continue;
		}
		EGE_VERBOSE("    render: " << it->getType());
		it->render(_delta, camera);
	}
	for (auto &it: m_engine) {
		if(it == null) {
			continue;
		}
		EGE_VERBOSE("    render: " << it->getType());
		it->renderDebug(_delta, camera);
	}
}

void ege::Environement::onCallbackPeriodicCall(const ewol::event::Time& _event) {
	float curentDelta = _event.getDeltaCall();
	EGE_VERBOSE("periodic call : " << _event);
	// small hack to change speed ...
	curentDelta *= *propertyRatio;
	// check if the processing is availlable
	if (propertyStatus.get() == gameStop) {
		return;
	}
	// update game time:
	int32_t lastGameTime = m_gameTime*0.000001f;
	m_gameTime += curentDelta;
	if (lastGameTime != (int32_t)(m_gameTime*0.000001f)) {
		EGE_VERBOSE("    Emit Signal");
		signalPlayTimeChange.emit(m_gameTime*0.000001f);
	}
	
	//EWOL_DEBUG("Time: m_lastCallTime=" << m_lastCallTime << " deltaTime=" << deltaTime);
	
	// update camera positions:
	for (auto &it : m_listCamera) {
		if (it.second != null) {
			EGE_VERBOSE("    update camera : '" << it.first << "'");
			it.second->periodicCall(curentDelta);
		}
	}
	EGE_VERBOSE("    step simulation : " << curentDelta);
	for (auto &it: m_engine) {
		if(it == null) {
			continue;
		}
		EGE_VERBOSE("    update: " << it->getType());
		it->update(echrono::Duration(double(curentDelta)));
	}
	
	//EGE_DEBUG("stepSimulation (start)");
	///step the simulation
	// TODO : m_physicEngine.update(curentDelta);
	// TODO : //optional but useful: debug drawing
	// TODO : m_physicEngine.debugDrawWorld();
	// TODO : EGE_INFO("    Update particule engine");
	// TODO : m_particuleEngine.update(curentDelta);
	// remove all entity that requested it ...
	/**
	{
		int32_t numberEnnemyKilled=0;
		int32_t victoryPoint=0;
		auto it(m_listEntity.begin());
		while (it != m_listEntity.end()) {
			if(*it != null) {
				if ((*it)->needToRemove() == true) {
					if ((*it)->getGroup() > 1) {
						numberEnnemyKilled++;
						victoryPoint++;
					}
					EGE_INFO("[" << (*it)->getUID() << "] entity Removing ... " << (*it)->getType());
					rmEntity((*it));
					it = m_listEntity.begin();
				} else {
					++it;
				}
			} else {
				++it;
			}
		}
		if (numberEnnemyKilled != 0) {
			//signalKillEnemy.emit(numberEnnemyKilled);
		}
	}
	*/
}


void ege::Environement::addCamera(const etk::String& _name, const ememory::SharedPtr<ege::Camera>& _camera) {
	m_listCamera.set(_name, _camera);
}

ememory::SharedPtr<ege::Camera> ege::Environement::getCamera(const etk::String& _name) {
	auto cameraIt = m_listCamera.find(_name);
	if (cameraIt != m_listCamera.end()) {
		return cameraIt->second;
	}
	return null;
}


void ege::Environement::onChangePropertyStatus() {
	if (propertyStatus.get() == gameStart) {
		m_periodicCallConnection = getObjectManager().periodicCall.connect(this, &ege::Environement::onCallbackPeriodicCall);
	} else {
		m_periodicCallConnection.disconnect();
	}
}


