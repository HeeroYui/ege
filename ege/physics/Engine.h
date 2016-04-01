/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
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
				std::shared_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
				std::shared_ptr<btCollisionDispatcher> m_dispatcher;
				std::shared_ptr<btBroadphaseInterface> m_broadphase;
				std::shared_ptr<btConstraintSolver> m_solver;
				std::shared_ptr<btDynamicsWorld> m_dynamicsWorld;
			public:
				Engine();
				~Engine();
				void setBulletConfig(std::shared_ptr<btDefaultCollisionConfiguration> _collisionConfiguration=nullptr,
				                     std::shared_ptr<btCollisionDispatcher> _dispatcher=nullptr,
				                     std::shared_ptr<btBroadphaseInterface> _broadphase=nullptr,
				                     std::shared_ptr<btConstraintSolver> _solver=nullptr,
				                     std::shared_ptr<btDynamicsWorld> _dynamicsWorld=nullptr);
				/**
				 * @brief set the curent world
				 * @param[in] _newWorld Pointer on the current world
				 */
				void setDynamicWorld(const std::shared_ptr<btDynamicsWorld>& _newWorld) {
					m_dynamicsWorld=_newWorld;
				};
				/**
				 * @brief get the curent world
				 * @return pointer on the current world
				 */
				std::shared_ptr<btDynamicsWorld> getDynamicWorld() {
					return m_dynamicsWorld;
				};
			public:
				// Define a collision point ==> for debug only ...
				//! @not_in_doc
				class collisionPoints {
					public:
						std::shared_ptr<ege::Element> elem1;
						std::shared_ptr<ege::Element> elem2;
						vec3 positionElem1;
						vec3 positionElem2;
						vec3 normalElem2;
						collisionPoints(const std::shared_ptr<ege::Element>& _elem1,
						                const std::shared_ptr<ege::Element>& _elem2,
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

