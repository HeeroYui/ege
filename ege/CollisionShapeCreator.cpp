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
#include <ewol/renderer/resources/physicsShape/PhysicsShape.h>
#include <ewol/renderer/resources/physicsShape/PhysicsBox.h>
#include <ewol/renderer/resources/physicsShape/PhysicsCapsule.h>
#include <ewol/renderer/resources/physicsShape/PhysicsCone.h>
#include <ewol/renderer/resources/physicsShape/PhysicsConvexHull.h>
#include <ewol/renderer/resources/physicsShape/PhysicsCylinder.h>
#include <ewol/renderer/resources/physicsShape/PhysicsSphere.h>

// Documentetion of bullet library :
// http://bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Shapes

btCollisionShape* ege::collision::CreateShape(const ewol::Mesh* _mesh)
{
	if (NULL == _mesh) {
		return new btEmptyShape();;
	}
	const etk::Vector<ewol::PhysicsShape*>& physiqueProperty = _mesh->GetPhysicalProperties();
	if (physiqueProperty.Size()==0) {
		return new btEmptyShape();;
	}
	int32_t count = 0;
	for (int32_t iii=0; iii<physiqueProperty.Size(); iii++) {
		if (NULL==physiqueProperty[iii]) {
			continue;
		}
		count++;
	}
	btCompoundShape* outputShape = NULL;
	if (count>1) {
		outputShape = new btCompoundShape();
	}
	for (int32_t iii=0; iii<physiqueProperty.Size(); iii++) {
		if (NULL==physiqueProperty[iii]) {
			continue;
		}
		switch (physiqueProperty[iii]->GetType()) {
			case ewol::PhysicsShape::box : {
				const ewol::PhysicsBox* tmpElement = physiqueProperty[iii]->ToBox();
				if (NULL ==tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btBoxShape(tmpElement->GetSize());
				if (NULL != tmpShape) {
					if (outputShape == NULL) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->GetQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->GetOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ewol::PhysicsShape::cylinder : {
				const ewol::PhysicsCylinder* tmpElement = physiqueProperty[iii]->ToCylinder();
				if (NULL ==tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btCylinderShape(tmpElement->GetSize());
				if (NULL != tmpShape) {
					if (outputShape == NULL) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->GetQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->GetOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ewol::PhysicsShape::capsule : {
				const ewol::PhysicsCapsule* tmpElement = physiqueProperty[iii]->ToCapsule();
				if (NULL ==tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btCapsuleShape(tmpElement->GetRadius(), tmpElement->GetHeight());
				if (NULL != tmpShape) {
					if (outputShape == NULL) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->GetQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->GetOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ewol::PhysicsShape::cone : {
				const ewol::PhysicsCone* tmpElement = physiqueProperty[iii]->ToCone();
				if (NULL ==tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btConeShape(tmpElement->GetRadius(), tmpElement->GetHeight());
				if (NULL != tmpShape) {
					if (outputShape == NULL) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->GetQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->GetOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ewol::PhysicsShape::sphere : {
				const ewol::PhysicsSphere* tmpElement = physiqueProperty[iii]->ToSphere();
				if (NULL ==tmpElement) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btSphereShape(tmpElement->GetRadius());
				if (NULL != tmpShape) {
					if (outputShape == NULL) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->GetQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->GetOrigin());
						outputShape->addChildShape(localTransform, tmpShape);
					}
				}
				break;
			}
			case ewol::PhysicsShape::convexHull : {
				const ewol::PhysicsConvexHull* tmpElement = physiqueProperty[iii]->ToConvexHull();
				if (NULL ==tmpElement) {
					// ERROR ...
					continue;
				}
				btConvexHullShape* tmpShape = new btConvexHullShape(&(tmpElement->GetPointList()[0].x()), tmpElement->GetPointList().Size());
				if (NULL != tmpShape) {
					if (outputShape == NULL) {
						return tmpShape;
					} else {
						vec4 qqq = tmpElement->GetQuaternion();
						const btTransform localTransform(btQuaternion(qqq.x(),qqq.y(),qqq.z(),qqq.w()), tmpElement->GetOrigin());
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
	if (NULL==outputShape) {
		return new btEmptyShape();
	}
	return outputShape;
}


