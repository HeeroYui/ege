	/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <etk/types.h>
#include <ege/debug.h>
#include <ege/ElementGame.h>
#include <ege/Environement.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <LinearMath/btIDebugDraw.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btConvexPolyhedron.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <LinearMath/btTransformUtil.h>
#include <LinearMath/btIDebugDraw.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

#include <ege/CollisionShapeCreator.h>

#undef __class__
#define __class__	"ElementGame"

const std::string& ege::ElementGame::getType() const {
	static const std::string nameType("----");
	return nameType;
}


ege::ElementGame::ElementGame(ege::Environement& _env) :
  m_env(_env),
  m_body(nullptr),
  m_uID(0),
  m_mesh(nullptr),
  m_shape(nullptr),
  m_life(100),
  m_lifeMax(100),
  m_group(0),
  m_fixe(true),
  m_radius(0),
  m_elementInPhysicsSystem(false),
  m_IA(nullptr) {
	static uint32_t unique=0;
	m_uID = unique;
	EGE_DEBUG("Create element : uId=" << m_uID);
	m_debugText.setFontSize(12);
	unique++;
}

ege::ElementGame::~ElementGame() {
	// in every case remove IA
	iaDisable();
	// same ...
	dynamicDisable();
	removeShape();
	if (nullptr != m_body) {
		delete(m_body);
		m_body = nullptr;
	}
	EGE_DEBUG("Destroy element : uId=" << m_uID);
}

void ege::ElementGame::removeShape() {
	// no shape
	if (nullptr == m_shape) {
		return;
	}
	// need to chek if the shape is the same as the mesh shape ...
	if (m_mesh == nullptr) {
		// no mesh  == > standalone shape
		delete(m_shape);
		m_shape=nullptr;
		return;
	}
	if (m_shape != m_mesh->getShape()) {
		delete(m_shape);
		m_shape=nullptr;
		return;
	}
	// otherwise : the shape is auto remove by the resources when no more needed ...
}

void ege::ElementGame::FunctionFreeShape(void* _pointer) {
	if (nullptr == _pointer) {
		return;
	}
	delete(static_cast<btCollisionShape*>(_pointer));
}

bool ege::ElementGame::loadMesh(const std::string& _meshFileName) {
	ewol::object::Shared<ege::resource::Mesh> tmpMesh = ege::resource::Mesh::keep(_meshFileName);
	if(nullptr == tmpMesh) {
		EGE_ERROR("can not load the resources : " << _meshFileName);
		return false;
	}
	return setMesh(tmpMesh);
}

bool ege::ElementGame::setMesh(const ewol::object::Shared<ege::resource::Mesh>& _mesh) {
	if (nullptr!=m_mesh) {
		removeShape();
		m_mesh.reset();
	}
	m_mesh = _mesh;
	// auto load the shape :
	if (m_mesh == nullptr) {
		return true;
	}
	if (m_mesh->getShape() != nullptr) {
		m_shape = static_cast<btCollisionShape*>(m_mesh->getShape());
		return true;
	}
	m_mesh->setShape(ege::collision::createShape(m_mesh));
	m_mesh->setFreeShapeFunction(&FunctionFreeShape);
	m_shape = static_cast<btCollisionShape*>(m_mesh->getShape());
	return true;
}

bool ege::ElementGame::setShape(btCollisionShape* _shape) {
	removeShape();
	m_shape = _shape;
	return true;
}

float ege::ElementGame::getLifeRatio() {
	if (0 >= m_life) {
		return 0;
	}
	return m_life/m_lifeMax;
}

void ege::ElementGame::setFireOn(int32_t _groupIdSource, int32_t _type, float _power, const vec3& _center) {
	float previousLife = m_life;
	m_life += _power;
	m_life = etk_avg(0, m_life, m_lifeMax);
	if (m_life <= 0) {
		EGE_DEBUG("[" << getUID() << "] element is killed ..." << getType());
	}
	if (m_life!=previousLife) {
		onLifeChange();
	}
}

void ege::ElementGame::setPosition(const vec3& _pos) {
	if (nullptr!=m_body) {
		btTransform transformation = m_body->getCenterOfMassTransform();
		transformation.setOrigin(_pos);
		m_body->setCenterOfMassTransform(transformation);
	}
}

const vec3& ege::ElementGame::getPosition() {
	// this is to prevent error like segmentation fault ...
	static vec3 emptyPosition(-1000000,-1000000,-1000000);
	if (nullptr!=m_body) {
		return m_body->getCenterOfMassPosition();
	}
	return emptyPosition;
};

const vec3& ege::ElementGame::getSpeed() {
	// this is to prevent error like segmentation fault ...
	static vec3 emptySpeed(0,0,0);
	if (nullptr!=m_body) {
		return m_body->getLinearVelocity();
	}
	return emptySpeed;
};

const float ege::ElementGame::getInvMass() {
	if (nullptr!=m_body) {
		return m_body->getInvMass();
	}
	return 0.0000000001f;
};

static void drawSphere(const ewol::object::Shared<ewol::resource::Colored3DObject>& _draw,
                       btScalar _radius,
                       int _lats,
                       int _longs,
                       mat4& _transformationMatrix,
                       etk::Color<float>& _tmpColor) {
	int i, j;
	std::vector<vec3> EwolVertices;
	for(i = 0; i <= _lats; i++) {
		btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / _lats);
		btScalar z0  = _radius*sin(lat0);
		btScalar zr0 = _radius*cos(lat0);
		
		btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / _lats);
		btScalar z1 = _radius*sin(lat1);
		btScalar zr1 = _radius*cos(lat1);
		
		//glBegin(GL_QUAD_STRIP);
		for(j = 0; j < _longs; j++) {
			btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / _longs;
			btScalar x = cos(lng);
			btScalar y = sin(lng);
			vec3 v1 = vec3(x * zr1, y * zr1, z1);
			vec3 v4 = vec3(x * zr0, y * zr0, z0);
			
			lng = 2 * SIMD_PI * (btScalar) (j) / _longs;
			x = cos(lng);
			y = sin(lng);
			vec3 v2 = vec3(x * zr1, y * zr1, z1);
			vec3 v3 = vec3(x * zr0, y * zr0, z0);
			
			EwolVertices.push_back(v1);
			EwolVertices.push_back(v2);
			EwolVertices.push_back(v3);
			
			EwolVertices.push_back(v1);
			EwolVertices.push_back(v3);
			EwolVertices.push_back(v4);
		}
	}
	_draw->draw(EwolVertices, _tmpColor, _transformationMatrix);
}

const float lifeBorder = 0.1f;
const float lifeHeight = 0.3f;
const float lifeWidth = 2.0f;
const float lifeYPos = 1.7f;

void ege::ElementGame::drawLife(const ewol::object::Shared<ewol::resource::Colored3DObject>& _draw, const ege::Camera& _camera) {
	if (nullptr == _draw) {
		return;
	}
	float ratio = getLifeRatio();
	if (ratio == 1.0f) {
		return;
	}
	mat4 transformationMatrix =   etk::matTranslate(getPosition())
	                            * etk::matRotate(vec3(0,0,1),_camera.getAngleZ())
	                            * etk::matRotate(vec3(1,0,0),(M_PI/2.0f-_camera.getAngleTeta()));
	std::vector<vec3> localVertices;
	localVertices.push_back(vec3(-lifeWidth/2.0-lifeBorder,lifeYPos           -lifeBorder,0));
	localVertices.push_back(vec3(-lifeWidth/2.0-lifeBorder,lifeYPos+lifeHeight+lifeBorder,0));
	localVertices.push_back(vec3( lifeWidth/2.0+lifeBorder,lifeYPos+lifeHeight+lifeBorder,0));
	localVertices.push_back(vec3(-lifeWidth/2.0-lifeBorder,lifeYPos           -lifeBorder,0));
	localVertices.push_back(vec3( lifeWidth/2.0+lifeBorder,lifeYPos+lifeHeight+lifeBorder,0));
	localVertices.push_back(vec3( lifeWidth/2.0+lifeBorder,lifeYPos           -lifeBorder,0));
	etk::Color<float> myColor(0x0000FF99);
	_draw->draw(localVertices, myColor, transformationMatrix, false, false);
	localVertices.clear();
	/** Bounding box  == > model shape **/
	localVertices.push_back(vec3(-lifeWidth/2.0                ,lifeYPos,0));
	localVertices.push_back(vec3(-lifeWidth/2.0                ,lifeYPos + lifeHeight,0));
	localVertices.push_back(vec3(-lifeWidth/2.0+lifeWidth*ratio,lifeYPos + lifeHeight,0));
	localVertices.push_back(vec3(-lifeWidth/2.0                ,lifeYPos,0));
	localVertices.push_back(vec3(-lifeWidth/2.0+lifeWidth*ratio,lifeYPos + lifeHeight,0));
	localVertices.push_back(vec3(-lifeWidth/2.0+lifeWidth*ratio,lifeYPos,0));
	myColor =0x00FF00FF;
	if (ratio < 0.2f) {
		myColor = 0xFF0000FF;
	} else if (ratio < 0.4f) {
		myColor = 0xDA7B00FF;
	}
	_draw->draw(localVertices, myColor, transformationMatrix, false, false);
}

static void drawShape(const btCollisionShape* _shape,
                      const ewol::object::Shared<ewol::resource::Colored3DObject>& _draw,
                      mat4 _transformationMatrix,
                      std::vector<vec3> _tmpVertices) {
	if(    nullptr == _draw
	    || nullptr == _shape) {
		return;
	}
	etk::Color<float> tmpColor(1.0, 0.0, 0.0, 0.3);
	
	//EGE_DEBUG("        draw (6): !btIDebugDraw::DBG_DrawWireframe");
	int shapetype=_shape->getShapeType();
	switch (shapetype) {
		case SPHERE_SHAPE_PROXYTYPE: {
			// Sphere collision shape ...
			//EGE_DEBUG("            draw (01): SPHERE_SHAPE_PROXYTYPE");
			const btSphereShape* sphereShape = static_cast<const btSphereShape*>(_shape);
			float radius = sphereShape->getMargin();//radius doesn't include the margin, so draw with margin
			drawSphere(_draw, radius, 10, 10, _transformationMatrix, tmpColor);
			break;
		}
		case BOX_SHAPE_PROXYTYPE: {
			// Box collision shape ...
			//EGE_DEBUG("            draw (02): BOX_SHAPE_PROXYTYPE");
			const btBoxShape* boxShape = static_cast<const btBoxShape*>(_shape);
			btVector3 halfExtent = boxShape->getHalfExtentsWithMargin();
			static int indices[36] = { 0,1,2,	3,2,1,	4,0,6,
			                           6,0,2,	5,1,4,	4,1,0,
			                           7,3,1,	7,1,5,	5,4,7,
			                           7,4,6,	7,2,3,	7,6,2};
			vec3 vertices[8]={ vec3(halfExtent[0],halfExtent[1],halfExtent[2]),
			                   vec3(-halfExtent[0],halfExtent[1],halfExtent[2]),
			                   vec3(halfExtent[0],-halfExtent[1],halfExtent[2]),
			                   vec3(-halfExtent[0],-halfExtent[1],halfExtent[2]),
			                   vec3(halfExtent[0],halfExtent[1],-halfExtent[2]),
			                   vec3(-halfExtent[0],halfExtent[1],-halfExtent[2]),
			                   vec3(halfExtent[0],-halfExtent[1],-halfExtent[2]),
			                   vec3(-halfExtent[0],-halfExtent[1],-halfExtent[2])};
			_tmpVertices.clear();
			for (int32_t iii=0 ; iii<36 ; iii+=3) {
				// normal calculation :
				//btVector3 normal = (vertices[indices[iii+2]]-vertices[indices[iii]]).cross(vertices[indices[iii+1]]-vertices[indices[iii]]);
				//normal.normalize ();
				_tmpVertices.push_back(vertices[indices[iii]]);
				_tmpVertices.push_back(vertices[indices[iii+1]]);
				_tmpVertices.push_back(vertices[indices[iii+2]]);
			}
			_draw->draw(_tmpVertices, tmpColor, _transformationMatrix);
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
					if (nullptr!=poly) {
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
			//EGE_DEBUG("            draw (07): COMPOUND_SHAPE_PROXYTYPE");
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
			//EGE_DEBUG("            draw (08): EMPTY_SHAPE_PROXYTYPE");
			// nothing to display ...
			break;
		}
		default: {
			// must be done later ...
			EGE_DEBUG("            draw (09): default");
		}
	}
}

void ege::ElementGame::drawDebug(const ewol::object::Shared<ewol::resource::Colored3DObject>& _draw, const ege::Camera& _camera) {
	m_debugText.clear();
	m_debugText.setColor(0x00FF00FF);
	m_debugText.setPos(vec3(-20,32,0));
	m_debugText.print(getType());
	m_debugText.setPos(vec3(-20,20,0));
	m_debugText.print("life=("+std::to_string(getLifeRatio()));
	//m_debugText.print(std::string("Axe=(")+std::string(m_tmpAxe.x())+std::string(",")+etk::UString(m_tmpAxe.y())+etk::UString(",")+etk::UString(m_tmpAxe.z())+etk::UString(")"));
	btScalar mmm[16];
	btDefaultMotionState* myMotionState = (btDefaultMotionState*)m_body->getMotionState();
	myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(mmm);
	
	mat4 transformationMatrix(mmm);
	transformationMatrix.transpose();
	
	// note : set the vertice here to prevent multiple allocations...
	std::vector<vec3> EwolVertices;
	drawShape(m_shape, _draw, transformationMatrix, EwolVertices);
	
	m_debugText.draw(   etk::matTranslate(getPosition())
	                  * etk::matRotate(vec3(0,0,1),_camera.getAngleZ())
	                  * etk::matRotate(vec3(1,0,0),(M_PI/2.0f-_camera.getAngleTeta()))
	                  * etk::matScale(vec3(0.05,0.05,0.05)));
}

void ege::ElementGame::draw(int32_t _pass) {
	if (false == m_elementInPhysicsSystem) {
		return;
	}
	if (_pass == 0) {
		if(    nullptr != m_body
		    && nullptr != m_mesh
		    && m_body->getMotionState() ) {
			btScalar mmm[16];
			btDefaultMotionState* myMotionState = (btDefaultMotionState*)m_body->getMotionState();
			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(mmm);
			
			mat4 transformationMatrix(mmm);
			transformationMatrix.transpose();
			m_mesh->draw(transformationMatrix);
		}
	}
}

void ege::ElementGame::dynamicEnable() {
	if (true == m_elementInPhysicsSystem) {
		return;
	}
	if(nullptr!=m_body) {
		m_env.getDynamicWorld()->addRigidBody(m_body);
	}
	if(nullptr!=m_IA) {
		m_env.getDynamicWorld()->addAction(m_IA);
	}
	m_elementInPhysicsSystem = true;
}

void ege::ElementGame::dynamicDisable() {
	if (false == m_elementInPhysicsSystem) {
		return;
	}
	if(nullptr!=m_IA) {
		m_env.getDynamicWorld()->removeAction(m_IA);
	}
	if(nullptr!=m_body) {
		// Unlink element from the engine
		m_env.getDynamicWorld()->removeRigidBody(m_body);
		m_env.getDynamicWorld()->removeCollisionObject(m_body);
	}
	m_elementInPhysicsSystem = false;
}

void ege::ElementGame::iaEnable() {
	if (nullptr != m_IA) {
		// IA already started ...
		return;
	}
	m_IA = new localIA(*this);
	if (nullptr == m_IA) {
		EGE_ERROR("Can not start the IA  == > allocation error");
		return;
	}
	if (true == m_elementInPhysicsSystem) {
		m_env.getDynamicWorld()->addAction(m_IA);
	}
}

void ege::ElementGame::iaDisable() {
	if (nullptr == m_IA) {
		// IA already stopped ...
		return;
	}
	if (true == m_elementInPhysicsSystem) {
		m_env.getDynamicWorld()->removeAction(m_IA);
	}
	// remove IA :
	delete(m_IA);
	m_IA = nullptr;
}




