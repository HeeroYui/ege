/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/Entity.hpp>
#include <ege/physics/Engine.hpp>

#include <ege/debug.hpp>

#include <gale/renderer/openGL/openGL.hpp>
#include <etk/math/Matrix4x4.hpp>

const std::string& ege::physics::Engine::getType() const {
	static std::string tmp("physics");
	return tmp;
}

void ege::physics::Engine::beginContact(const rp3d::ContactPointInfo& _contact) {
	// Called when a new contact point is found between two bodies that were separated before.
	EGE_WARNING("collision detection [BEGIN] " << vec3(_contact.normal.x, _contact.normal.y, _contact.normal.z) << " depth=" << _contact.penetrationDepth);
}

void ege::physics::Engine::newContact(const rp3d::ContactPointInfo& _contact) {
	//Called when a new contact point is found between two bodies.
	EGE_WARNING("collision detection [ NEW ] " << vec3(_contact.normal.x, _contact.normal.y, _contact.normal.z) << " depth=" << _contact.penetrationDepth);
}

void ege::physics::Engine::componentRemove(const ememory::SharedPtr<ege::Component>& _ref) {
	ememory::SharedPtr<ege::physics::Component> ref = ememory::dynamicPointerCast<ege::physics::Component>(_ref);
	for (auto it=m_component.begin();
	     it != m_component.end();
	     ++it) {
		if (*it == ref) {
			it->reset();
			return;
		}
	}
}

void ege::physics::Engine::componentAdd(const ememory::SharedPtr<ege::Component>& _ref) {
	ememory::SharedPtr<ege::physics::Component> ref = ememory::dynamicPointerCast<ege::physics::Component>(_ref);
	for (auto it=m_component.begin();
	     it != m_component.end();
	     ++it) {
		if (*it == nullptr) {
			*it = ref;
			return;
		}
	}
	m_component.push_back(ref);
}

ege::physics::Engine::Engine(ege::Environement* _env) :
  ege::Engine(_env),
  propertyDebugAABB(this, "debug-AABB", false, "display the global AABB box of every shape"),
  propertyDebugShape(this, "debug-shape", false, "display the physic shape"),
  m_dynamicsWorld(nullptr),
  m_accumulator(0.0f) {
	m_debugDrawProperty = ewol::resource::Colored3DObject::create();
	// Start engine with no gravity
	//rp3d::Vector3 gravity(0.0, -9.81, 0.0); // generic earth gravity
	rp3d::Vector3 gravity(0.0f, 0.0f, 0.0f);
	// Create the dynamics world
	m_dynamicsWorld = new rp3d::DynamicsWorld(gravity);
	if (m_dynamicsWorld != nullptr) {
		// Set the number of iterations of the constraint solver
		m_dynamicsWorld->setNbIterationsVelocitySolver(15);
		m_dynamicsWorld->setEventListener(this);
	}
}

ege::physics::Engine::~Engine() {
	if (m_dynamicsWorld != nullptr) {
		m_dynamicsWorld->setEventListener(nullptr);
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

void ege::physics::Engine::update(const echrono::Duration& _delta) {
	float deltaTime = _delta.toSeconds();
	// Add the time difference in the accumulator
	m_accumulator += deltaTime;
	// While there is enough accumulated time to take one or several physics steps
	while (m_accumulator >= timeStep) {
		if (m_dynamicsWorld != nullptr) {
			// Update the Dynamics world with a constant time step
			EGE_DEBUG("Update the Physic engine ... " << timeStep);
			m_dynamicsWorld->update(timeStep);
		}
		// Decrease the accumulated time
		m_accumulator -= timeStep;
	}
	for (auto &it: m_component) {
		// check nullptr  pointer
		if (it == nullptr) {
			// no pointer null are set in the output list ...
			continue;
		}
		it->emitAll();
	}
}

void ege::physics::Engine::renderDebug(const echrono::Duration& _delta, const ememory::SharedPtr<ege::Camera>& _camera) {
	if (propertyDebugShape.get() == true) {
		for (auto &it : m_component) {
			if (it == nullptr) {
				continue;
			}
			it->drawShape(m_debugDrawProperty, _camera);
		}
	}
	if (propertyDebugAABB.get() == true) {
		for (auto &it : m_component) {
			if (it == nullptr) {
				continue;
			}
			it->drawAABB(m_debugDrawProperty, _camera);
		}
	}
}

