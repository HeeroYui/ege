/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/Scene.h>

#include <math.h>
#include <ege/debug.h>
#include <ewol/ewol.h>
#include <ewol/renderer/openGL.h>
#include <etk/math/Matrix4.h>
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
#define __class__	"Scene"


const char * const ege::Scene::eventPlayTimeChange = "event-scene-play-time-change";
const char * const ege::Scene::eventKillEnemy = "event-scene-kill-ennemy";



ege::Scene::Scene(bool _setAutoBullet, bool _setAutoCamera) :
	m_gameTime(0),
	m_angleView(M_PI/3.0),
	m_dynamicsWorld(NULL),
	m_camera(NULL),
	m_isRunning(true),
	m_debugMode(false),
	m_debugDrawing(NULL)
{
	SetKeyboardRepeate(false);
	SetCanHaveFocus(true);
	PeriodicCallEnable();
	AddEventId(eventPlayTimeChange);
	AddEventId(eventKillEnemy);
	
	m_debugDrawing = ewol::Colored3DObject::Keep();
	
	m_ratioTime = 1.0f;
	if (_setAutoBullet==true) {
		SetBulletConfig();
	}
	if (_setAutoCamera==true) {
		SetCamera();
	}
}

void ege::Scene::SetBulletConfig(btDefaultCollisionConfiguration* _collisionConfiguration,
                                 btCollisionDispatcher* _dispatcher,
                                 btBroadphaseInterface* _broadphase,
                                 btConstraintSolver* _solver,
                                 btDynamicsWorld* _dynamicsWorld)
{
	if (NULL != _collisionConfiguration) {
		m_collisionConfiguration = _collisionConfiguration;
	} else {
		m_collisionConfiguration = new btDefaultCollisionConfiguration();
	}
	///use the default collision dispatcher.
	if (NULL != _dispatcher) {
		m_dispatcher = _dispatcher;
	} else {
		m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	}
	if (NULL != _broadphase) {
		m_broadphase = _broadphase;
	} else {
		m_broadphase = new btDbvtBroadphase();
	}
	
	///the default constraint solver.
	if (NULL != _solver) {
		m_solver = _solver;
	} else {
		m_solver = new btSequentialImpulseConstraintSolver();
	}
	
	if (NULL != _dynamicsWorld) {
		m_dynamicsWorld = _dynamicsWorld;
	} else {
		m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
		// By default we set no gravity
		m_dynamicsWorld->setGravity(btVector3(0,0,0));
	}
	
	m_env.SetDynamicWorld(m_dynamicsWorld);
	
}

void ege::Scene::SetCamera(ege::Camera* _camera)
{
	if (NULL != _camera) {
		m_camera = _camera;
	} else {
		m_camera = new ege::Camera(vec3(0,0,0), 0, DEG_TO_RAD(45) ,50);
		// SET THE STATION ..
		m_camera->SetEye(vec3(0,0,0));
	}
}

ege::Scene::~Scene(void)
{
	ewol::Colored3DObject::Release(m_debugDrawing);
/*
	ewol::resource::Release(m_directDrawObject);
	//cleanup in the reverse order of creation/initialization
	//remove the rigidbodies from the dynamics world and delete them
	for (int32_t iii=m_dynamicsWorld->getNumCollisionObjects()-1; iii>=0 ;iii--) {
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[iii];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	delete m_dynamicsWorld;
	delete m_solver;
	delete m_broadphase;
	delete m_dispatcher;
	delete m_collisionConfiguration;
	*/
}


void ege::Scene::OnRegenerateDisplay(void)
{
	if (true == NeedRedraw()) {
		
	}
}


void ege::Scene::Pause(void)
{
	EGE_DEBUG("Set Pause");
	m_isRunning = false;
}


void ege::Scene::Resume(void)
{
	EGE_DEBUG("Set Resume");
	m_isRunning = true;
}


void ege::Scene::PauseToggle(void)
{
	if(true==m_isRunning) {
		EGE_DEBUG("Set Toggle: Pause");
		m_isRunning=false;
	} else {
		EGE_DEBUG("Set Toggle: Resume");
		m_isRunning=true;
	}
}

//#define SCENE_DISPLAY_SPEED

//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef SCENE_DISPLAY_SPEED
	static int64_t g_startTime = 0;
	static int32_t g_counterNbTimeDisplay = 0;
#endif

#define NUMBER_OF_SUB_PASS  (0)


void ege::Scene::OnDraw(void)
{
	#ifdef SCENE_DISPLAY_SPEED
		g_counterNbTimeDisplay++;
		g_startTime = ewol::GetTime();
	#endif
	//EGE_DEBUG("Draw (start)");
	mat4 tmpMatrix;
	if (m_dynamicsWorld) {
		m_env.GetOrderedElementForDisplay(m_displayElementOrdered, m_camera->GetOrigin(), m_camera->GetViewVector());
		//EGE_DEBUG("DRAW : " << m_displayElementOrdered.Size() << " elements");
		
		// TODO : Remove this ==> no more needed ==> checked in the Generate the list of the element ordered
		for (int32_t iii=0; iii<m_displayElementOrdered.Size(); iii++) {
			m_displayElementOrdered[iii].element->PreCalculationDraw(*m_camera);
		}
		// note :  the first pass is done at the reverse way to prevent multiple display od the same point in the screen 
		//         (and we remember that the first pass is to display all the non transparent elements)
		#if 0
			// note : We keep this one for the test only ...
			for (int32_t iii=0; iii<m_displayElementOrdered.Size(); iii++) {
		#else
			for (int32_t iii=m_displayElementOrdered.Size()-1; iii>=0; iii--) {
		#endif
			m_displayElementOrdered[iii].element->Draw(0);
		}
		// for the other pass the user can draw transparent elements ...
		for (int32_t pass=1; pass<=NUMBER_OF_SUB_PASS+1; pass++) {
			for (int32_t iii=0; iii<m_displayElementOrdered.Size(); iii++) {
				m_displayElementOrdered[iii].element->Draw(pass);
			}
		}
		for (int32_t iii=0; iii<m_displayElementOrdered.Size(); iii++) {
			m_displayElementOrdered[iii].element->DrawLife(m_debugDrawing, *m_camera);
		}
		#ifdef DEBUG
			if (true == m_debugMode) {
				for (int32_t iii=0; iii<m_displayElementOrdered.Size(); iii++) {
					m_displayElementOrdered[iii].element->DrawDebug(m_debugDrawing, *m_camera);
				}
			}
		#endif
	}
	m_env.GetParticuleEngine().Draw();
	#ifdef SCENE_DISPLAY_SPEED
		float localTime = (float)(ewol::GetTime() - g_startTime) / 1000.0f;
		if (localTime>1) {
			EWOL_ERROR("      scene : " << localTime << "ms " << g_counterNbTimeDisplay);
		} else {
			EWOL_DEBUG("      scene : " << localTime << "ms " << g_counterNbTimeDisplay);
		}
	#endif
	//EGE_DEBUG("Draw (stop)");
}

// I really does not know what is this ...
btRigidBody& btActionInterface::getFixedBody()
{
	static btRigidBody s_fixed(0, 0,0);
	s_fixed.setMassProps(btScalar(0.),btVector3(btScalar(0.),btScalar(0.),btScalar(0.)));
	return s_fixed;
}


void ege::Scene::PeriodicCall(const ewol::EventTime& _event)
{
	float curentDelta=_event.GetDeltaCall();
	// small hack to change speed ...
	if (m_ratioTime != 1) {
		curentDelta *= m_ratioTime;
	}
	// check if the processing is availlable
	if (false == m_isRunning) {
		MarkToRedraw();
		return;
	}
	// update game time:
	int32_t lastGameTime = m_gameTime;
	m_gameTime += curentDelta;
	if (lastGameTime != (int32_t)m_gameTime) {
		GenerateEventId(eventPlayTimeChange, m_gameTime);
	}
	
	//EWOL_DEBUG("Time: m_lastCallTime=" << m_lastCallTime << " deltaTime=" << deltaTime);
	
	// update camera positions:
	if (NULL != m_camera) {
		m_camera->PeriodicCall(curentDelta);
	}
	//EGE_DEBUG("stepSimulation (start)");
	///step the simulation
	if (m_dynamicsWorld) {
		m_dynamicsWorld->stepSimulation(curentDelta);
		//optional but useful: debug drawing
		m_dynamicsWorld->debugDrawWorld();
	}
	m_env.GetParticuleEngine().Update(curentDelta);
	// Remove all element that requested it ...
	{
		int32_t numberEnnemyKilled=0;
		int32_t victoryPoint=0;
		etk::Vector<ege::ElementGame*>& elementList = m_env.GetElementGame();
		for (int32_t iii=elementList.Size()-1; iii>=0; --iii) {
			if(NULL != elementList[iii]) {
				if (true == elementList[iii]->NeedToRemove()) {
					if (elementList[iii]->GetGroup() > 1) {
						numberEnnemyKilled++;
						victoryPoint++;
					}
					EGE_DEBUG("[" << elementList[iii]->GetUID() << "] element Removing ... " << elementList[iii]->GetType());
					m_env.RmElementGame(elementList[iii]);
				}
			}
		}
		
		if (0 != numberEnnemyKilled) {
			GenerateEventId(eventKillEnemy, numberEnnemyKilled);
		}
	}
	MarkToRedraw();
}

#define GAME_Z_NEAR  (1)
#define GAME_Z_FAR   (4000)

//#define SCENE_BRUT_PERFO_TEST

void ege::Scene::SystemDraw(const ewol::DrawProperty& _displayProp)
{
#ifdef SCENE_BRUT_PERFO_TEST
int64_t tmp___startTime0 = ewol::GetTime();
#endif
	ewol::openGL::Push();
	// here we invert the reference of the standard OpenGl view because the reference in the common display is Top left and not buttom left
	glViewport( m_origin.x(),
	            m_origin.y(),
	            m_size.x(),
	            m_size.y());
#ifdef SCENE_BRUT_PERFO_TEST
float tmp___localTime0 = (float)(ewol::GetTime() - tmp___startTime0) / 1000.0f;
EWOL_DEBUG("      SCENE000  : " << tmp___localTime0 << "ms ");
int64_t tmp___startTime1 = ewol::GetTime();
#endif
	float ratio = m_size.x() / m_size.y();
	//EWOL_INFO("ratio : " << ratio);
	mat4 tmpProjection = etk::matPerspective(m_angleView, ratio, GAME_Z_NEAR, GAME_Z_FAR);
#ifdef SCENE_BRUT_PERFO_TEST
float tmp___localTime1 = (float)(ewol::GetTime() - tmp___startTime1) / 1000.0f;
EWOL_DEBUG("      SCENE111  : " << tmp___localTime1 << "ms ");
int64_t tmp___startTime2 = ewol::GetTime();
#endif
	ewol::openGL::SetCameraMatrix(m_camera->GetMatrix());
	//mat4 tmpMat = tmpProjection * m_camera->GetMatrix();
	// set internal matrix system :
	//ewol::openGL::SetMatrix(tmpMat);
	ewol::openGL::SetMatrix(tmpProjection);
#ifdef SCENE_BRUT_PERFO_TEST
float tmp___localTime2 = (float)(ewol::GetTime() - tmp___startTime2) / 1000.0f;
EWOL_DEBUG("      SCENE222  : " << tmp___localTime2 << "ms ");
#endif
	
#ifdef SCENE_BRUT_PERFO_TEST
int64_t tmp___startTime3 = ewol::GetTime();
#endif
	OnDraw();
#ifdef SCENE_BRUT_PERFO_TEST
float tmp___localTime3 = (float)(ewol::GetTime() - tmp___startTime3) / 1000.0f;
EWOL_DEBUG("      SCENE333  : " << tmp___localTime3 << "ms ");
int64_t tmp___startTime4 = ewol::GetTime();
#endif
	ewol::openGL::Pop();
#ifdef SCENE_BRUT_PERFO_TEST
float tmp___localTime4 = (float)(ewol::GetTime() - tmp___startTime4) / 1000.0f;
EWOL_DEBUG("      SCENE444  : " << tmp___localTime4 << "ms ");
#endif
}

vec2 ege::Scene::CalculateDeltaAngle(const vec2& posScreen)
{
	double ratio = m_size.x() / m_size.y();
	vec2 pos = vec2(m_size.x()/-2.0, m_size.y()/-2.0) + posScreen;
	
	double xmax = tan(m_angleView/2.0);
	double ymax = xmax / ratio;
	
	double newX = pos.x() * xmax / m_size.x()*2.0;
	double newY = pos.y() * ymax / m_size.y()*2.0;
	
	double angleX = atan(newX);
	double angleY = atan(newY);
	
	return vec2(angleX,
	            angleY);
}

vec3 ege::Scene::ConvertScreenPositionInMapPosition(const vec2& posScreen)
{
	return m_camera->projectOnZGround(CalculateDeltaAngle(posScreen));
}



