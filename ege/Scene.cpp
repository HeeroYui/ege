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


#define  WALK_FLAG_FORWARD  (1<<0)
#define  WALK_FLAG_BACK     (1<<1)
#define  WALK_FLAG_LEFT     (1<<2)
#define  WALK_FLAG_RIGHT    (1<<3)
#define  WALK_FLAG_CAUTION  (1<<4)

ege::Scene::Scene(btDefaultCollisionConfiguration* _collisionConfiguration,
                  btCollisionDispatcher* _dispatcher,
                  btBroadphaseInterface* _broadphase,
                  btConstraintSolver* _solver,
                  btDynamicsWorld* _dynamicsWorld,
                  ege::Camera* _camera) :
	m_gameTime(0),
	m_angleView(M_PI/3.0),
	m_finger_DoubleTouch(false),
	m_dynamicsWorld(NULL),
	m_camera(NULL),
	m_isRunning(true),
	m_walk(0),
	m_debugMode(false),
	m_debugDrawing(NULL)
{
	SetKeyboardRepeate(false);
	SetCanHaveFocus(true);
	PeriodicCallEnable();
	AddEventId(eventPlayTimeChange);
	AddEventId(eventKillEnemy);
	
	ewol::resource::Keep(m_debugDrawing);
	
	m_ratioTime = 1.0f;
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
	ewol::resource::Release(m_debugDrawing);
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

#define  WALK_FLAG_FORWARD  (1<<0)
#define  WALK_FLAG_BACK     (1<<1)
#define  WALK_FLAG_LEFT     (1<<2)
#define  WALK_FLAG_RIGHT    (1<<3)
#define  WALK_FLAG_CAUTION  (1<<4)

static const float l_walkRatio = 15;
static const float l_walkLateralRatio = 15;

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
	m_camera->PeriodicCall(curentDelta);
	
	//EGE_DEBUG("stepSimulation (start)");
	///step the simulation
	if (m_dynamicsWorld) {
		m_dynamicsWorld->stepSimulation(curentDelta);
		//optional but useful: debug drawing
		m_dynamicsWorld->debugDrawWorld();
	}
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
	if (m_walk!=0) {
		float walkValue = 0;
		if(    (m_walk&WALK_FLAG_FORWARD)!=0
		    && (m_walk&WALK_FLAG_BACK)!=0) {
			// request back and forward in the same time ... this is really bad ....
			walkValue = 0;
		} else if ( (m_walk&WALK_FLAG_FORWARD)!=0) {
			walkValue = 1;
		} else if ( (m_walk&WALK_FLAG_BACK)!=0) {
			walkValue = -1;
		}
		if (walkValue!=0) {
			float angleZ = m_camera->GetAngleZ();
			vec3 offsetPosition( cosf(angleZ-M_PI/2.0)*walkValue,
			                    -sinf(angleZ-M_PI/2.0)*walkValue,
			                    0);
			//EWOL_DEBUG("Walk : " << ((int32_t)(angles.z/M_PI*180+180)%360-180) << " ==> " << angles);
			// walk is 6 km/h
			vec3 pos = m_camera->GetEye() + offsetPosition*l_walkRatio*curentDelta;
			m_camera->SetEye(pos);
		}
		walkValue=0;
		if(    (m_walk&WALK_FLAG_LEFT)!=0
		    && (m_walk&WALK_FLAG_RIGHT)!=0) {
			// request left and right in the same time ... this is really bad ....
			walkValue=0;
		} else if ( (m_walk&WALK_FLAG_LEFT)!=0) {
			walkValue = 1;
		} else if ( (m_walk&WALK_FLAG_RIGHT)!=0) {
			walkValue = -1;
		}
		if (walkValue != 0) {
			float angleZ = m_camera->GetAngleZ();
			vec3 offsetPosition( cosf(angleZ)*walkValue,
			                    -sinf(angleZ)*walkValue,
			                    0);
			//EWOL_DEBUG("Walk : " << ((int32_t)(angles.z/M_PI*180+180)%360-180) << " ==> " << angles);
			// lateral walk is 4 km/h
			vec3 pos = m_camera->GetEye() + offsetPosition*l_walkLateralRatio*curentDelta;
			m_camera->SetEye(pos);
		}
	}
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

bool ege::Scene::OnEventInput(const ewol::EventInput& _event)
{
	//EWOL_DEBUG("Input event : " << _event);
	vec2 relPos = RelativePosition(_event.GetPos());
	/*
	 *    *             
	 *    |\            
	 *    | \           
	 *    |  \          
	 *    |   \         
	 *    |    \        
	 *    |     \       
	 *    *------*      
	 *        \ \       
	 *         \ \      
	 *          \ \     
	 *           *-*    
	 */
	if (_event.GetType() == ewol::keyEvent::typeMouse) {
		if (0 != _event.GetId()) {
			KeepFocus();
		}
		//EGE_DEBUG("Move mouse at position = " << relPos << " ==> " << curentCreatingPosition );
		if (1 == _event.GetId()) {
			
		} else if (2 == _event.GetId()) {
			// center button : change the angle the camara
			if (ewol::keyEvent::statusMove == _event.GetStatus()) {
				vec2 tmppPos = relPos-m_centerButtonStartPos;
				tmppPos *= M_PI/(360.0f*6);
				m_camera->SetAngleZ(m_camera->GetAngleZ()- tmppPos.x());
				m_camera->SetAngleTeta(m_camera->GetAngleTeta()-tmppPos.y());
			}
			// update register position ...
			m_centerButtonStartPos = relPos;
		} else if (3 == _event.GetId()) {
			// center button : change the angle the camara
			if (ewol::keyEvent::statusMove == _event.GetStatus()) {
				vec3 nextPosition = ConvertScreenPositionInMapPosition(relPos);
				vec3 tmppPos = nextPosition-m_finger_StartPosMoving;
				vec3 oldposition = m_camera->GetEye();
				// update the camera positions:
				oldposition.setX(oldposition.x() - tmppPos.x());
				oldposition.setY(oldposition.y() - tmppPos.y());
				// set the new position
				m_camera->SetEye(oldposition);
			}
			// update register position ...
			m_leftButtonStartPos = relPos;
			m_finger_StartPosMoving = ConvertScreenPositionInMapPosition(relPos);
		} else if (4 == _event.GetId()) {
			if (ewol::keyEvent::statusSingle == _event.GetStatus()) {
				// scrool input
				float cameraDistance = m_camera->GetDistance()-3;
				EGE_DEBUG("New camera distance : " << etk_avg(10, cameraDistance, 100));
				m_camera->SetDistance(etk_avg(10, cameraDistance, 100));
			}
		} else if (5 == _event.GetId()) {
			if (ewol::keyEvent::statusSingle == _event.GetStatus()) {
				// scrool output
				float cameraDistance = m_camera->GetDistance()+3;
				EGE_DEBUG("New camera distance : " << etk_avg(10, cameraDistance, 100));
				m_camera->SetDistance(etk_avg(10, cameraDistance, 100));
			}
		}
	/*
	 *                         
	 *     ---                 
	 *    /   \                
	 *    |   |                
	 *    |   |                
	 *    |   |                
	 *    |   |                
	 *    |   | ---  ---  ---  
	 *    |   |/   \/   \/   \ 
	 *    |                  | 
	 *   /|                  | 
	 *  / |                  | 
	 *  |                    | 
	 *  |                    | 
	 *  |                    | 
	 *   \                  /  
	 *    \                /   
	 *     \              /    
	 *      \            /     
	 */
	} else if (_event.GetType() == ewol::keyEvent::typeFinger) {
		KeepFocus();
		if (1 == _event.GetId()) {
			if (m_finger_DoubleTouch==false) {
				
			} else {
				if (ewol::keyEvent::statusMove == _event.GetStatus()) {
					m_finger_1Position = relPos;
					if (m_finger_2Position.x() > -10000) {
						vec2 distance = m_finger_1Position-m_finger_2Position;
						float realDistance = distance.length();
						float fingerAngle = acosf(etk_avg(-1.0f, (distance.x()/realDistance), 1.0f) );
						if (distance.y()<0){
							fingerAngle *=-1;
						}
						realDistance /= 2.0f;
						if (m_finger_oldDistance>=0) {
							float distanceDelta = m_finger_oldDistance-realDistance;
							m_camera->SetDistance(etk_avg(10,m_camera->GetDistance()+distanceDelta/3.0f,100));
							float angleDelta = m_finger_oldAngle - fingerAngle;
							m_camera->SetAngleZ(m_camera->GetAngleZ()+angleDelta);
						}
						m_finger_oldDistance = realDistance;
						m_finger_oldAngle = fingerAngle;
					}
				}
			}
			m_finger_StartPosMoving = ConvertScreenPositionInMapPosition(relPos);
			if (ewol::keyEvent::statusUp == _event.GetStatus()) {
				m_finger_DoubleTouch = false;
			}
		} else if (2 == _event.GetId()) {
			if (ewol::keyEvent::statusDown == _event.GetStatus()) {
				m_finger_DoubleTouch = true;
				m_finger_1Position = vec2(-500000,-500000);
				m_finger_2Position = vec2(-500000,-500000);
				m_finger_oldDistance = -1;
				m_finger_oldAngle = 0;
			} else if (ewol::keyEvent::statusMove == _event.GetStatus()) {
				m_finger_2Position = relPos;
				if (m_finger_1Position.x() > -10000) {
					vec2 distance = m_finger_1Position-m_finger_2Position;
					float realDistance = distance.length();
					float fingerAngle = acosf(etk_avg(-1.0f, (distance.x()/realDistance), 1.0f) );
					if (distance.y()<0){
						fingerAngle *=-1;
					}
					realDistance /= 2.0f;
					if (m_finger_oldDistance>=0) {
						float distanceDelta = m_finger_oldDistance-realDistance;
						m_camera->SetDistance(etk_avg(10,m_camera->GetDistance()+distanceDelta/3.0f,100));
						float angleDelta = m_finger_oldAngle - fingerAngle;
						m_camera->SetAngleZ(m_camera->GetAngleZ()+angleDelta);
					}
					m_finger_oldDistance = realDistance;
					m_finger_oldAngle = fingerAngle;
				}
			} else if (ewol::keyEvent::statusUp == _event.GetStatus()) {
				m_finger_DoubleTouch = false;
			}
		}
	}
	
	return false;
}


bool ege::Scene::OnEventEntry(const ewol::EventEntry& _event)
{
	if (_event.GetType() == ewol::keyEvent::keyboardChar) {
		EWOL_DEBUG("Entry enevnt : " << _event );
		
		if(    _event.GetChar() == 'z'
		    || _event.GetChar() == 'Z') {
			if (_event.GetStatus() == ewol::keyEvent::statusDown) {
				m_walk |= WALK_FLAG_FORWARD;
			}
			if (_event.GetStatus() == ewol::keyEvent::statusUp) {
				if ((m_walk&WALK_FLAG_FORWARD) != 0) {
					m_walk -= WALK_FLAG_FORWARD;
				}
			}
		}
		if(    _event.GetChar() == 's'
		    || _event.GetChar() == 'S') {
			if (_event.GetStatus() == ewol::keyEvent::statusDown) {
				m_walk |= WALK_FLAG_BACK;
			}
			if (_event.GetStatus() == ewol::keyEvent::statusUp) {
				if ((m_walk&WALK_FLAG_BACK) != 0) {
					m_walk -= WALK_FLAG_BACK;
				}
			}
		}
		if(    _event.GetChar() == 'q'
		    || _event.GetChar() == 'Q') {
			if (_event.GetStatus() == ewol::keyEvent::statusDown) {
				m_walk |= WALK_FLAG_LEFT;
			}
			if (_event.GetStatus() == ewol::keyEvent::statusUp) {
				if ((m_walk&WALK_FLAG_LEFT) != 0) {
					m_walk -= WALK_FLAG_LEFT;
				}
			}
		}
		if(    _event.GetChar() == 'd'
		    || _event.GetChar() == 'D') {
			if (_event.GetStatus() == ewol::keyEvent::statusDown) {
				m_walk |= WALK_FLAG_RIGHT;
			}
			if (_event.GetStatus() == ewol::keyEvent::statusUp) {
				if ((m_walk&WALK_FLAG_RIGHT) != 0) {
					m_walk -= WALK_FLAG_RIGHT;
				}
			}
		}
		EWOL_DEBUG("m_walk=" << m_walk);
		return false;
	}
	// Move event ...
	if (_event.GetType() == ewol::keyEvent::keyboardUp) {
		if (_event.GetStatus() == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_FORWARD;
		}
		if (_event.GetStatus() == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_FORWARD) != 0) {
				m_walk -= WALK_FLAG_FORWARD;
			}
		}
	}
	if (_event.GetType() == ewol::keyEvent::keyboardDown) {
		if (_event.GetStatus() == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_BACK;
		}
		if (_event.GetStatus() == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_BACK) != 0) {
				m_walk -= WALK_FLAG_BACK;
			}
		}
	}
	if (_event.GetType() == ewol::keyEvent::keyboardLeft) {
		if (_event.GetStatus() == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_LEFT;
		}
		if (_event.GetStatus() == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_LEFT) != 0) {
				m_walk -= WALK_FLAG_LEFT;
			}
		}
	}
	if (_event.GetType() == ewol::keyEvent::keyboardRight) {
		if (_event.GetStatus() == ewol::keyEvent::statusDown) {
			m_walk |= WALK_FLAG_RIGHT;
		}
		if (_event.GetStatus() == ewol::keyEvent::statusUp) {
			if ((m_walk&WALK_FLAG_RIGHT) != 0) {
				m_walk -= WALK_FLAG_RIGHT;
			}
		}
	}
	EWOL_DEBUG("m_walk=" << m_walk);
	return false;
}


void ege::Scene::OnGetFocus(void)
{
	
}

void ege::Scene::OnLostFocus(void)
{
	
}


