/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

namespace ege {
	namespace physics {
		class Engine;
	};
};
#include <etk/types.h>
#include <etk/math/Vector3D.h>
#include <etk/math/Matrix4.h>
#include <vector>
#include <ewol/debug.h>
#include <ege/camera/Camera.h>
#include <ewol/widget/Widget.h>
#include <gale/renderer/openGL/openGL.h>
#include <gale/resource/Manager.h>
#include <gale/Dimension.h>


class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;
class btDynamicsWorld;
#include <LinearMath/btScalar.h>
class btVector3;


//#include <ege/elements/Element.h>

namespace ege {
	namespace physics {
		class Engine {
			private:
				///this is the most important class
				ememory::SharedPtr<btDefaultCollisionConfiguration> m_collisionConfiguration;
				ememory::SharedPtr<btCollisionDispatcher> m_dispatcher;
				ememory::SharedPtr<btBroadphaseInterface> m_broadphase;
				ememory::SharedPtr<btConstraintSolver> m_solver;
				ememory::SharedPtr<btDynamicsWorld> m_dynamicsWorld;
			public:
				Engine();
				~Engine();
				void setBulletConfig(ememory::SharedPtr<btDefaultCollisionConfiguration> _collisionConfiguration=nullptr,
				                     ememory::SharedPtr<btCollisionDispatcher> _dispatcher=nullptr,
				                     ememory::SharedPtr<btBroadphaseInterface> _broadphase=nullptr,
				                     ememory::SharedPtr<btConstraintSolver> _solver=nullptr,
				                     ememory::SharedPtr<btDynamicsWorld> _dynamicsWorld=nullptr);
				/**
				 * @brief set the curent world
				 * @param[in] _newWorld Pointer on the current world
				 */
				void setDynamicWorld(const ememory::SharedPtr<btDynamicsWorld>& _newWorld) {
					m_dynamicsWorld=_newWorld;
				};
				/**
				 * @brief get the curent world
				 * @return pointer on the current world
				 */
				ememory::SharedPtr<btDynamicsWorld> getDynamicWorld() {
					return m_dynamicsWorld;
				};
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
		};
	}
}

