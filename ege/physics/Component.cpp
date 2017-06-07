/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/physics/Component.hpp>
#include <ege/physics/Engine.hpp>
#include <ege/Environement.hpp>
#include <ege/physics/shape/Shape.hpp>
#include <ege/physics/shape/Box.hpp>
#include <ege/physics/shape/Capsule.hpp>
#include <ege/physics/shape/Cone.hpp>
#include <ege/physics/shape/ConvexHull.hpp>
#include <ege/physics/shape/Cylinder.hpp>
#include <ege/physics/shape/Sphere.hpp>
#include <ege/physics/shape/Concave.hpp>
#include <ephysics/collision/shapes/ConcaveShape.h>
#include <ephysics/collision/shapes/ConcaveMeshShape.h>

const std::string& ege::physics::Component::getType() const {
	static std::string tmp("physics");
	return tmp;
}

void ege::physics::Component::beginContact(ege::physics::Component* _other, const vec3& _normal, const vec3& _pos, const vec3& _posOther, float _penetrationDepth) {
	EGE_WARNING("    collision [BEGIN] " << _pos << " depth=" << _penetrationDepth);
}

void ege::physics::Component::newContact(ege::physics::Component* _other, const vec3& _normal, const vec3& _pos, const vec3& _posOther, float _penetrationDepth) {
	EGE_WARNING("    collision [ NEW ] " << _pos << " depth=" << _penetrationDepth);
}

ege::physics::Component::Component(ememory::SharedPtr<ege::Environement> _env):
  m_staticForceApplyCenterOfMass(0,0,0),
  m_staticTorqueApply(0,0,0) {
	m_engine = ememory::dynamicPointerCast<ege::physics::Engine>(_env->getEngine(getType()));
	// Initial position and orientation of the rigid body
	rp3d::Vector3 initPosition(0.0f, 0.0f, 0.0f);
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Transform transform(initPosition, initOrientation);
	m_lastTransformEmit = etk::Transform3D(vec3(0,0,0), etk::Quaternion::identity());
	m_rigidBody = m_engine->getDynamicWorld()->createRigidBody(transform);
	m_rigidBody->setUserData(this);
	// set collision callback:
	//m_engine->getDynamicWorld()->testCollision(m_rigidBody, this);
}

ege::physics::Component::Component(ememory::SharedPtr<ege::Environement> _env, const etk::Transform3D& _transform):
  m_staticForceApplyCenterOfMass(0,0,0),
  m_staticTorqueApply(0,0,0) {
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
	m_rigidBody->setUserData(this);
	m_lastTransformEmit = _transform;
	// set collision callback:
	//m_engine->getDynamicWorld()->testCollision(m_rigidBody, this);
	EGE_ERROR("Bounciness=" << m_rigidBody->getMaterial().getBounciness());
	EGE_ERROR("FrictionCoefficient=" << m_rigidBody->getMaterial().getFrictionCoefficient());
	EGE_ERROR("RollingResistance=" << m_rigidBody->getMaterial().getRollingResistance());
	EGE_ERROR("LinearDamping=" << m_rigidBody->getLinearDamping());
	EGE_ERROR("AngularDamping=" << m_rigidBody->getAngularDamping());
	m_rigidBody->getMaterial().setBounciness(0.4);
	//m_rigidBody->getMaterial().setFrictionCoefficient(0.01);
	//m_rigidBody->getMaterial().setRollingResistance(0.01);
	m_rigidBody->setAngularDamping(0.9);
	m_rigidBody->setLinearDamping(0.9);
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
	// disable callback
	m_rigidBody->setUserData(nullptr);
	m_engine->getDynamicWorld()->testCollision(m_rigidBody, nullptr);
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
			case ege::physics::Shape::type::box: {
				EGE_DEBUG("    Box");
				const ege::physics::shape::Box* tmpElement = it->toBox();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Box ==> can not cast in BOX");
					continue;
				}
				// Half extents of the box in the x, y and z directions
				const rp3d::Vector3 halfExtents(tmpElement->getSize().x(),
				                                tmpElement->getSize().y(),
				                                tmpElement->getSize().z());
				// Create the box shape
				rp3d::BoxShape* shape = new rp3d::BoxShape(halfExtents, 0.0001);
				m_listShape.push_back(shape);
				rp3d::Vector3 position(it->getOrigin().x(),
				                       it->getOrigin().y(),
				                       it->getOrigin().z());
				rp3d::Quaternion orientation(it->getQuaternion().x(),
				                             it->getQuaternion().y(),
				                             it->getQuaternion().z(),
				                             it->getQuaternion().w());
				// The ephysic use Y as UP ==> ege use Z as UP
				//orientation = orientation * rp3d::Quaternion(-0.707107, 0, 0, 0.707107);
				rp3d::Transform transform(position, orientation);
				rp3d::ProxyShape* proxyShape = m_rigidBody->addCollisionShape(shape, transform, it->getMass());
				proxyShape->setUserData(this);
				m_listProxyShape.push_back(proxyShape);
				break;
			}
			case ege::physics::Shape::type::cylinder: {
				EGE_DEBUG("    Cylinder");
				const ege::physics::shape::Cylinder* tmpElement = it->toCylinder();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Cylinder ==> can not cast in Cylinder");
					continue;
				}
				// Create the Cylinder shape
				rp3d::CylinderShape* shape = new rp3d::CylinderShape(tmpElement->getRadius(), tmpElement->getSize());
				rp3d::Vector3 position(it->getOrigin().x(),
				                       it->getOrigin().y(),
				                       it->getOrigin().z());
				rp3d::Quaternion orientation(it->getQuaternion().x(),
				                             it->getQuaternion().y(),
				                             it->getQuaternion().z(),
				                             it->getQuaternion().w());
				// The ephysic use Y as UP ==> ege use Z as UP
				orientation = orientation * rp3d::Quaternion(-0.707107, 0, 0, 0.707107);
				rp3d::Transform transform(position, orientation);
				rp3d::ProxyShape* proxyShape = m_rigidBody->addCollisionShape(shape, transform, it->getMass());
				proxyShape->setUserData(this);
				m_listProxyShape.push_back(proxyShape);
				break;
			}
			case ege::physics::Shape::type::capsule: {
				EGE_DEBUG("    Capsule");
				const ege::physics::shape::Capsule* tmpElement = it->toCapsule();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Capsule ==> can not cast in Capsule");
					continue;
				}
				// Create the Capsule shape
				rp3d::CapsuleShape* shape = new rp3d::CapsuleShape(tmpElement->getRadius(), tmpElement->getSize());
				rp3d::Vector3 position(it->getOrigin().x(),
				                       it->getOrigin().y(),
				                       it->getOrigin().z());
				rp3d::Quaternion orientation(it->getQuaternion().x(),
				                             it->getQuaternion().y(),
				                             it->getQuaternion().z(),
				                             it->getQuaternion().w());
				// The ephysic use Y as UP ==> ege use Z as UP
				orientation = orientation * rp3d::Quaternion(-0.707107, 0, 0, 0.707107);
				rp3d::Transform transform(position, orientation);
				rp3d::ProxyShape* proxyShape = m_rigidBody->addCollisionShape(shape, transform, it->getMass());
				proxyShape->setUserData(this);
				m_listProxyShape.push_back(proxyShape);
				break;
			}
			case ege::physics::Shape::type::cone: {
				EGE_DEBUG("    Cone");
				const ege::physics::shape::Cone* tmpElement = it->toCone();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Cone ==> can not cast in Cone");
					continue;
				}
				// Create the Cone shape
				rp3d::ConeShape* shape = new rp3d::ConeShape(tmpElement->getRadius(), tmpElement->getSize());
				rp3d::Vector3 position(it->getOrigin().x(),
				                       it->getOrigin().y(),
				                       it->getOrigin().z());
				rp3d::Quaternion orientation(it->getQuaternion().x(),
				                             it->getQuaternion().y(),
				                             it->getQuaternion().z(),
				                             it->getQuaternion().w());
				// The ephysic use Y as UP ==> ege use Z as UP
				orientation = orientation * rp3d::Quaternion(-0.707107, 0, 0, 0.707107);
				rp3d::Transform transform(position, orientation);
				rp3d::ProxyShape* proxyShape = m_rigidBody->addCollisionShape(shape, transform, it->getMass());
				proxyShape->setUserData(this);
				m_listProxyShape.push_back(proxyShape);
				break;
			}
			case ege::physics::Shape::type::sphere: {
				EGE_DEBUG("    Sphere");
				const ege::physics::shape::Sphere* tmpElement = it->toSphere();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Sphere ==> can not cast in Sphere");
					continue;
				}
				// Create the box shape
				rp3d::SphereShape* shape = new rp3d::SphereShape(tmpElement->getRadius());
				rp3d::Vector3 position(it->getOrigin().x(),
				                       it->getOrigin().y(),
				                       it->getOrigin().z());
				rp3d::Quaternion orientation(it->getQuaternion().x(),
				                             it->getQuaternion().y(),
				                             it->getQuaternion().z(),
				                             it->getQuaternion().w());
				// The ephysic use Y as UP ==> ege use Z as UP
				orientation = orientation * rp3d::Quaternion(-0.707107, 0, 0, 0.707107);
				rp3d::Transform transform(position, orientation);
				rp3d::ProxyShape* proxyShape = m_rigidBody->addCollisionShape(shape, transform, it->getMass());
				proxyShape->setUserData(this);
				m_listProxyShape.push_back(proxyShape);
				break;
			}
			case ege::physics::Shape::type::concave: {
				EGE_DEBUG("    Concave");
				const ege::physics::shape::Concave* tmpElement = it->toConcave();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Concave ==> can not cast in Concave");
					continue;
				}
				
				float* vertices = (float*)&tmpElement->getVertex()[0];
				int* indices = (int*)&tmpElement->getIndices()[0];
				
				int32_t nbVertices = tmpElement->getVertex().size();
				int32_t nbTriangles = tmpElement->getIndices().size()/3;
				// TODO : Manage memory leak ...
				//we have an error here ...
				
				rp3d::TriangleVertexArray* triangleArray = new rp3d::TriangleVertexArray(nbVertices,
				                                                                         vertices,
				                                                                         4 * sizeof(float),
				                                                                         nbTriangles,
				                                                                         indices,
				                                                                         sizeof(int32_t),
				                                                                         rp3d::TriangleVertexArray::VERTEX_FLOAT_TYPE,
				                                                                         rp3d::TriangleVertexArray::INDEX_INTEGER_TYPE);
				// Now that we have a TriangleVertexArray, we need to create a TriangleMesh and add the TriangleVertexArray into it as a subpart.
				// Once this is done, we can create the actual ConcaveMeshShape and add it to the body we want to simulate as in the following example:
				rp3d::TriangleMesh triangleMesh;
				// Add the triangle vertex array to the triangle mesh
				triangleMesh.addSubpart(triangleArray);
				// Create the concave mesh shape
				/*
				// TODO : Manage memory leak ...
				reactphysics3d::ConcaveShape* shape = new reactphysics3d::ConcaveMeshShape(&triangleMesh);
				rp3d::Vector3 position(it->getOrigin().x(),
				                       it->getOrigin().y(),
				                       it->getOrigin().z());
				rp3d::Quaternion orientation(it->getQuaternion().x(),
				                             it->getQuaternion().y(),
				                             it->getQuaternion().z(),
				                             it->getQuaternion().w());
				// The ephysic use Y as UP ==> ege use Z as UP
				orientation = orientation * rp3d::Quaternion(-0.707107, 0, 0, 0.707107);
				rp3d::Transform transform(position, orientation);
				rp3d::ProxyShape* proxyShape = m_rigidBody->addCollisionShape(shape, transform, it->getMass());
				proxyShape->setUserData(this);
				m_listProxyShape.push_back(proxyShape);
				*/
				break;
			}
			default :
				EGE_DEBUG("    ???");
				// TODO: UNKNOW type ... 
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

void ege::physics::Component::update(float _delta) {
	if (m_rigidBody == nullptr) {
		return;
	}
	if (m_staticForceApplyCenterOfMass != rp3d::Vector3(0,0,0)) {
		rp3d::Vector3 tmp = m_staticForceApplyCenterOfMass*_delta;
		EGE_ERROR("FORCE : " << vec3(tmp.x, tmp.y, tmp.z) );
		m_rigidBody->applyForceToCenterOfMass(tmp);
	}
	if (m_staticTorqueApply != rp3d::Vector3(0,0,0)) {
		rp3d::Vector3 tmp = m_staticTorqueApply*_delta;
		EGE_ERROR("TORQUE : " << vec3(tmp.x, tmp.y, tmp.z));
		m_rigidBody->applyTorque(tmp);
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

vec3 ege::physics::Component::getRelativeLinearVelocity() const {
	if (m_rigidBody == nullptr) {
		return vec3(0,0,0);
	}
	rp3d::Vector3 value = m_rigidBody->getLinearVelocity();
	value = m_rigidBody->getTransform().getOrientation().getInverse()*value;
	return vec3(value.x,
	            value.y,
	            value.z);
}

void ege::physics::Component::setRelativeLinearVelocity(const vec3& _linearVelocity) {
	if (m_rigidBody == nullptr) {
		return;
	}
	rp3d::Vector3 value(_linearVelocity.x(),
	                    _linearVelocity.y(),
	                    _linearVelocity.z());
	value = m_rigidBody->getTransform().getOrientation()*value;
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

vec3 ege::physics::Component::getRelativeAngularVelocity() const {
	if (m_rigidBody == nullptr) {
		return vec3(0,0,0);
	}
	rp3d::Vector3 value = m_rigidBody->getAngularVelocity();
	value = m_rigidBody->getTransform().getOrientation().getInverse()*value;
	return vec3(value.x,
	            value.y,
	            value.z);
}
void ege::physics::Component::setRelativeAngularVelocity(const vec3& _angularVelocity) {
	if (m_rigidBody == nullptr) {
		return;
	}
	rp3d::Vector3 value(_angularVelocity.x(),
	                    _angularVelocity.y(),
	                    _angularVelocity.z());
	value = m_rigidBody->getTransform().getOrientation()*value;
	m_rigidBody->setAngularVelocity(value);
}


void ege::physics::Component::applyForce(const vec3& _force,const vec3& _point) {
	if (m_rigidBody == nullptr) {
		return;
	}
	rp3d::Vector3 force(_force.x(),
	                    _force.y(),
	                    _force.z());
	rp3d::Vector3 point(_point.x(),
	                    _point.y(),
	                    _point.z());
	m_rigidBody->applyForce(force, point);
}

void ege::physics::Component::applyForceToCenterOfMass(const vec3& _force, bool _static) {
	if (m_rigidBody == nullptr) {
		return;
	}
	rp3d::Vector3 force(_force.x(),
	                    _force.y(),
	                    _force.z());
	if(_static == true) {
		m_staticForceApplyCenterOfMass = force;
	} else {
		m_rigidBody->applyForceToCenterOfMass(force);
	}
}

void ege::physics::Component::applyRelativeForceToCenterOfMass(const vec3& _force, bool _static) {
	if (m_rigidBody == nullptr) {
		return;
	}
	rp3d::Vector3 force(_force.x(),
	                    _force.y(),
	                    _force.z());
	force = m_rigidBody->getTransform().getOrientation()*force;
	if(_static == true) {
		m_staticForceApplyCenterOfMass = force;
	} else {
		m_rigidBody->applyForceToCenterOfMass(force);
	}
}

void ege::physics::Component::applyTorque(const vec3& _torque, bool _static) {
	if (m_rigidBody == nullptr) {
		return;
	}
	rp3d::Vector3 torque(_torque.x(),
	                     _torque.y(),
	                     _torque.z());
	if(_static == true) {
		m_staticTorqueApply = torque;
	} else {
		m_rigidBody->applyTorque(torque);
	}
}

void ege::physics::Component::applyRelativeTorque(const vec3& _torque, bool _static) {
	if (m_rigidBody == nullptr) {
		return;
	}
	rp3d::Vector3 torque(_torque.x(),
	                     _torque.y(),
	                     _torque.z());
	torque = m_rigidBody->getTransform().getOrientation()*torque;
	if(_static == true) {
		m_staticTorqueApply = torque;
	} else {
		m_rigidBody->applyTorque(torque);
	}
}

const std::vector<ememory::SharedPtr<ege::physics::Shape>>& ege::physics::Component::getShape() const {
	return m_shape;
}

void ege::physics::Component::setShape(const std::vector<ememory::SharedPtr<ege::physics::Shape>>& _prop) {
	m_shape = _prop;
}

void ege::physics::Component::addShape(const ememory::SharedPtr<ege::physics::Shape>& _shape) {
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
			case ege::physics::Shape::type::box: {
				EGE_DEBUG("    Box");
				const ege::physics::shape::Box* tmpElement = it->toBox();
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
			case ege::physics::Shape::type::cylinder: {
				EGE_DEBUG("    Cylinder");
				const ege::physics::shape::Cylinder* tmpElement = it->toCylinder();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Cylinder ==> can not cast in Cylinder");
					continue;
				}
				etk::Transform3D transformLocal(it->getOrigin(), it->getOrientation());
				transformLocal.getOpenGLMatrix(mmm);
				mat4 transformationMatrixLocal(mmm);
				transformationMatrixLocal.transpose();
				transformationMatrixLocal = transformationMatrix * transformationMatrixLocal;
				_draw->drawCylinder(tmpElement->getRadius(), tmpElement->getSize(), 10, 10, transformationMatrixLocal, tmpColor);
				break;
			}
			case ege::physics::Shape::type::capsule: {
				EGE_DEBUG("    Capsule");
				const ege::physics::shape::Capsule* tmpElement = it->toCapsule();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Capsule ==> can not cast in Capsule");
					continue;
				}
				etk::Transform3D transformLocal(it->getOrigin(), it->getOrientation());
				transformLocal.getOpenGLMatrix(mmm);
				mat4 transformationMatrixLocal(mmm);
				transformationMatrixLocal.transpose();
				transformationMatrixLocal = transformationMatrix * transformationMatrixLocal;
				_draw->drawCapsule(tmpElement->getRadius(), tmpElement->getSize(), 10, 10, transformationMatrixLocal, tmpColor);
				break;
			}
			case ege::physics::Shape::type::cone: {
				EGE_DEBUG("    Cone");
				const ege::physics::shape::Cone* tmpElement = it->toCone();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Cone ==> can not cast in Cone");
					continue;
				}
				etk::Transform3D transformLocal(it->getOrigin(), it->getOrientation());
				transformLocal.getOpenGLMatrix(mmm);
				mat4 transformationMatrixLocal(mmm);
				transformationMatrixLocal.transpose();
				transformationMatrixLocal = transformationMatrix * transformationMatrixLocal;
				_draw->drawCone(tmpElement->getRadius(), tmpElement->getSize(), 10, 10, transformationMatrixLocal, tmpColor);
				break;
			}
			case ege::physics::Shape::type::sphere: {
				EGE_DEBUG("    Sphere");
				const ege::physics::shape::Sphere* tmpElement = it->toSphere();
				if (tmpElement == nullptr) {
					EGE_ERROR("    Sphere ==> can not cast in Sphere");
					continue;
				}
				etk::Transform3D transformLocal(it->getOrigin(), it->getOrientation());
				transformLocal.getOpenGLMatrix(mmm);
				mat4 transformationMatrixLocal(mmm);
				transformationMatrixLocal.transpose();
				transformationMatrixLocal = transformationMatrix * transformationMatrixLocal;
				_draw->drawSphere(tmpElement->getRadius(), 10, 10, transformationMatrixLocal, tmpColor);
				break;
			}
			case ege::physics::Shape::type::convexHull: {
				EGE_DEBUG("    convexHull");
				const ege::physics::shape::ConvexHull* tmpElement = it->toConvexHull();
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
				// TODO: UNKNOW type ... 
				break;
		}
	}
}

void ege::physics::Component::drawAABB(ememory::SharedPtr<ewol::resource::Colored3DObject> _draw, ememory::SharedPtr<ege::Camera> _camera) {
	if (m_rigidBody == nullptr) {
		return;
	}
	mat4 transformationMatrix;
	etk::Color<float> tmpColor(0.0, 1.0, 0.0, 0.8);
	rp3d::AABB value = m_rigidBody->getAABB();
	vec3 minimum(value.getMin().x, value.getMin().y, value.getMin().z);
	vec3 maximum(value.getMax().x, value.getMax().y, value.getMax().z);
	_draw->drawCubeLine(minimum, maximum, tmpColor, transformationMatrix);
	
}
