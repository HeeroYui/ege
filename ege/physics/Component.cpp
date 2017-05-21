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
	m_lastTransformEmit = etk::Transform3D(vec3(0,0,0), etk::Quaternion::identity());
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
	m_lastTransformEmit = _transform;
}

void ege::physics::Component::setType(enum ege::physics::Component::type _type) {
	if (m_rigidBody == nullptr) {
		return;
	}
	switch(_type) {
		case ege::physics::Component::type::bodyStatic:
			m_rigidBody->setType(rp3d::STATIC);
			break;
		case ege::physics::Component::type::bodyKinematic:
			m_rigidBody->setType(rp3d::KINEMATIC);
			break;
		case ege::physics::Component::type::bodyDynamic:
			m_rigidBody->setType(rp3d::DYNAMIC);
			break;
	}
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
				EGE_DEBUG("    Box");
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
				rp3d::BoxShape* shape = new rp3d::BoxShape(halfExtents);
				m_listShape.push_back(shape);
				rp3d::Vector3 position(it->getOrigin().x(),
				                       it->getOrigin().y(),
				                       it->getOrigin().z());
				rp3d::Quaternion orientation(it->getQuaternion().x(),
				                             it->getQuaternion().y(),
				                             it->getQuaternion().z(),
				                             it->getQuaternion().w());
				rp3d::Transform transform(position, orientation);
				rp3d::ProxyShape* proxyShape = m_rigidBody->addCollisionShape(shape, transform, it->getMass());
				m_listProxyShape.push_back(proxyShape);
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
				rp3d::CylinderShape* shape = new rp3d::CylinderShape(tmpElement->getSize().x(), tmpElement->getSize().y());
				rp3d::Vector3 position(it->getOrigin().x(),
				                       it->getOrigin().y(),
				                       it->getOrigin().z());
				rp3d::Quaternion orientation(it->getQuaternion().x(),
				                             it->getQuaternion().y(),
				                             it->getQuaternion().z(),
				                             it->getQuaternion().w());
				rp3d::Transform transform(position, orientation);
				rp3d::ProxyShape* proxyShape = m_rigidBody->addCollisionShape(shape, transform, it->getMass());
				m_listProxyShape.push_back(proxyShape);
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


void ege::physics::Component::emitAll() {
	// emit onbly of new ...
	etk::Transform3D transform = getTransform();
	if (m_lastTransformEmit != transform) {
		m_lastTransformEmit = transform;
		signalPosition.emit(transform);
	}
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

vec3 ege::physics::Component::getLinearVelocity() const {
	if (m_rigidBody == nullptr) {
		return vec3(0,0,0);
	}
	rp3d::Vector3 value = m_rigidBody->getLinearVelocity();
	return vec3(value.x,
	            value.y,
	            value.z);
}

void ege::physics::Component::setLinearVelocity(const vec3& _linearVelocity) {
	if (m_rigidBody == nullptr) {
		return;
	}
	rp3d::Vector3 value(_linearVelocity.x(),
	                    _linearVelocity.y(),
	                    _linearVelocity.z());
	m_rigidBody->setLinearVelocity(value);
}

vec3 ege::physics::Component::getAngularVelocity() const {
	if (m_rigidBody == nullptr) {
		return vec3(0,0,0);
	}
	rp3d::Vector3 value = m_rigidBody->getAngularVelocity();
	return vec3(value.x,
	            value.y,
	            value.z);
}
void ege::physics::Component::setAngularVelocity(const vec3& _angularVelocity) {
	if (m_rigidBody == nullptr) {
		return;
	}
	rp3d::Vector3 value(_angularVelocity.x(),
	                    _angularVelocity.y(),
	                    _angularVelocity.z());
	m_rigidBody->setAngularVelocity(value);
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


void ege::physics::Component::drawShape(ememory::SharedPtr<ewol::resource::Colored3DObject> _draw, ememory::SharedPtr<ege::Camera> _camera) {
	etk::Transform3D transform = getTransform();
	float mmm[16];
	// Get the OpenGL matrix array of the transform 
	transform.getOpenGLMatrix(mmm);
	mat4 transformationMatrix(mmm);
	transformationMatrix.transpose();
	etk::Color<float> tmpColor(1.0, 0.0, 0.0, 0.3);
	for (auto &it: m_shape) {
		if (it == nullptr) {
			continue;
		}
		switch (it->getType()) {
			case ege::PhysicsShape::box: {
				EGE_DEBUG("    Box");
				const ege::PhysicsBox* tmpElement = it->toBox();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Box ==> can not cast in BOX");
					continue;
				}
				etk::Transform3D transformLocal(it->getOrigin(), it->getOrientation());
				transformLocal.getOpenGLMatrix(mmm);
				mat4 transformationMatrixLocal(mmm);
				transformationMatrixLocal.transpose();
				transformationMatrixLocal = transformationMatrix * transformationMatrixLocal;
				_draw->drawSquare(tmpElement->getSize(), transformationMatrixLocal, tmpColor);
				break;
			}
			case ege::PhysicsShape::cylinder : {
				EGE_DEBUG("    Cylinder");
				const ege::PhysicsCylinder* tmpElement = it->toCylinder();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Cylinder ==> can not cast in Cylinder");
					continue;
				}
				etk::Transform3D transformLocal(it->getOrigin(), it->getOrientation());
				transformLocal.getOpenGLMatrix(mmm);
				mat4 transformationMatrixLocal(mmm);
				transformationMatrixLocal.transpose();
				transformationMatrixLocal = transformationMatrix * transformationMatrixLocal;
				//_draw->drawSphere(radius, 10, 10, _transformationMatrix, tmpColor);
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