/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ege/Engine.hpp>

namespace ege {
	namespace physics {
		class Engine;
	};
};
#include <etk/types.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Matrix4x4.hpp>
#include <vector>
#include <ewol/debug.hpp>
#include <ege/camera/Camera.hpp>
#include <ewol/widget/Widget.hpp>
#include <gale/renderer/openGL/openGL.hpp>
#include <gale/resource/Manager.hpp>
#include <gale/Dimension.hpp>
#include <ephysics/reactphysics3d.h>


namespace ege {
	namespace physics {
		class Engine : public ege::Engine {
			private:
				rp3d::DynamicsWorld* m_dynamicsWorld;
				float m_accumulator; // limit call of the step rendering
			public:
				Engine(ege::Environement* _env);
				~Engine();
			public:
				// Define a collision point ==> for debug only ...
				//! @not_in_doc
#if 0
				class collisionPoints {
					public:
						ememory::SharedPtr<ege::Element> elem1;
						ememory::SharedPtr<ege::Element> elem2;
						vec3 positionElem1;
						vec3 positionElem2;
						vec3 normalElem2;
						collisionPoints(const ememory::SharedPtr<ege::Element>& _elem1,
						                const ememory::SharedPtr<ege::Element>& _elem2,
						                const vec3& _pos1,
						                const vec3& _pos2,
						                const vec3& _normal) :
						  elem1(_elem1),
						  elem2(_elem2),
						  positionElem1(_pos1),
						  positionElem2(_pos2),
						  normalElem2(_normal) { }
				};
				/**
				 * @brief Get the list of all collision point actually availlable ...
				 * @return the requested list of points
				 */
				std::vector<ege::physics::Engine::collisionPoints> getListOfCollision();
#endif
				/**
				 * @brief Set the gravity axis of the physic engine
				 * @param[in] _axePower energy of this gravity
				 */
				void setGravity(const vec3& _axePower);
				
				void debugDrawWorld() {
					// TODO: later ...
				}
				rp3d::DynamicsWorld* getDynamicWorld() {
					return m_dynamicsWorld;
				}
			public:
				const std::string& getType() const override;
				void componentRemove(const ememory::SharedPtr<ege::Component>& _ref) override;
				void componentAdd(const ememory::SharedPtr<ege::Component>& _ref) override;
				void update(const echrono::Duration& _delta) override;
				//void render(const echrono::Duration& _delta, const ememory::SharedPtr<ege::Camera>& _camera) override;
		};
	}
}

