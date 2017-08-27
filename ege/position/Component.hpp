/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ege/debug.hpp>

#include <ege/Component.hpp>
#include <etk/math/Transform3D.hpp>
#include <esignal/Signal.hpp>

namespace ege {
	namespace position {
		class Component : public ege::Component {
			public:
				esignal::Signal<etk::Transform3D> signalPosition;
			protected:
				etk::Transform3D m_transform;
			public:
				/**
				 * @brief Create a basic position component (no orientation and position (0,0,0))
				 */
				Component();
				/**
				 * @brief Create a basic position component
				 * @param[in] _transform transformation of the position
				 */
				Component(const etk::Transform3D& _transform);
				/**
				 * @brief set a new transformation
				 * @param[in] _transform transformation of the position
				 */
				void setTransform(const etk::Transform3D& _transform);
				/**
				 * @brief set a new transformation
				 * @return Transformation of the position
				 */
				const etk::Transform3D& getTransform() const;
			public:
				const etk::String& getType() const override;
				void addFriendComponent(const ememory::SharedPtr<ege::Component>& _component) override;
		};
	}
}