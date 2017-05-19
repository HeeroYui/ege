/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/physics/Component.hpp>
#include <ege/physics/Engine.hpp>
#include <ege/Environement.hpp>
#include <ege/physicsShape/PhysicsShape.hpp>
#include <ege/physicsShape/PhysicsBox.hpp>
#include <ege/physicsShape/PhysicsCapsule.hpp>
#include <ege/physicsShape/PhysicsCone.hpp>
#include <ege/physicsShape/PhysicsConvexHull.hpp>
#include <ege/physicsShape/PhysicsCylinder.hpp>
#include <ege/physicsShape/PhysicsSphere.hpp>

const std::string& ege::physics::Component::getType() const {
	static std::string tmp("physics");
	return tmp;
}

ege::physics::Component::Component(ememory::SharedPtr<ege::Environement> _env) {
	m_engine = ememory::dynamicPointerCast<ege::physics::Engine>(_env->getEngine(getType()));
	// Initial position and orientation of the rigid body
	rp3d::Vector3 initPosition(0.0f, 0.0f, 0.0f);
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Transform transform(initPosition, initOrientation);
	m_rigidBody = m_engine->getDynamicWorld()->createRigidBody(transform);
}

ege::physics::Component::Component(ememory::SharedPtr<ege::Environement> _env, const etk::Transform3D& _transform) {
	m_engine = ememory::dynamicPointerCast<ege::physics::Engine>(_env->getEngine(getType()));
	rp3d::Vector3 initPosition(_transform.getPosition().x(),
	                           _transform.getPosition().y(),
	                           _transform.getPosition().z());
	rp3d::Quaternion initOrientation(_transform.getOrientation().x(),
	                                 _transform.getOrientation().y(),
	                                 _transform.getOrientation().z(),
	                                 _transform.getOrientation().w());
	rp3d::Transform transform(initPosition, initOrientation);
	// Create a rigid body in the world
	m_rigidBody = m_engine->getDynamicWorld()->createRigidBody(transform);
}

ege::physics::Component::~Component() {
	if (m_rigidBody == nullptr) {
		return;
	}
	m_engine->getDynamicWorld()->destroyRigidBody(m_rigidBody);
	m_rigidBody = nullptr;
}

void ege::physics::Component::generate() {
	if (m_shape.size() == 0) {
		EGE_WARNING("No Shape Availlable ...");
		return;
	}
	
	for (auto &it: m_shape) {
		if (it == nullptr) {
			continue;
		}
		switch (it->getType()) {
			case ege::PhysicsShape::box : {
				EGE_ERROR("    Box");
				const ege::PhysicsBox* tmpElement = it->toBox();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Box ==> can not cast in BOX");
					continue;
				}
				// Half extents of the box in the x, y and z directions
				const rp3d::Vector3 halfExtents(tmpElement->getSize().x(),
				                                tmpElement->getSize().y(),
				                                tmpElement->getSize().z());
				// Create the box shape
				rp3d::BoxShape shape(halfExtents);
				rp3d::Vector3 position(tmpElement->getOrigin().x(),
				                       tmpElement->getOrigin().y(),
				                       tmpElement->getOrigin().z());
				rp3d::Quaternion orientation(tmpElement->getQuaternion().x(),
				                             tmpElement->getQuaternion().y(),
				                             tmpElement->getQuaternion().z(),
				                             tmpElement->getQuaternion().w());
				rp3d::Transform transform(position, orientation);
				m_rigidBody->addCollisionShape(&shape, transform, 4.0f /* mass */);
				break;
			}
			case ege::PhysicsShape::cylinder : {
				EGE_DEBUG("    Cylinder");
				const ege::PhysicsCylinder* tmpElement = it->toCylinder();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Cylinder ==> can not cast in Cylinder");
					continue;
				}
				// Create the box shape
				rp3d::CylinderShape shape(tmpElement->getSize().x(), tmpElement->getSize().y());
				rp3d::Vector3 position(tmpElement->getOrigin().x(),
				                       tmpElement->getOrigin().y(),
				                       tmpElement->getOrigin().z());
				rp3d::Quaternion orientation(tmpElement->getQuaternion().x(),
				                             tmpElement->getQuaternion().y(),
				                             tmpElement->getQuaternion().z(),
				                             tmpElement->getQuaternion().w());
				rp3d::Transform transform(position, orientation);
				m_rigidBody->addCollisionShape(&shape, transform, 4.0f /* mass */);
				break;
			}
			case ege::PhysicsShape::capsule : {
				EGE_DEBUG("    Capsule");
				const ege::PhysicsCapsule* tmpElement = it->toCapsule();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Capsule ==> can not cast in Capsule");
					continue;
				}
				/*
				btCollisionShape* tmpShape = new btCapsuleShape(tmpElement->getRadius(), tmpElement->getHeight());
				if (tmpShape != nullptr) {
					if (outputShape == nullptr) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				*/
				break;
			}
			case ege::PhysicsShape::cone : {
				EGE_DEBUG("    Cone");
				const ege::PhysicsCone* tmpElement = it->toCone();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Cone ==> can not cast in Cone");
					continue;
				}
				/*
				btCollisionShape* tmpShape = new btConeShape(tmpElement->getRadius(), tmpElement->getHeight());
				if (tmpShape != nullptr) {
					if (outputShape == nullptr) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				*/
				break;
			}
			case ege::PhysicsShape::sphere : {
				EGE_DEBUG("    Sphere");
				const ege::PhysicsSphere* tmpElement = it->toSphere();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Sphere ==> can not cast in Sphere");
					continue;
				}
				/*
				btCollisionShape* tmpShape = new btSphereShape(tmpElement->getRadius());
				if (tmpShape != nullptr) {
					if (outputShape == nullptr) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				*/
				break;
			}
			case ege::PhysicsShape::convexHull : {
				EGE_DEBUG("    convexHull");
				const ege::PhysicsConvexHull* tmpElement = it->toConvexHull();
				if (tmpElement == nullptr) {
					EGE_ERROR("    convexHull ==> can not cast in convexHull");
					continue;
				}
				/*
				btConvexHullShape* tmpShape = new btConvexHullShape(&(tmpElement->getPointList()[0].x()), tmpElement->getPointList().size());
				if (tmpShape != nullptr) {
					if (outputShape == nullptr) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				*/
				break;
			}
			default :
				EGE_DEBUG("    ???");
				// TODO : UNKNOW type ... 
				break;
		}
	}
}

const std::vector<ememory::SharedPtr<ege::PhysicsShape>>& ege::physics::Component::getShape() const {
	return m_shape;
}

void ege::physics::Component::setShape(const std::vector<ememory::SharedPtr<ege::PhysicsShape>>& _prop) {
	m_shape = _prop;
}

void ege::physics::Component::addShape(const ememory::SharedPtr<ege::PhysicsShape>& _shape) {
	m_shape.push_back(_shape);
}

void ege::physics::Component::setTransform(const etk::Transform3D& _transform) {
	if (m_rigidBody == nullptr) {
		return;
	}
	rp3d::Vector3 position(_transform.getPosition().x(),
	                       _transform.getPosition().y(),
	                       _transform.getPosition().z());
	rp3d::Quaternion orientation(_transform.getOrientation().x(),
	                             _transform.getOrientation().y(),
	                             _transform.getOrientation().z(),
	                             _transform.getOrientation().w());
	rp3d::Transform transform(position, orientation);
	m_rigidBody->setTransform(transform);
}

etk::Transform3D ege::physics::Component::getTransform() const {
	if (m_rigidBody == nullptr) {
		return etk::Transform3D::identity();
	}
	rp3d::Transform transform = m_rigidBody->getTransform();
	vec3 position(transform.getPosition().x,
	              transform.getPosition().y,
	              transform.getPosition().z);
	etk::Quaternion orientation(transform.getOrientation().x,
	                            transform.getOrientation().y,
	                            transform.getOrientation().z,
	                            transform.getOrientation().w);
	return etk::Transform3D(position, orientation);
}

