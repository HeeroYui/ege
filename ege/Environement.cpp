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




ege::ElementGame* ege::Environement::GetElementNearest(ege::ElementGame* _sourceRequest, float& _distance)
{
	if (NULL==_sourceRequest) {
		return NULL;
	}
	vec3 sourcePosition = _sourceRequest->GetPosition();
	ege::ElementGame* result = NULL;
	for (int32_t iii=0; iii<m_listElementGame.Size() ; iii++) {
		// chack NULL  pointer
		if (NULL == m_listElementGame[iii]) {
			continue;
		}
		if (m_listElementGame[iii]->GetGroup()<=0) {
			continue;
		}
		// check if they are in the same group:
		if (m_listElementGame[iii]->GetGroup() == _sourceRequest->GetGroup()) {
			continue;
		}
		// check distance ...
		vec3 destPosition = m_listElementGame[iii]->GetPosition();
		float distance = btDistance(sourcePosition, destPosition);
		//EGE_DEBUG("Distance : " << _distance << " >? " << distance << " id=" << iii);
		if (_distance>distance) {
			_distance = distance;
			result = m_listElementGame[iii];
		}
	}
	return result;
}


void ege::Environement::GetElementNearest(const vec3& _sourcePosition,
                                          float _distanceMax,
                                          etk::Vector<ege::Environement::ResultNearestElement>& _resultList)
{
	_resultList.Clear();
	ege::Environement::ResultNearestElement result;
	result.dist = 99999999999.0f;
	result.element = NULL;
	for (int32_t iii=0; iii<m_listElementGame.Size() ; iii++) {
		// chack NULL  pointer
		result.element = m_listElementGame[iii];
		if (NULL == result.element) {
			continue;
		}
		// check distance ...
		vec3 destPosition = result.element->GetPosition();
		if (_sourcePosition == destPosition) {
			continue;
		}
		result.dist = btDistance(_sourcePosition, destPosition);
		//EGE_DEBUG("Distance : " << _distance << " >? " << distance << " id=" << iii);
		if (_distanceMax>result.dist) {
			_resultList.PushBack(result);
		}
	}
}

void ege::Environement::GetElementNearestFixed(const vec3& _sourcePosition,
                                               float _distanceMax,
                                               etk::Vector<ege::Environement::ResultNearestElement>& _resultList)
{
	_resultList.Clear();
	ege::Environement::ResultNearestElement result;
	result.dist = 99999999999.0f;
	result.element = NULL;
	for (int32_t iii=0; iii<m_listElementGame.Size() ; iii++) {
		// chack NULL  pointer
		result.element = m_listElementGame[iii];
		if (NULL==result.element) {
			continue;
		}
		if (false==result.element->IsFixed()) {
			continue;
		}
		// check distance ...
		vec3 destPosition = result.element->GetPositionTheoric();
		result.dist = btDistance(_sourcePosition, destPosition);
		//EGE_DEBUG("Distance : " << _distance << " >? " << distance << " id=" << iii);
		if (_distanceMax<=result.dist) {
			continue;
		}
		// try to add the element at the best positions:
		int32_t jjj;
		for (jjj=0; jjj<_resultList.Size(); jjj++) {
			if (_resultList[jjj].dist>result.dist) {
				_resultList.Insert(jjj, result);
				break;
			}
		}
		// add element at the end :
		if (jjj>=_resultList.Size()) {
			_resultList.PushBack(result);
		}
	}
}

static etk::Hash<ege::createElement_tf>& GetHachTableCreating(void)
{
	static etk::Hash<ege::createElement_tf> s_table;
	return s_table;
}


void ege::Environement::AddCreator(const etk::UString& _type, ege::createElement_tf _creator)
{
	if (NULL == _creator) {
		EGE_ERROR("Try to add an empty CREATOR ...");
		return;
	}
	GetHachTableCreating().Add(_type, _creator);
}

ege::ElementGame* ege::Environement::CreateElement(const etk::UString& _type, const etk::UString& _description, bool _autoAddElement)
{
	if (false==GetHachTableCreating().Exist(_type)) {
		EGE_ERROR("Request creating of an type that is not known '" << _type << "'");
		return NULL;
	}
	ege::createElement_tf pointerFunction = GetHachTableCreating()[_type];
	if (NULL == pointerFunction) {
		EGE_ERROR("NULL pointer ==> internal error... '" << _type << "'");
		// internal error
		return NULL;
	}
	ege::ElementGame* tmpElement = pointerFunction(*this, _description);
	if (NULL == tmpElement) {
		EGE_ERROR("Sub creator han an error when creating element : '" << _type << "'");
		return NULL;
	}
	if (_autoAddElement==true) {
		AddElementGame(tmpElement);
	}
	return tmpElement;
}


void ege::Environement::AddElementGame(ege::ElementGame* _newElement)
{
	// prevent memory allocation and un allocation ...
	if (NULL == _newElement) {
		return;
	}
	for (int32_t iii=0; iii<m_listElementGame.Size() ; iii++) {
		if (NULL == m_listElementGame[iii]) {
			m_listElementGame[iii] = _newElement;
			m_listElementGame[iii]->DynamicEnable();
			return;
		}
	}
	m_listElementGame.PushBack(_newElement);
	_newElement->DynamicEnable();
}

void ege::Environement::RmElementGame(ege::ElementGame* _removeElement)
{
	if (NULL == _removeElement) {
		return;
	}
	// inform the element that an element has been removed ==> this permit to keep pointer on elements ...
	for (int32_t iii=0; iii<m_listElementGame.Size() ; iii++) {
		if (NULL != m_listElementGame[iii]) {
			m_listElementGame[iii]->ElementIsRemoved(_removeElement);
		}
	}
	// ream remove on the element :
	for (int32_t iii=0; iii<m_listElementGame.Size() ; iii++) {
		if (_removeElement == m_listElementGame[iii]) {
			m_listElementGame[iii]->DynamicDisable();
			delete(m_listElementGame[iii]);
			m_listElementGame[iii] = NULL;
		}
	}
}


void ege::Environement::GetOrderedElementForDisplay(etk::Vector<ege::Environement::ResultNearestElement>& _resultList,
                                                    const vec3& _position,
                                                    const vec3& _direction)
{
	// remove all unneeded elements (old display...)
	_resultList.Clear();
	// basic element result
	ege::Environement::ResultNearestElement result;
	result.dist = 99999999999.0f;
	result.element = NULL;
	// for all element in the game we chek if it is needed to display it ...
	for (int32_t iii=0; iii<m_listElementGame.Size() ; iii++) {
		// chack NULL  pointer
		if (NULL==m_listElementGame[iii]) {
			// no pointer null are set in the output list ...
			continue;
		}
		result.element = m_listElementGame[iii];
		// check distance ...
		vec3 destPosition = result.element->GetPosition();
		vec3 angleView = (destPosition - _position).normalized();
		float dotResult=angleView.dot(_direction);
		//EGE_DEBUG("Dot position : " << destPosition << " ==> dot=" << dotResult);
		if (dotResult<=0.85f) {
			// they are not in the camera angle view ... ==> no need to process display
			continue;
		}
		result.dist = btDistance(_position, destPosition);
		if (result.dist>500.0f) {
			// The element is realy too far ... ==> no need to display
			continue;
		}
		// try to add the element at the best positions:
		int32_t jjj;
		for (jjj=0; jjj<_resultList.Size(); jjj++) {
			if (_resultList[jjj].dist>result.dist) {
				_resultList.Insert(jjj, result);
				break;
			}
		}
		// add element at the end :
		if (jjj>=_resultList.Size()) {
			_resultList.PushBack(result);
		}
	}
}


void ege::Environement::GenerateInteraction(ege::ElementInteraction& _event)
{
	// inform the element that an element has been removed ==> this permit to keep pointer on elements ...
	for (int32_t iii=0; iii<m_listElementGame.Size() ; iii++) {
		if (NULL == m_listElementGame[iii]) {
			continue;
		}
		_event.ApplyEvent(*m_listElementGame[iii]);
		/*
		vec3 destPosition = m_listElementGame[iii]->GetPosition();
		float dist = btDistance(sourcePosition, destPosition);
		if (dist==0 || dist>decreasePower) {
			continue;
		}
		float inpact = (decreasePower-dist)/decreasePower * power;
		g_listElementGame[iii]->SetFireOn(groupIdSource, type, -inpact, sourcePosition);
		*/
	}
}
