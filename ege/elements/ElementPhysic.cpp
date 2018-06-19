/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

*******************************************
** IS REMOVED
*******************************************




#include <etk/types.hpp>
#include <ege/debug.hpp>
#include <ege/elements/ElementPhysic.hpp>
#include <ege/Environement.hpp>

#include <ege/CollisionShapeCreator.hpp>

const etk::String& ege::ElementPhysic::getType() const {
	static const etk::String nameType("----");
	return nameType;
}


ege::ElementPhysic::ElementPhysic(const ememory::SharedPtr<ege::Environement>& _env, bool _autoRigidBody) ://, float _mass) :
  ege::Element(_env),
  m_body(null),
  //m_shape(null),
  m_elementInPhysicsSystem(false),
  m_IA(null),
  m_detectCollisionEnable(false) {
	if (_autoRigidBody == true) {
		createRigidBody();
	} else {
		EGE_TODO("add basic API to create custum rigid body");
	}
}

ege::ElementPhysic::~ElementPhysic() {
	// in every case remove IA
	//iaDisable();
	// same ...
	//dynamicDisable();
	//removeShape();
	// Destroy the rigid body
	//m_dynamicsWorld->destroyRigidBody(m_body);
	m_body = null;
}


void ege::ElementPhysic::createRigidBody(float _mass, bool _static) {
	// Initial position and orientation of the rigid body
	rp3d::Vector3 initPosition(0.0, 3.0, 0.0);
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Transform transform(initPosition, initOrientation);
	
	// Create a rigid body in the world
	m_body = null;//m_dynamicsWorld->createRigidBody(transform);
	/*
	if (_static = true) {
		m_body->setType(STATIC);
		//m_body->setType(KINEMATIC);
		// Disable gravity for this body
		m_body->enableGravity(false);
	} else {
		m_body->setType(DYNAMIC);
	}
	*/
}

bool ege::ElementPhysic::setMesh(ememory::SharedPtr<ege::resource::Mesh> _mesh) {
	EGE_WARNING("Set Mesh");
	if (m_mesh != null) {
		//removeShape();
	}
	ege::Element::setMesh(_mesh);
	// auto load the shape:
	if (m_mesh == null) {
		return true;
	}
	/*
	if (m_mesh->getShape() != null) {
		EGE_WARNING("create shape whith mesh internal shape ...");
		m_shape = static_cast<btCollisionShape*>(m_mesh->getShape());
		return true;
	}
	EGE_DEBUG("create the mesh shape with the mesh");
	m_mesh->setShape(ege::collision::createShape(m_mesh));
	EGE_DEBUG("set remove function shape");
	m_mesh->setFreeShapeFunction(&FunctionFreeShape);
	m_shape = static_cast<btCollisionShape*>(m_mesh->getShape());
	vec3 localInertia(0,0,0);
	m_shape->calculateLocalInertia(50000000, localInertia); // TODO : BETTER ///
	*/
	return true;
}

/*
bool ege::ElementPhysic::setShape(btCollisionShape* _shape) {
	EGE_DEBUG("Set Shape");
	removeShape();
	m_shape = _shape;
	if (_shape == null) {
		EGE_WARNING("Remove shape ...");
	} else {
		EGE_INFO("set shape ...");
	}
	return true;
}

void ege::ElementPhysic::removeShape() {
	// no shape
	if (m_shape == null) {
		return;
	}
	// need to chek if the shape is the same as the mesh shape ...
	if (m_mesh == null) {
		// no mesh  == > standalone shape
		ETK_DELETE(btCollisionShape, m_shape);
		m_shape=null;
		EGE_WARNING("Remove shape .2.");
		return;
	}
	if (m_shape != m_mesh->getShape()) {
		ETK_DELETE(btCollisionShape, m_shape);
		m_shape=null;
		EGE_WARNING("Remove shape .3.");
		return;
	}
	// otherwise : the shape is auto remove by the resources when no more needed ...
}

void ege::ElementPhysic::FunctionFreeShape(void* _pointer) {
	if (_pointer == null) {
		return;
	}
	ETK_DELETE(btCollisionShape, _pointer);
}
*/
void ege::ElementPhysic::setPosition(const vec3& _pos) {
	if (m_body != null) {
		/*
		btTransform transformation = m_body->getCenterOfMassTransform();
		transformation.setOrigin(_pos);
		m_body->setCenterOfMassTransform(transformation);
		*/
	}
}

const vec3& ege::ElementPhysic::getPosition() {
	/*
	if (m_body != null) {
		return m_body->getCenterOfMassPosition();
	}
	*/
	return ege::Element::getPosition();
};

const vec3& ege::ElementPhysic::getSpeed() {
	static vec3 emptySpeed(0,0,0);
	/*
	if (m_body != null) {
		return m_body->getLinearVelocity();
	}
	*/
	return emptySpeed;
};

const float ege::ElementPhysic::getInvMass() {
	/*
	if (m_body != null) {
		return m_body->getInvMass();
	}
	*/
	return 0.0000000001f;
};

void ege::ElementPhysic::drawShape(/*const btCollisionShape* _shape,*/
                                   ememory::SharedPtr<ewol::resource::Colored3DObject> _draw,
                                   mat4 _transformationMatrix,
                                   etk::Vector<vec3> _tmpVertices) {
	#if 0
	if(    _draw == null
	    || _shape == null) {
		return;
	}
	etk::Color<float> tmpColor(1.0, 0.0, 0.0, 0.3);
	//EGE_DEBUG("        draw (6): !btIDebugDraw::DBG_DrawWireframe");
	int shapetype=_shape->getShapeType();
	switch (shapetype) {
		case SPHERE_SHAPE_PROXYTYPE: {
			// Sphere collision shape ...
			EGE_DEBUG("            draw (01): SPHERE_SHAPE_PROXYTYPE");
			const btSphereShape* sphereShape = static_cast<const btSphereShape*>(_shape);
			float radius = sphereShape->getMargin();//radius doesn't include the margin, so draw with margin
			_draw->drawSphere(radius, 10, 10, _transformationMatrix, tmpColor);
			break;
		}
		case BOX_SHAPE_PROXYTYPE: {
			// Box collision shape ...
			EGE_DEBUG("            draw (02): BOX_SHAPE_PROXYTYPE");
			const btBoxShape* boxShape = static_cast<const btBoxShape*>(_shape);
			vec3 halfExtent = boxShape->getHalfExtentsWithMargin();
			_draw->drawSquare(halfExtent, _transformationMatrix, tmpColor);
			break;
		}
		case CONE_SHAPE_PROXYTYPE: {
			// Cone collision shape ...
			EGE_DEBUG("            draw (03): CONE_SHAPE_PROXYTYPE");
			break;
		}
		case CAPSULE_SHAPE_PROXYTYPE: {
			// Capsule collision shape ...
			EGE_DEBUG("            draw (04): CAPSULE_SHAPE_PROXYTYPE");
			break;
		}
		case CYLINDER_SHAPE_PROXYTYPE: {
			// Cylinder collision shape ...
			EGE_DEBUG("            draw (05): CYLINDER_SHAPE_PROXYTYPE");
			break;
		}
		case CONVEX_HULL_SHAPE_PROXYTYPE: {
			// Convex Hull collision shape ...
			EGE_DEBUG("            draw (06): CONVEX_HULL_SHAPE_PROXYTYPE");
			if (_shape->isConvex()) {
					EGE_DEBUG("                shape->isConvex()");
					const btConvexPolyhedron* poly = _shape->isPolyhedral() ? ((btPolyhedralConvexShape*) _shape)->getConvexPolyhedron() : 0;
					if (null!=poly) {
						EGE_DEBUG("                      have poly");
						/*
						glBegin(GL_TRIANGLES);
						for (int32_t iii=0 ; iii<poly->m_faces.size() ; iii++) {
							btVector3 centroid(0,0,0);
							int numVerts = poly->m_faces[iii].m_indices.size();
							if (numVerts>2) {
								btVector3 v1 = poly->m_vertices[poly->m_faces[iii].m_indices[0]];
								for (int32_t vvv=0;vvv<poly->m_faces[iii].m_indices.size()-2;vvv++) {
									btVector3 v2 = poly->m_vertices[poly->m_faces[iii].m_indices[vvv+1]];
									btVector3 v3 = poly->m_vertices[poly->m_faces[iii].m_indices[vvv+2]];
									btVector3 normal = (v3-v1).cross(v2-v1);
									normal.normalize ();
									
									glNormal3f(normal.getX(),normal.getY(),normal.getZ());
									glVertex3f (v1.x(), v1.y(), v1.z());
									glVertex3f (v2.x(), v2.y(), v2.z());
									glVertex3f (v3.x(), v3.y(), v3.z());
									
								}
							}
						}
						glEnd();
						*/
					} else {
						// TODO : set it back ...
						/*
						ShapeCache*	sc=cache((btConvexShape*)_shape);
						//glutSolidCube(1.0);
						btShapeHull* hull = &sc->m_shapehull;
						if (hull->numTriangles () > 0) {
							int index = 0;
							const unsigned int* idx = hull->getIndexPointer();
							const btVector3* vtx = hull->getVertexPointer();
							glBegin (GL_TRIANGLES);
							for (int i = 0; i < hull->numTriangles(); i++) {
								int i1 = index++;
								int i2 = index++;
								int i3 = index++;
								btAssert(i1 < hull->numIndices() &&
								         i2 < hull->numIndices() &&
								         i3 < hull->numIndices());
								int index1 = idx[i1];
								int index2 = idx[i2];
								int index3 = idx[i3];
								btAssert(index1 < hull->numVertices() &&
								         index2 < hull->numVertices() &&
								         index3 < hull->numVertices());
								btVector3 v1 = vtx[index1];
								btVector3 v2 = vtx[index2];
								btVector3 v3 = vtx[index3];
								btVector3 normal = (v3-v1).cross(v2-v1);
								normal.normalize();
								glNormal3f(normal.getX(),normal.getY(),normal.getZ());
								glVertex3f (v1.x(), v1.y(), v1.z());
								glVertex3f (v2.x(), v2.y(), v2.z());
								glVertex3f (v3.x(), v3.y(), v3.z());
							}
							glEnd ();
						}
						*/
					}
				} else {
					EGE_DEBUG("                !!! shape is NOT Convex() !!!");
				}
			break;
		}
		case COMPOUND_SHAPE_PROXYTYPE: {
			// Multiple sub element collision shape ...
			EGE_DEBUG("            draw (07): COMPOUND_SHAPE_PROXYTYPE");
			const btCompoundShape* compoundShape = static_cast<const btCompoundShape*>(_shape);
			for (int32_t iii=compoundShape->getNumChildShapes()-1;iii >= 0;iii--) {
				btTransform childTrans = compoundShape->getChildTransform(iii);
				const btCollisionShape* colShape = compoundShape->getChildShape(iii);
				btScalar mmm[16];
				childTrans.getOpenGLMatrix(mmm);
				mat4 transformationMatrix(mmm);
				transformationMatrix.transpose();
				transformationMatrix = _transformationMatrix * transformationMatrix;
				drawShape(colShape, _draw, transformationMatrix, _tmpVertices);
			}
			break;
		}
		case EMPTY_SHAPE_PROXYTYPE: {
			// No collision shape ...
			EGE_DEBUG("            draw (08): EMPTY_SHAPE_PROXYTYPE");
			// nothing to display ...
			break;
		}
		default: {
			// must be done later ...
			EGE_DEBUG("            draw (09): default");
		}
	}
	#endif
}


void ege::ElementPhysic::draw(int32_t _pass) {
	if (m_elementInPhysicsSystem == false) {
		return;
	}
	/*
	//EGE_INFO("draw : " << _pass );
	if (_pass == 0) {
		if(    m_body != null
		    && m_mesh != null) {
			//EGE_INFO("element pos = " << getPosition());
			float mmm[16];
			// Get the interpolated transform of the rigid body 
			rp3d::Transform transform = m_body->getInterpolatedTransform();
			// Get the OpenGL matrix array of the transform 
			transform.getOpenGLMatrix(matrix);
			
			//EGE_INFO("    mat = " << mat4(mmm));
			mat4 transformationMatrix(mmm);
			//mat4 transformationMatrix = mat4(mmm) * etk::matScale(vec3(20,20,20));
			// TODO: check this : transformationMatrix.transpose();
			m_mesh->draw(transformationMatrix);
		}
	}
	*/
}

void ege::ElementPhysic::dynamicEnable() {
	if (m_elementInPhysicsSystem == true) {
		return;
	}
	if(m_body != null) {
		EGE_VERBOSE("dynamicEnable : RigidBody");
		//m_env->getPhysicEngine().getDynamicWorld()->addRigidBody(m_body);
	}
	if(m_IA != null) {
		EGE_VERBOSE("dynamicEnable : IA");
		//m_env->getPhysicEngine().getDynamicWorld()->addAction(m_IA);
	}
	m_elementInPhysicsSystem = true;
}

void ege::ElementPhysic::dynamicDisable() {
	if (m_elementInPhysicsSystem == false) {
		return;
	}
	if(m_IA != null) {
		EGE_VERBOSE("dynamicDisable : IA");
		//m_env->getPhysicEngine().getDynamicWorld()->removeAction(m_IA);
	}
	if(m_body != null) {
		EGE_VERBOSE("dynamicDisable : RigidBody");
		// Unlink element from the engine
		//m_env->getPhysicEngine().getDynamicWorld()->removeRigidBody(m_body);
		//m_env->getPhysicEngine().getDynamicWorld()->removeCollisionObject(m_body);
	}
	m_elementInPhysicsSystem = false;
}

void ege::ElementPhysic::iaEnable() {
	if (m_IA != null) {
		// IA already started ...
		return;
	}
	m_IA = ETK_NEW(localIA, *this);
	if (m_IA == null) {
		EGE_ERROR("Can not start the IA == > allocation error");
		return;
	}
	if (m_elementInPhysicsSystem == true) {
		//m_env->getPhysicEngine().getDynamicWorld()->addAction(m_IA);
	}
}

void ege::ElementPhysic::iaDisable() {
	if (m_IA == null) {
		// IA already stopped ...
		return;
	}
	if (m_elementInPhysicsSystem == true) {
		//m_env->getPhysicEngine().getDynamicWorld()->removeAction(m_IA);
	}
	// remove IA:
	ETK_DELETE(localIA, m_IA);
	m_IA = null;
}

void ege::ElementPhysic::setMass(float _value) {
	if (m_body == null) {
		return;
	}
	/*
	vec3 localInertia(0,0,0);
	if (_value != 0.0f && getShape()!=null) {
		//getShape()->calculateLocalInertia(_value, localInertia);
		EWOL_ERROR("Update inertia calculated : " << localInertia);
	}
	m_body->setMassProps(_value, localInertia);
	*/
}

void ege::ElementPhysic::setLinearVelocity(const vec3& _value) {
	if (m_body == null) {
		EGE_WARNING("no body");
		return;
	}
	// Force vector (in Newton)
	rp3d::Vector3 force(_value.x(), _value.y(), _value.z());
	// Apply a force to the center of the body
	m_body->applyForceToCenterOfMass(force);
}

void ege::ElementPhysic::setTorqueImpulse(const vec3& _value) {
	if (m_body == null) {
		EGE_WARNING("no body");
		return;
	}
	// Torque vector
	rp3d::Vector3 torque(_value.x(), _value.y(), _value.z());
	// Apply a torque to the body 
	m_body->applyTorque(torque);
}

void ege::ElementPhysic::setAngularVelocity(const vec3& _value) {
	if (m_body == null) {
		EGE_WARNING("no body");
		return;
	}
	//m_body->setAngularVelocity(_value);
}
/*
btQuaternion ege::ElementPhysic::getOrientation() const {
	if (m_body == null) {
		EGE_WARNING("no body");
		return btQuaternion(0,0,0,0);
	}
	//return m_body->getOrientation();
	return btQuaternion(0,0,0,0);
}
*/

void ege::ElementPhysic::setCollisionDetectionStatus(bool _status) {
	if (m_body == null) {
		EGE_WARNING("no body");
		return;
	}
	/*
	if (m_detectCollisionEnable == _status) {
		return;
	}
	m_detectCollisionEnable = _status;
	if (m_detectCollisionEnable == true) {
		m_body->setCollisionFlags(m_body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	} else {
		if ((m_body->getCollisionFlags() & btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK) != 0) {
			m_body->setCollisionFlags(m_body->getCollisionFlags() - btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
		}
	}
	*/
}

