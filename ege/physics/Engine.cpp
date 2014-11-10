/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/physics/Engine.h>


#include <ewol/openGL/openGL.h>
#include <etk/math/Matrix4.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <LinearMath/btIDebugDraw.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btConvexPolyhedron.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <LinearMath/btTransformUtil.h>
#include <LinearMath/btIDebugDraw.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>


ege::physics::Engine::Engine() {
	setBulletConfig();
}

ege::physics::Engine::~Engine() {
	/*
	m_dynamicsWorld.release();
	m_solver.release();
	m_broadphase.release();
	m_dispatcher.release();
	m_collisionConfiguration.release();
	*/
}

void ege::physics::Engine::setBulletConfig(std::shared_ptr<btDefaultCollisionConfiguration> _collisionConfiguration,
                                           std::shared_ptr<btCollisionDispatcher> _dispatcher,
                                           std::shared_ptr<btBroadphaseInterface> _broadphase,
                                           std::shared_ptr<btConstraintSolver> _solver,
                                           std::shared_ptr<btDynamicsWorld> _dynamicsWorld) {
	if (_collisionConfiguration != nullptr) {
		m_collisionConfiguration = _collisionConfiguration;
	} else {
		m_collisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
	}
	///use the default collision dispatcher.
	if (_dispatcher != nullptr) {
		m_dispatcher = _dispatcher;
	} else {
		m_dispatcher = std::make_shared<btCollisionDispatcher>(m_collisionConfiguration.get());
	}
	if (_broadphase != nullptr) {
		m_broadphase = _broadphase;
	} else {
		m_broadphase = std::make_shared<btDbvtBroadphase>();
	}
	
	///the default constraint solver.
	if (_solver != nullptr) {
		m_solver = _solver;
	} else {
		m_solver = std::make_shared<btSequentialImpulseConstraintSolver>();
	}
	
	if (_dynamicsWorld != nullptr) {
		m_dynamicsWorld = _dynamicsWorld;
	} else {
		m_dynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(m_dispatcher.get(),m_broadphase.get(),m_solver.get(),m_collisionConfiguration.get());
		// By default we set no gravity
		m_dynamicsWorld->setGravity(btVector3(0,0,0));
	}
	
	//m_env.setDynamicWorld(m_dynamicsWorld);
	
}