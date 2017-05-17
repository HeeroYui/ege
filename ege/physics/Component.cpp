/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/physics/Component.hpp>

const std::string& ege::physics::Component::getType() const {
	static std::string tmp("physics");
	return tmp;
}


void ege::physics::Component::setTransform(const etk::Transform3D& _transform) {
	/*
	if (_transform == m_transform) {
		return;
	}
	m_transform = _transform;
	signalPosition.emit(m_transform);
	*/
}

const etk::Transform3D& ege::physics::Component::getTransform() const {
	/*
	return m_transform;
	*/
	return etk::Transform3D::identity();
}

