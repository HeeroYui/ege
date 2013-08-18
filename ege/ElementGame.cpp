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
#include <ewol/renderer/ResourceManager.h>
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

#undef __class__
#define __class__	"ElementGame"

const etk::UString& ege::ElementGame::GetType(void) const
{
	static const etk::UString nameType("----");
	return nameType;
}


ege::ElementGame::ElementGame(ege::Environement& _env) :
	m_env(_env),
	m_body(NULL),
	m_shape(NULL),
	m_life(100),
	m_lifeMax(100),
	m_group(0),
	m_fixe(true),
	m_radius(0),
	m_elementInPhysicsSystem(false),
	m_IA(NULL)
{
	static uint32_t unique=0;
	m_uID = unique;
	EGE_DEBUG("Create element : uId=" << m_uID);
	unique++;
}

ege::ElementGame::~ElementGame(void)
{
	// in every case remove IA
	IADisable();
	// same ...
	DynamicDisable();
	EGE_DEBUG("Destroy element : uId=" << m_uID);
}


float ege::ElementGame::GetLifeRatio(void)
{
	if (0>=m_life) {
		return 0;
	}
	return m_life/m_lifeMax;
}


void ege::ElementGame::SetFireOn(int32_t groupIdSource, int32_t type, float power, const vec3& center)
{
	float previousLife = m_life;
	m_life += power;
	m_life = etk_avg(0, m_life, m_lifeMax);
	if (m_life<=0) {
		EGE_DEBUG("[" << GetUID() << "] element is killed ..." << GetType());
	}
	if (m_life!=previousLife) {
		OnLifeChange();
	}
}

void ege::ElementGame::SetPosition(const vec3& pos)
{
	if (NULL!=m_body) {
		btTransform transformation = m_body->getCenterOfMassTransform();
		transformation.setOrigin(pos);
		m_body->setCenterOfMassTransform(transformation);
	}
}

const vec3& ege::ElementGame::GetPosition(void)
{
	// this is to prevent error like segmentation fault ...
	static vec3 emptyPosition(-1000000,-1000000,-1000000);
	if (NULL!=m_body) {
		return m_body->getCenterOfMassPosition();
	}
	return emptyPosition;
};

const vec3& ege::ElementGame::GetSpeed(void)
{
	// this is to prevent error like segmentation fault ...
	static vec3 emptySpeed(0,0,0);
	if (NULL!=m_body) {
		return m_body->getLinearVelocity();
	}
	return emptySpeed;
};

const float ege::ElementGame::GetInvMass(void)
{
	if (NULL!=m_body) {
		return m_body->getInvMass();
	}
	return 0.0000000001f;
};


static void DrawSphere(ewol::Colored3DObject* _draw,
                       btScalar _radius,
                       int _lats,
                       int _longs,
                       mat4& _transformationMatrix,
                       etk::Color<float>& _tmpColor)
{
	int i, j;
	etk::Vector<vec3> EwolVertices;
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
			
			EwolVertices.PushBack(v1);
			EwolVertices.PushBack(v2);
			EwolVertices.PushBack(v3);
			
			EwolVertices.PushBack(v1);
			EwolVertices.PushBack(v3);
			EwolVertices.PushBack(v4);
		}
	}
	_draw->Draw(EwolVertices, _tmpColor, _transformationMatrix);
}

const float lifeBorder = 0.1f;
const float lifeHeight = 0.3f;
const float lifeWidth = 2.0f;
const float lifeYPos = 1.7f;

void ege::ElementGame::DrawLife(ewol::Colored3DObject* _draw, const ege::Camera& _camera)
{
	if (NULL==_draw) {
		return;
	}
	float ratio = GetLifeRatio();
	if (ratio == 1.0f) {
		return;
	}
	mat4 transformationMatrix =   etk::matTranslate(GetPosition())
	                            * etk::matRotate(vec3(0,0,1),_camera.GetAngleZ())
	                            * etk::matRotate(vec3(1,0,0),(M_PI/2.0f-_camera.GetAngleTeta()));
	etk::Vector<vec3> localVertices;
	localVertices.PushBack(vec3(-lifeWidth/2.0-lifeBorder,lifeYPos           -lifeBorder,0));
	localVertices.PushBack(vec3(-lifeWidth/2.0-lifeBorder,lifeYPos+lifeHeight+lifeBorder,0));
	localVertices.PushBack(vec3( lifeWidth/2.0+lifeBorder,lifeYPos+lifeHeight+lifeBorder,0));
	localVertices.PushBack(vec3(-lifeWidth/2.0-lifeBorder,lifeYPos           -lifeBorder,0));
	localVertices.PushBack(vec3( lifeWidth/2.0+lifeBorder,lifeYPos+lifeHeight+lifeBorder,0));
	localVertices.PushBack(vec3( lifeWidth/2.0+lifeBorder,lifeYPos           -lifeBorder,0));
	etk::Color<float> myColor(0x0000FF99);
	_draw->Draw(localVertices, myColor, transformationMatrix, false, false);
	localVertices.Clear();
	/** Bounding box ==> model shape **/
	localVertices.PushBack(vec3(-lifeWidth/2.0                ,lifeYPos,0));
	localVertices.PushBack(vec3(-lifeWidth/2.0                ,lifeYPos + lifeHeight,0));
	localVertices.PushBack(vec3(-lifeWidth/2.0+lifeWidth*ratio,lifeYPos + lifeHeight,0));
	localVertices.PushBack(vec3(-lifeWidth/2.0                ,lifeYPos,0));
	localVertices.PushBack(vec3(-lifeWidth/2.0+lifeWidth*ratio,lifeYPos + lifeHeight,0));
	localVertices.PushBack(vec3(-lifeWidth/2.0+lifeWidth*ratio,lifeYPos,0));
	myColor =0x00FF00FF;
	if (ratio < 0.2f) {
		myColor = 0xFF0000FF;
	} else if (ratio < 0.4f) {
		myColor = 0xDA7B00FF;
	}
	_draw->Draw(localVertices, myColor, transformationMatrix, false, false);
}

static void DrawShape(const btCollisionShape* _shape,
                      ewol::Colored3DObject* _draw,
                      mat4 _transformationMatrix,
                      etk::Vector<vec3> _tmpVertices)
{
	if(    NULL == _draw
	    || NULL == _shape) {
		return;
	}
	etk::Color<float> tmpColor(1.0, 0.0, 0.0, 0.3);
	
	//EGE_DEBUG("        Draw (6): !btIDebugDraw::DBG_DrawWireframe");
	int shapetype=_shape->getShapeType();
	switch (shapetype) {
		case SPHERE_SHAPE_PROXYTYPE: {
			// Sphere collision shape ...
			//EGE_DEBUG("            Draw (01): SPHERE_SHAPE_PROXYTYPE");
			const btSphereShape* sphereShape = static_cast<const btSphereShape*>(_shape);
			float radius = sphereShape->getMargin();//radius doesn't include the margin, so draw with margin
			DrawSphere(_draw, radius, 10, 10, _transformationMatrix, tmpColor);
			break;
		}
		case BOX_SHAPE_PROXYTYPE: {
			// Box collision shape ...
			//EGE_DEBUG("            Draw (02): BOX_SHAPE_PROXYTYPE");
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
			_tmpVertices.Clear();
			for (int32_t iii=0 ; iii<36 ; iii+=3) {
				// normal calculation :
				//btVector3 normal = (vertices[indices[iii+2]]-vertices[indices[iii]]).cross(vertices[indices[iii+1]]-vertices[indices[iii]]);
				//normal.normalize ();
				_tmpVertices.PushBack(vertices[indices[iii]]);
				_tmpVertices.PushBack(vertices[indices[iii+1]]);
				_tmpVertices.PushBack(vertices[indices[iii+2]]);
			}
			_draw->Draw(_tmpVertices, tmpColor, _transformationMatrix);
			break;
		}
		case CONE_SHAPE_PROXYTYPE: {
			// Cone collision shape ...
			EGE_DEBUG("            Draw (03): CONE_SHAPE_PROXYTYPE");
			break;
		}
		case CAPSULE_SHAPE_PROXYTYPE: {
			// Capsule collision shape ...
			EGE_DEBUG("            Draw (04): CAPSULE_SHAPE_PROXYTYPE");
			break;
		}
		case CYLINDER_SHAPE_PROXYTYPE: {
			// Cylinder collision shape ...
			EGE_DEBUG("            Draw (05): CYLINDER_SHAPE_PROXYTYPE");
			break;
		}
		case CONVEX_HULL_SHAPE_PROXYTYPE: {
			// Convex Hull collision shape ...
			EGE_DEBUG("            Draw (06): CONVEX_HULL_SHAPE_PROXYTYPE");
			if (_shape->isConvex()) {
					EGE_DEBUG("                shape->isConvex()");
					const btConvexPolyhedron* poly = _shape->isPolyhedral() ? ((btPolyhedralConvexShape*) _shape)->getConvexPolyhedron() : 0;
					if (NULL!=poly) {
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
						// TODO : Set it back ...
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
			//EGE_DEBUG("            Draw (07): COMPOUND_SHAPE_PROXYTYPE");
			const btCompoundShape* compoundShape = static_cast<const btCompoundShape*>(_shape);
			for (int32_t iii=compoundShape->getNumChildShapes()-1;iii>=0;iii--) {
				btTransform childTrans = compoundShape->getChildTransform(iii);
				const btCollisionShape* colShape = compoundShape->getChildShape(iii);
				btScalar mmm[16];
				childTrans.getOpenGLMatrix(mmm);
				mat4 transformationMatrix(mmm);
				transformationMatrix.Transpose();
				transformationMatrix = _transformationMatrix * transformationMatrix;
				DrawShape(colShape, _draw, transformationMatrix, _tmpVertices);
			}
			break;
		}
		case EMPTY_SHAPE_PROXYTYPE: {
			// No collision shape ...
			//EGE_DEBUG("            Draw (08): EMPTY_SHAPE_PROXYTYPE");
			// nothing to display ...
			break;
		}
		default: {
			// must be done later ...
			EGE_DEBUG("            Draw (09): default");
		}
	}
}

void ege::ElementGame::DrawDebug(ewol::Colored3DObject* _draw, const ege::Camera& _camera)
{
	m_debugText.Clear();
	m_debugText.SetFontSize(12);
	m_debugText.SetColor(0x00FF00FF);
	m_debugText.SetPos(vec3(-20,32,0));
	m_debugText.Print(GetType());
	m_debugText.SetPos(vec3(-20,20,0));
	m_debugText.Print(etk::UString("life=(")+etk::UString(GetLifeRatio()));
	//m_debugText.Print(etk::UString("Axe=(")+etk::UString(m_tmpAxe.x())+etk::UString(",")+etk::UString(m_tmpAxe.y())+etk::UString(",")+etk::UString(m_tmpAxe.z())+etk::UString(")"));
	btScalar mmm[16];
	btDefaultMotionState* myMotionState = (btDefaultMotionState*)m_body->getMotionState();
	myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(mmm);
	
	mat4 transformationMatrix(mmm);
	transformationMatrix.Transpose();
	
	// note : set the vertice here to prevent multiple allocations...
	etk::Vector<vec3> EwolVertices;
	DrawShape(m_shape, _draw, transformationMatrix, EwolVertices);
	
	m_debugText.Draw(   etk::matTranslate(GetPosition())
	                  * etk::matRotate(vec3(0,0,1),_camera.GetAngleZ())
	                  * etk::matRotate(vec3(1,0,0),(M_PI/2.0f-_camera.GetAngleTeta()))
	                  * etk::matScale(vec3(0.05,0.05,0.05)));
}


void ege::ElementGame::DynamicEnable(void)
{
	if (true == m_elementInPhysicsSystem) {
		return;
	}
	if(NULL!=m_body) {
		m_env.GetDynamicWorld()->addRigidBody(m_body);
	}
	if(NULL!=m_IA) {
		m_env.GetDynamicWorld()->addAction(m_IA);
	}
	m_elementInPhysicsSystem = true;
}

void ege::ElementGame::DynamicDisable(void)
{
	if (false == m_elementInPhysicsSystem) {
		return;
	}
	if(NULL!=m_IA) {
		m_env.GetDynamicWorld()->removeAction(m_IA);
	}
	if(NULL!=m_body) {
		// Unlink element from the engine
		m_env.GetDynamicWorld()->removeRigidBody(m_body);
		m_env.GetDynamicWorld()->removeCollisionObject(m_body);
	}
	m_elementInPhysicsSystem = false;
}

void ege::ElementGame::IAEnable(void)
{
	if (NULL != m_IA) {
		// IA already started ...
		return;
	}
	m_IA = new localIA(*this);
	if (NULL == m_IA) {
		EGE_ERROR("Can not start the IA ==> allocation error");
		return;
	}
	if (true == m_elementInPhysicsSystem) {
		m_env.GetDynamicWorld()->addAction(m_IA);
	}
}

void ege::ElementGame::IADisable(void)
{
	if (NULL == m_IA) {
		// IA already stopped ...
		return;
	}
	if (true == m_elementInPhysicsSystem) {
		m_env.GetDynamicWorld()->removeAction(m_IA);
	}
	// Remove IA :
	delete(m_IA);
	m_IA = NULL;
}




