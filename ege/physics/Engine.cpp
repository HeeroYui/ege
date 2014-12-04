/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/physics/Engine.h>

#include <ege/debug.h>

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

#include <ege/elements/ElementPhysic.h>


// unique callback function :
extern ContactProcessedCallback gContactProcessedCallback;

// TODO : remove double collision call ...
static bool handleContactsProcess(btManifoldPoint& _point, btCollisionObject* _body0, btCollisionObject* _body1) {
	ege::ElementPhysic* elem0 = static_cast<ege::ElementPhysic*>(_body0->getUserPointer());
	ege::ElementPhysic* elem1 = static_cast<ege::ElementPhysic*>(_body1->getUserPointer());
	if (    elem0 == nullptr
	     || elem1 == nullptr) {
		EGE_WARNING("callback of collision error");
		return false;
	}
	EGE_VERBOSE("collision process between " << elem0->getUID() << " && " << elem1->getUID() << " pos=" << _point.getPositionWorldOnA() << " norm=" << _point.m_normalWorldOnB);
	if (elem0->getCollisionDetectionStatus() == true) {
		elem0->onCollisionDetected(elem1->shared_from_this(), _point.getPositionWorldOnA(), -_point.m_normalWorldOnB);
	}
	if (elem1->getCollisionDetectionStatus() == true) {
		elem1->onCollisionDetected(elem0->shared_from_this(), _point.getPositionWorldOnA(), _point.m_normalWorldOnB);
	}
	return true;
}


ege::physics::Engine::Engine() {
	setBulletConfig();
	// set callback for collisions ...
	gContactProcessedCallback = (ContactProcessedCallback)handleContactsProcess;
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

// some doccumantation : http://www.bulletphysics.org/mediawiki-1.5.8/index.php?title=Collision_Callbacks_and_Triggers
std::vector<ege::physics::Engine::collisionPoints> ege::physics::Engine::getListOfCollision() {
	std::vector<collisionPoints> out;
	if (m_dynamicsWorld != nullptr) {
		int32_t numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
		for (int i=0;i<numManifolds;i++) {
			btPersistentManifold* contactManifold = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
			const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());
			if (    obA == nullptr
			     || obB == nullptr) {
				continue;
			}
			ege::ElementPhysic* elem0 = static_cast<ege::ElementPhysic*>(obA->getUserPointer());
			ege::ElementPhysic* elem1 = static_cast<ege::ElementPhysic*>(obB->getUserPointer());
			if (    elem0 == nullptr
			     || elem1 == nullptr) {
				continue;
			}
			int numContacts = contactManifold->getNumContacts();
			for (int j=0;j<numContacts;j++) {
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getDistance()<0.f) {
					out.push_back(collisionPoints(elem0->shared_from_this(), elem1->shared_from_this(), pt.getPositionWorldOnA(), pt.getPositionWorldOnB(), pt.m_normalWorldOnB));
				}
			}
		}
	}
	return out;
}