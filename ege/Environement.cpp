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


#undef __class__
#define __class__ "Environement"


ege::ElementGame* ege::Environement::getElementNearest(ege::ElementGame* _sourceRequest, float& _distance) {
	if (NULL == _sourceRequest) {
		return NULL;
	}
	vec3 sourcePosition = _sourceRequest->getPosition();
	ege::ElementGame* result = NULL;
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		// chack NULL  pointer
		if (NULL == m_listElementGame[iii]) {
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
	result.element = NULL;
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		// chack NULL  pointer
		result.element = m_listElementGame[iii];
		if (NULL == result.element) {
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
	result.element = NULL;
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		// chack NULL  pointer
		result.element = m_listElementGame[iii];
		if (NULL == result.element) {
			continue;
		}
		if (false == result.element->isFixed()) {
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
	if (NULL == _creator) {
		EGE_ERROR("Try to add an empty CREATOR ...");
		return;
	}
	EGE_DEBUG("Add creator: " << _type);
	getHachTableCreating().add(_type, _creator);
	EGE_DEBUG("Add creator: " << _type << " (done)");
}

ege::ElementGame* ege::Environement::createElement(const std::string& _type, bool _autoAddElement, enum ege::property _property, void* _value) {
	if (false == getHachTableCreating().exist(_type)) {
		EGE_ERROR("Request creating of an type that is not known '" << _type << "'");
		return NULL;
	}
	ege::createElement_tf creatorPointer = getHachTableCreating()[_type];
	if (NULL == creatorPointer) {
		EGE_ERROR("NULL pointer creator  == > internal error... '" << _type << "'");
		return NULL;
	}
	ege::ElementGame* tmpElement = creatorPointer(*this);
	if (NULL == tmpElement) {
		EGE_ERROR("allocation error '" << _type << "'");
		return NULL;
	}
	if (false == tmpElement->init(_property, _value)) {
		EGE_ERROR("Init error ... '" << _type << "'");
		// remove created element ...
		delete(tmpElement);
		return NULL;
	}
	if (_autoAddElement == true) {
		addElementGame(tmpElement);
	}
	return tmpElement;
}

ege::ElementGame* ege::Environement::createElement(const std::string& _type, std::string& _description, bool _autoAddElement) {
	return createElement(_type, _autoAddElement, ege::typeString, static_cast<void*>(&_description));
}

ege::ElementGame* ege::Environement::createElement(const std::string& _type, ejson::Value* _value, bool _autoAddElement) {
	return createElement(_type, _autoAddElement, ege::typeJson, static_cast<void*>(_value));
}

ege::ElementGame* ege::Environement::createElement(const std::string& _type, exml::Node* _node, bool _autoAddElement) {
	return createElement(_type, _autoAddElement, ege::typeXml, static_cast<void*>(_node));
}


void ege::Environement::addElementGame(ege::ElementGame* _newElement) {
	// prevent memory allocation and un allocation ...
	if (NULL == _newElement) {
		return;
	}
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		if (NULL == m_listElementGame[iii]) {
			m_listElementGame[iii] = _newElement;
			m_listElementGame[iii]->dynamicEnable();
			return;
		}
	}
	m_listElementGame.push_back(_newElement);
	_newElement->dynamicEnable();
}

void ege::Environement::rmElementGame(ege::ElementGame* _removeElement) {
	if (NULL == _removeElement) {
		return;
	}
	// inform the element that an element has been removed  == > this permit to keep pointer on elements ...
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		if (NULL != m_listElementGame[iii]) {
			m_listElementGame[iii]->elementIsRemoved(_removeElement);
		}
	}
	// ream remove on the element :
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		if (_removeElement == m_listElementGame[iii]) {
			m_listElementGame[iii]->onDestroy();
			m_listElementGame[iii]->dynamicDisable();
			m_listElementGame[iii]->unInit();
			delete(m_listElementGame[iii]);
			m_listElementGame[iii] = NULL;
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
	result.element = NULL;
	// for all element in the game we chek if it is needed to display it ...
	for (size_t iii=0; iii<m_listElementGame.size() ; iii++) {
		// chack NULL  pointer
		if (NULL == m_listElementGame[iii]) {
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
		if (NULL == m_listElementGame[iii]) {
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
  m_dynamicsWorld(NULL),
  m_particuleEngine(*this) {
	// nothing to do ...
}
