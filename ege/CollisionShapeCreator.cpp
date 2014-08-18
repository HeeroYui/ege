/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 * 
 */

#include <ege/CollisionShapeCreator.h>

#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btConvexPolyhedron.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <ege/physicsShape/PhysicsShape.h>
#include <ege/physicsShape/PhysicsBox.h>
#include <ege/physicsShape/PhysicsCapsule.h>
#include <ege/physicsShape/PhysicsCone.h>
#include <ege/physicsShape/PhysicsConvexHull.h>
#include <ege/physicsShape/PhysicsCylinder.h>
#include <ege/physicsShape/PhysicsSphere.h>

// Documentetion of bullet library :
// http://bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Shapes

#undef __class__
#define __class__ "CollisionShapeCreator"

btCollisionShape* ege::collision::createShape(const std::shared_ptr<ege::resource::Mesh>& _mesh) {
	if (nullptr == _mesh) {
		return new btEmptyShape();;
	}
	const std::vector<ege::PhysicsShape*>& physiqueProperty = _mesh->getPhysicalProperties();
	if (physiqueProperty.size() == 0) {
		return new btEmptyShape();;
	}
	int32_t count = 0;
	for (size_t iii=0; iii<physiqueProperty.size(); iii++) {
		if (nullptr == physiqueProperty[iii]) {
			continue;
		}
		count++;
	}
	btCompoundShape* outputShape = nullptr;
	if (count>1) {
		outputShape = new btCompoundShape();
	}
	for (size_t iii=0; iii<physiqueProperty.size(); iii++) {
		if (nullptr == physiqueProperty[iii]) {
			continue;
		}
		switch (physiqueProperty[iii]->getType()) {
			case ege::PhysicsShape::box : {
				const ege::PhysicsBox* tmpElement = physiqueProperty[iii]->toBox();
				if (nullptr  == tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btBoxShape(tmpElement->getSize());
				if (nullptr != tmpShape) {
					if (outputShape == nullptr) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ege::PhysicsShape::cylinder : {
				const ege::PhysicsCylinder* tmpElement = physiqueProperty[iii]->toCylinder();
				if (nullptr  == tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btCylinderShape(tmpElement->getSize());
				if (nullptr != tmpShape) {
					if (outputShape == nullptr) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ege::PhysicsShape::capsule : {
				const ege::PhysicsCapsule* tmpElement = physiqueProperty[iii]->toCapsule();
				if (nullptr  == tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btCapsuleShape(tmpElement->getRadius(), tmpElement->getHeight());
				if (nullptr != tmpShape) {
					if (outputShape == nullptr) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ege::PhysicsShape::cone : {
				const ege::PhysicsCone* tmpElement = physiqueProperty[iii]->toCone();
				if (nullptr  == tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btConeShape(tmpElement->getRadius(), tmpElement->getHeight());
				if (nullptr != tmpShape) {
					if (outputShape == nullptr) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ege::PhysicsShape::sphere : {
				const ege::PhysicsSphere* tmpElement = physiqueProperty[iii]->toSphere();
				if (nullptr  == tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btSphereShape(tmpElement->getRadius());
				if (nullptr != tmpShape) {
					if (outputShape == nullptr) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ege::PhysicsShape::convexHull : {
				const ege::PhysicsConvexHull* tmpElement = physiqueProperty[iii]->toConvexHull();
				if (nullptr  == tmpElement) {
					// ERROR ...
					continue;
				}
				btConvexHullShape* tmpShape = new btConvexHullShape(&(tmpElement->getPointList()[0].x()), tmpElement->getPointList().size());
				if (nullptr != tmpShape) {
					if (outputShape == nullptr) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			default :
				// TODO : UNKNOW type ... 
				break;
		}
	}
	if (nullptr == outputShape) {
		return new btEmptyShape();
	}
	return outputShape;
}


