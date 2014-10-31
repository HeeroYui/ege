/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_PHYSICS_ENGINE_H__
#define __EGE_PHYSICS_ENGINE_H__

#include <etk/types.h>
#include <etk/math/Vector3D.h>
#include <etk/math/Matrix4.h>
#include <vector>
#include <ewol/debug.h>
#include <ege/Camera.h>
#include <ewol/widget/Widget.h>
#include <ewol/openGL/openGL.h>
#include <ewol/resource/Manager.h>
#include <ege/ElementGame.h>
#include <ewol/Dimension.h>

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


namespace ege {
	namespace physics {
		class Engine {
			private:
				///this is the most important class
				std::shared_ptr<btDefaultCollisionConfiguration< m_collisionConfiguration;
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
		};
	};
};

#endif
