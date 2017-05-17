/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/physics/Engine.hpp>

#include <ege/debug.hpp>

#include <gale/renderer/openGL/openGL.hpp>
#include <etk/math/Matrix4x4.hpp>

#include <ege/elements/ElementPhysic.hpp>


// unique callback function :
/*
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
		elem0->onCollisionDetected(elem1->sharedFromThis(), _point.getPositionWorldOnA(), -_point.m_normalWorldOnB);
	}
	if (elem1->getCollisionDetectionStatus() == true) {
		elem1->onCollisionDetected(elem0->sharedFromThis(), _point.getPositionWorldOnA(), _point.m_normalWorldOnB);
	}
	return true;
}
*/

ege::physics::Engine::Engine():
  m_dynamicsWorld(nullptr),
  m_accumulator(0.0f) {
	// Start engine with no gravity
	//rp3d::Vector3 gravity(0.0, -9.81, 0.0); // generic earth gravity
	rp3d::Vector3 gravity(0.0f, 0.0f, 0.0f);
	// Create the dynamics world
	m_dynamicsWorld = new rp3d::DynamicsWorld(gravity);
	if (m_dynamicsWorld != nullptr) {
		// Set the number of iterations of the constraint solver
		m_dynamicsWorld->setNbIterationsVelocitySolver(15);
	}
}

ege::physics::Engine::~Engine() {
	if (m_dynamicsWorld != nullptr) {
		delete m_dynamicsWorld;
		m_dynamicsWorld = nullptr;
	}
}

void ege::physics::Engine::setGravity(const vec3& _axePower) {
	if (m_dynamicsWorld != nullptr) {
		rp3d::Vector3 gravity(_axePower.x(), _axePower.y(), _axePower.z());
		m_dynamicsWorld->setGravity(gravity);
	}
}

// Constant physics time step
const float timeStep = 1.0 / 60.0;

void ege::physics::Engine::update(float _delta) {
	// Add the time difference in the accumulator
	m_accumulator += _delta;
	// While there is enough accumulated time to take one or several physics steps
	while (m_accumulator >= timeStep) {
		if (m_dynamicsWorld != nullptr) {
			// Update the Dynamics world with a constant time step
			EGE_WARNING("Update the Physic engine ... " << timeStep);
			m_dynamicsWorld->update(timeStep);
		}
		// Decrease the accumulated time
		m_accumulator -= timeStep;
	}
}

std::vector<ege::physics::Engine::collisionPoints> ege::physics::Engine::getListOfCollision() {
	std::vector<collisionPoints> out;
	/*
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
					out.push_back(collisionPoints(elem0->sharedFromThis(), elem1->sharedFromThis(), pt.getPositionWorldOnA(), pt.getPositionWorldOnB(), pt.m_normalWorldOnB));
				}
			}
		}
	}
	*/
	return out;
}