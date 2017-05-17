/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/position/Component.hpp>

const std::string& ege::position::Component::getType() const {
	static std::string tmp("position");
	return tmp;
}


ege::position::Component::Component(const etk::Transform3D& _transform):
  m_transform(_transform) {
	
}

ege::position::Component::Component():
  m_transform(etk::Transform3D::identity()) {
	
}

void ege::position::Component::setTransform(const etk::Transform3D& _transform) {
	if (_transform == m_transform) {
		return;
	}
	m_transform = _transform;
	signalPosition.emit(m_transform);
}

const etk::Transform3D& ege::position::Component::getTransform() const {
	return m_transform;
}


void ege::position::Component::addFriendComponent(const ememory::SharedPtr<ege::Component>& _component) {
	if (_component->getType() == "physics") {
		EGE_ERROR("Can not add a 'physic' component and a 'position' component ... ==> incompatible");
	}
}

