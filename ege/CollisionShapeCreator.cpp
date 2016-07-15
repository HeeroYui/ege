/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ege/debug.h>
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

btCollisionShape* ege::collision::createShape(const ememory::SharedPtr<ege::resource::Mesh>& _mesh) {
	if (_mesh == nullptr) {
		EGE_DEBUG("Create empty shape (no mesh)");
		return new btEmptyShape();;
	}
	const std::vector<ememory::SharedPtr<ege::PhysicsShape>>& physiqueProperty = _mesh->getPhysicalProperties();
	if (physiqueProperty.size() == 0) {
		EGE_DEBUG("Create empty shape (no default shape)");
		return new btEmptyShape();;
	}
	int32_t count = 0;
	for (size_t iii=0; iii<physiqueProperty.size(); iii++) {
		if (physiqueProperty[iii] == nullptr) {
			continue;
		}
		count++;
	}
	btCompoundShape* outputShape = nullptr;
	if (count>1) {
		EGE_DEBUG("Create complexe shape");
		outputShape = new btCompoundShape();
	} else {
		EGE_DEBUG("Create simple shape");
	}
	for (size_t iii=0; iii<physiqueProperty.size(); iii++) {
		if (physiqueProperty[iii] == nullptr) {
			continue;
		}
		switch (physiqueProperty[iii]->getType()) {
			case ege::PhysicsShape::box : {
				EGE_DEBUG("    Box");
				const ege::PhysicsBox* tmpElement = physiqueProperty[iii]->toBox();
				if (tmpElement == nullptr) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btBoxShape(tmpElement->getSize());
				if (tmpShape != nullptr) {
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
				EGE_DEBUG("    Cylinder");
				const ege::PhysicsCylinder* tmpElement = physiqueProperty[iii]->toCylinder();
				if (tmpElement == nullptr) {
					// ERROR ...
					continue;
				}
				btCollisionShape* tmpShape = new btCylinderShape(tmpElement->getSize());
				if (tmpShape != nullptr) {
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
				EGE_DEBUG("    Capsule");
				const ege::PhysicsCapsule* tmpElement = physiqueProperty[iii]->toCapsule();
				if (tmpElement == nullptr) {
					// ERROR ...
					continue;
				}
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
				break;
			}
			case ege::PhysicsShape::cone : {
				EGE_DEBUG("    Cone");
				const ege::PhysicsCone* tmpElement = physiqueProperty[iii]->toCone();
				if (tmpElement == nullptr) {
					// ERROR ...
					continue;
				}
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
				break;
			}
			case ege::PhysicsShape::sphere : {
				EGE_DEBUG("    Sphere");
				const ege::PhysicsSphere* tmpElement = physiqueProperty[iii]->toSphere();
				if (tmpElement == nullptr) {
					// ERROR ...
					continue;
				}
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
				break;
			}
			case ege::PhysicsShape::convexHull : {
				EGE_DEBUG("    convexHull");
				const ege::PhysicsConvexHull* tmpElement = physiqueProperty[iii]->toConvexHull();
				if (tmpElement == nullptr) {
					// ERROR ...
					continue;
				}
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
				break;
			}
			default :
				EGE_DEBUG("    ???");
				// TODO : UNKNOW type ... 
				break;
		}
	}
	if (outputShape == nullptr) {
		EGE_DEBUG("create empty shape ...");
		return new btEmptyShape();
	}
	return outputShape;
}


