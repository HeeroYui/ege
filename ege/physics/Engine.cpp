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
	ege::physics::Component* component1 = nullptr;
	ege::physics::Component* component2 = nullptr;
	vec3 normal(_contact.normal.x, _contact.normal.y, _contact.normal.z);
	vec3 pos1(_contact.localPoint1.x, _contact.localPoint1.y, _contact.localPoint1.z);
	vec3 pos2(_contact.localPoint2.x, _contact.localPoint2.y, _contact.localPoint2.z);
	float penetrationDepth = _contact.penetrationDepth;
	// Called when a new contact point is found between two bodies that were separated before.
	EGE_WARNING("collision detection [BEGIN] " << pos1 << " depth=" << penetrationDepth);
	if (    _contact.shape1 != nullptr
	     && _contact.shape1->getUserData() != nullptr) {
		component1 = static_cast<ege::physics::Component*>(_contact.shape1->getUserData());
	}
	if (    _contact.shape2 != nullptr
	     && _contact.shape2->getUserData() != nullptr) {
		component2 = static_cast<ege::physics::Component*>(_contact.shape2->getUserData());
	}
	if (component1 != nullptr) {
		component1->beginContact(component2, normal, pos1, pos2, penetrationDepth);
	}
	if (component2 != nullptr) {
		component2->beginContact(component1, -normal, pos2, pos1, penetrationDepth);
	}
}

void ege::physics::Engine::newContact(const rp3d::ContactPointInfo& _contact) {
	ege::physics::Component* component1 = nullptr;
	ege::physics::Component* component2 = nullptr;
	vec3 normal(_contact.normal.x, _contact.normal.y, _contact.normal.z);
	vec3 pos1(_contact.localPoint1.x, _contact.localPoint1.y, _contact.localPoint1.z);
	vec3 pos2(_contact.localPoint2.x, _contact.localPoint2.y, _contact.localPoint2.z);
	float penetrationDepth = _contact.penetrationDepth;
	//Called when a new contact point is found between two bodies.
	EGE_WARNING("collision detection [ NEW ] " << pos1 << " depth=" << penetrationDepth);
	if (    _contact.shape1 != nullptr
	     && _contact.shape1->getUserData() != nullptr) {
		component1 = static_cast<ege::physics::Component*>(_contact.shape1->getUserData());
	}
	if (    _contact.shape2 != nullptr
	     && _contact.shape2->getUserData() != nullptr) {
		component2 = static_cast<ege::physics::Component*>(_contact.shape2->getUserData());
	}
	if (component1 != nullptr) {
		component1->newContact(component2, normal, pos1, pos2, penetrationDepth);
	}
	if (component2 != nullptr) {
		component2->newContact(component1, -normal, pos2, pos1, penetrationDepth);
	}
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

class MyCallbackClass : public rp3d::RaycastCallback {
	public:
		vec3 m_position;
		vec3 m_normal;
		bool m_haveImpact;
		rp3d::CollisionBody* m_body;
		MyCallbackClass():
		  m_haveImpact(false),
		  m_body(nullptr) {
			
		}
	public:
		virtual float notifyRaycastHit(const rp3d::RaycastInfo& _info) {
			m_haveImpact = true;
			// Display the world hit point coordinates
			m_position = vec3(_info.worldPoint.x, _info.worldPoint.y, _info.worldPoint.z);
			m_normal = vec3(_info.worldNormal.x, _info.worldNormal.y, _info.worldNormal.z);
			m_body = _info.body;
			EGE_WARNING("Hit point: " << m_position);
			// Return a fraction of 1.0 to gather all hits
			return 1.0f;
		}
};

std::pair<vec3,vec3> ege::physics::Engine::testRay(const ege::Ray& _ray) {
	vec3 start = _ray.getOrigin();
	vec3 stop = _ray.getOrigin()+_ray.getDirection()*1000.0f;
	// Start and End are vectors
	// Create the ray
	rp3d::Vector3 startPoint(start.x(), start.y(), start.z());
	rp3d::Vector3 endPoint(stop.x(), stop.y(), stop.z()); 
	rp3d::Ray ray(startPoint, endPoint);
	// Create an instance of your callback class
	MyCallbackClass callbackObject;
	// Raycast test
	m_dynamicsWorld->raycast(ray, &callbackObject);
	if (callbackObject.m_haveImpact == true) {
		return std::pair<vec3,vec3>(callbackObject.m_position, callbackObject.m_normal);
	}
	EGE_VERBOSE("    No Hit");
	return std::pair<vec3,vec3>(vec3(0,0,0),vec3(0,0,0));
}

std::pair<ememory::SharedPtr<ege::Component>, std::pair<vec3,vec3>> ege::physics::Engine::testRayObject(const ege::Ray& _ray) {
	vec3 start = _ray.getOrigin();
	vec3 stop = _ray.getOrigin()+_ray.getDirection()*1000.0f;
	// Start and End are vectors
	// Create the ray
	rp3d::Vector3 startPoint(start.x(), start.y(), start.z());
	rp3d::Vector3 endPoint(stop.x(), stop.y(), stop.z());
	rp3d::Ray ray(startPoint, endPoint);
	// Create an instance of your callback class
	MyCallbackClass callbackObject;
	// Raycast test
	m_dynamicsWorld->raycast(ray, &callbackObject);
	if (callbackObject.m_haveImpact == true) {
		if (    callbackObject.m_body == nullptr
		     || callbackObject.m_body->getUserData() == nullptr) {
			std::pair<ememory::SharedPtr<ege::Component>, std::pair<vec3,vec3>>(nullptr, std::pair<vec3,vec3>(callbackObject.m_position, callbackObject.m_normal));
		}
		// TODO: je n'ai pas une entity, main un component ...
		ege::physics::Component* elem = static_cast<ege::physics::Component*>(callbackObject.m_body->getUserData());
		return std::pair<ememory::SharedPtr<ege::Component>, std::pair<vec3,vec3>>(elem->sharedFromThis(), std::pair<vec3,vec3>(callbackObject.m_position, callbackObject.m_normal));
	}
	EGE_VERBOSE("    No Hit");
	return std::pair<ememory::SharedPtr<ege::Component>, std::pair<vec3,vec3>>(nullptr, std::pair<vec3,vec3>(vec3(0,0,0),vec3(0,0,0)));
	
}