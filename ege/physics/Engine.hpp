/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

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
		class Engine {
			private:
				rp3d::DynamicsWorld* m_dynamicsWorld;
				float m_accumulator; // limit call of the step rendering
			public:
				Engine();
				~Engine();
			public:
				// Define a collision point ==> for debug only ...
				//! @not_in_doc
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
				/**
				 * @brief Set the gravity axis of the physic engine
				 * @param[in] _axePower energy of this gravity
				 */
				void setGravity(const vec3& _axePower);
				// update cycle
				void update(float _delta);
				
				void debugDrawWorld() {
					// TODO: later ...
				}
		};
	}
}

