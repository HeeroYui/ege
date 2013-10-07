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
#include <ewol/physicsShape/PhysicsShape.h>
#include <ewol/physicsShape/PhysicsBox.h>
#include <ewol/physicsShape/PhysicsCapsule.h>
#include <ewol/physicsShape/PhysicsCone.h>
#include <ewol/physicsShape/PhysicsConvexHull.h>
#include <ewol/physicsShape/PhysicsCylinder.h>
#include <ewol/physicsShape/PhysicsSphere.h>

// Documentetion of bullet library :
// http://bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Shapes

#undef __class__
#define __class__	"CollisionShapeCreator"

btCollisionShape* ege::collision::CreateShape(const ewol::Mesh* _mesh)
{
	if (NULL == _mesh) {
		return new btEmptyShape();;
	}
	const etk::Vector<ewol::PhysicsShape*>& physiqueProperty = _mesh->getPhysicalProperties();
	if (physiqueProperty.size() == 0) {
		return new btEmptyShape();;
	}
	int32_t count = 0;
	for (int32_t iii=0; iii<physiqueProperty.size(); iii++) {
		if (NULL == physiqueProperty[iii]) {
			continue;
		}
		count++;
	}
	btCompoundShape* outputShape = NULL;
	if (count>1) {
		outputShape = new btCompoundShape();
	}
	for (int32_t iii=0; iii<physiqueProperty.size(); iii++) {
		if (NULL == physiqueProperty[iii]) {
			continue;
		}
		switch (physiqueProperty[iii]->getType()) {
			case ewol::PhysicsShape::box : {
				const ewol::PhysicsBox* tmpElement = physiqueProperty[iii]->toBox();
				if (NULL  == tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btBoxShape(tmpElement->getSize());
				if (NULL != tmpShape) {
					if (outputShape == NULL) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ewol::PhysicsShape::cylinder : {
				const ewol::PhysicsCylinder* tmpElement = physiqueProperty[iii]->toCylinder();
				if (NULL  == tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btCylinderShape(tmpElement->getSize());
				if (NULL != tmpShape) {
					if (outputShape == NULL) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ewol::PhysicsShape::capsule : {
				const ewol::PhysicsCapsule* tmpElement = physiqueProperty[iii]->toCapsule();
				if (NULL  == tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btCapsuleShape(tmpElement->getRadius(), tmpElement->getHeight());
				if (NULL != tmpShape) {
					if (outputShape == NULL) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ewol::PhysicsShape::cone : {
				const ewol::PhysicsCone* tmpElement = physiqueProperty[iii]->toCone();
				if (NULL  == tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btConeShape(tmpElement->getRadius(), tmpElement->getHeight());
				if (NULL != tmpShape) {
					if (outputShape == NULL) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ewol::PhysicsShape::sphere : {
				const ewol::PhysicsSphere* tmpElement = physiqueProperty[iii]->toSphere();
				if (NULL  == tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btSphereShape(tmpElement->getRadius());
				if (NULL != tmpShape) {
					if (outputShape == NULL) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->getQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->getOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ewol::PhysicsShape::convexHull : {
				const ewol::PhysicsConvexHull* tmpElement = physiqueProperty[iii]->toConvexHull();
				if (NULL  == tmpElement) {
					// ERROR ...
					continue;
				}
				btConvexHullShape* tmpShape = new btConvexHullShape(&(tmpElement->getPointList()[0].x()), tmpElement->getPointList().size());
				if (NULL != tmpShape) {
					if (outputShape == NULL) {
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
	if (NULL == outputShape) {
		return new btEmptyShape();
	}
	return outputShape;
}


