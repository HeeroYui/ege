/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/widget/Scene.h>

#include <math.h>
#include <ege/debug.h>
#include <ewol/ewol.h>
#include <ewol/openGL/openGL.h>
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
#define __class__ "Scene"

ege::widget::Scene::Scene() //:
  //signalKillEnemy(*this, "kill-ennemy"),
  //m_gameTime(0),
  //m_angleView(M_PI/3.0),
  //m_dynamicsWorld(nullptr),
  //m_camera(nullptr),
  //m_debugMode(false),
  //m_debugDrawing(nullptr)
{
	addObjectType("ege::widget::Scene");
}

void ege::widget::Scene::init(std::shared_ptr<ege::Environement> _env) {
	m_env = _env;
	ewol::Widget::init();
	setKeyboardRepeate(false);
	setCanHaveFocus(true);
	periodicCallEnable();
	
	//m_debugDrawing = ewol::resource::Colored3DObject::create();
	
	/*
	m_ratioTime = 1.0f;
	if (_setAutoBullet == true) {
		setBulletConfig();
	}
	if (_setAutoCamera == true) {
		setCamera();
	}
	*/
}

/*
void ege::widget::Scene::setCamera(ege::Camera* _camera) {
	if (nullptr != _camera) {
		m_camera = _camera;
	} else {
		m_camera = new ege::Camera(vec3(0,0,0), 0, DEG_TO_RAD(45) ,50);
		// SET THE STATION ..
		m_camera->setEye(vec3(0,0,0));
	}
}
*/
ege::widget::Scene::~Scene() {
	/*
	ewol::resource::release(m_directDrawObject);
	//cleanup in the reverse order of creation/initialization
	//remove the rigidbodies from the dynamics world and delete them
	for (int32_t iii=m_dynamicsWorld->getNumCollisionObjects()-1; iii >= 0 ;iii--) {
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[iii];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}
	*/
}

void ege::widget::Scene::onRegenerateDisplay() {
	if (true == needRedraw()) {
		
	}
}

//#define SCENE_DISPLAY_SPEED

//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef SCENE_DISPLAY_SPEED
	static int64_t g_startTime = 0;
	static int32_t g_counterNbTimeDisplay = 0;
#endif

#define NUMBER_OF_SUB_PASS  (0)


void ege::widget::Scene::onDraw() {
	#ifdef SCENE_DISPLAY_SPEED
		g_counterNbTimeDisplay++;
		g_startTime = ewol::getTime();
	#endif
	// get camera :
	std::shared_ptr<ege::Camera> camera = m_env->getCamera(m_cameraName);
	//EGE_DEBUG("Draw (start)");
	mat4 tmpMatrix;
	std::shared_ptr<btDynamicsWorld> world = m_env->getDynamicWorld();
	if (world != nullptr) {
		
		m_env->getOrderedElementForDisplay(m_displayElementOrdered, camera->getOrigin(), camera->getViewVector());
		//EGE_DEBUG("DRAW : " << m_displayElementOrdered.size() << " elements");
		
		// TODO : remove this  == > no more needed ==> checked in the generate the list of the element ordered
		for (size_t iii=0; iii<m_displayElementOrdered.size(); iii++) {
			m_displayElementOrdered[iii].element->preCalculationDraw(*camera);
		}
		// note :  the first pass is done at the reverse way to prevent multiple display od the same point in the screen 
		//         (and we remember that the first pass is to display all the non transparent elements)
		#if 0
			// note : We keep this one for the test only ...
			for (int32_t iii=0; iii<m_displayElementOrdered.size(); iii++) {
		#else
			for (int32_t iii=m_displayElementOrdered.size()-1; iii >= 0; iii--) {
		#endif
			m_displayElementOrdered[iii].element->draw(0);
		}
		// for the other pass the user can draw transparent elements ...
		for (int32_t pass=1; pass <= NUMBER_OF_SUB_PASS+1; pass++) {
			for (size_t iii=0; iii<m_displayElementOrdered.size(); iii++) {
				m_displayElementOrdered[iii].element->draw(pass);
			}
		}
		/*
		for (size_t iii=0; iii<m_displayElementOrdered.size(); iii++) {
			m_displayElementOrdered[iii].element->drawLife(m_debugDrawing, *m_camera);
		}
		*/
		/*
		#ifdef DEBUG
			if (true == m_debugMode) {
				for (size_t iii=0; iii<m_displayElementOrdered.size(); iii++) {
					m_displayElementOrdered[iii].element->drawDebug(m_debugDrawing, *m_camera);
				}
			}
		#endif
		*/
	}
	if (camera != nullptr) {
		m_env->getParticuleEngine().draw(*camera);
	}
	#ifdef SCENE_DISPLAY_SPEED
		float localTime = (float)(ewol::getTime() - g_startTime) / 1000.0f;
		if (localTime>1) {
			EWOL_ERROR("      scene : " << localTime << "ms " << g_counterNbTimeDisplay);
		} else {
			EWOL_DEBUG("      scene : " << localTime << "ms " << g_counterNbTimeDisplay);
		}
	#endif
	//EGE_DEBUG("Draw (stop)");
}

// I really does not know what is this ...
btRigidBody& btActionInterface::getFixedBody() {
	static btRigidBody s_fixed(0, 0,0);
	s_fixed.setMassProps(btScalar(0.),btVector3(btScalar(0.),btScalar(0.),btScalar(0.)));
	return s_fixed;
}

void ege::widget::Scene::periodicCall(const ewol::event::Time& _event) {
	markToRedraw();
}

#define GAME_Z_NEAR  (1)
#define GAME_Z_FAR   (4000)

//#define SCENE_BRUT_PERFO_TEST

void ege::widget::Scene::systemDraw(const ewol::DrawProperty& _displayProp)
{
#ifdef SCENE_BRUT_PERFO_TEST
int64_t tmp___startTime0 = ewol::getTime();
#endif
	ewol::openGL::push();
	// here we invert the reference of the standard openGl view because the reference in the common display is Top left and not buttom left
	glViewport( m_origin.x(),
	            m_origin.y(),
	            m_size.x(),
	            m_size.y());
#ifdef SCENE_BRUT_PERFO_TEST
float tmp___localTime0 = (float)(ewol::getTime() - tmp___startTime0) / 1000.0f;
EWOL_DEBUG("      SCENE000  : " << tmp___localTime0 << "ms ");
int64_t tmp___startTime1 = ewol::getTime();
#endif
	float ratio = m_size.x() / m_size.y();
	//EWOL_INFO("ratio : " << ratio);
	// TODO : mat4 tmpProjection = etk::matPerspective(m_angleView, ratio, GAME_Z_NEAR, GAME_Z_FAR);
#ifdef SCENE_BRUT_PERFO_TEST
float tmp___localTime1 = (float)(ewol::getTime() - tmp___startTime1) / 1000.0f;
EWOL_DEBUG("      SCENE111  : " << tmp___localTime1 << "ms ");
int64_t tmp___startTime2 = ewol::getTime();
#endif
	// TODO : ewol::openGL::setCameraMatrix(m_camera->getMatrix());
	//mat4 tmpMat = tmpProjection * m_camera->getMatrix();
	// set internal matrix system :
	//ewol::openGL::setMatrix(tmpMat);
	// TODO : ewol::openGL::setMatrix(tmpProjection);
#ifdef SCENE_BRUT_PERFO_TEST
float tmp___localTime2 = (float)(ewol::getTime() - tmp___startTime2) / 1000.0f;
EWOL_DEBUG("      SCENE222  : " << tmp___localTime2 << "ms ");
#endif
	
#ifdef SCENE_BRUT_PERFO_TEST
int64_t tmp___startTime3 = ewol::getTime();
#endif
	onDraw();
#ifdef SCENE_BRUT_PERFO_TEST
float tmp___localTime3 = (float)(ewol::getTime() - tmp___startTime3) / 1000.0f;
EWOL_DEBUG("      SCENE333  : " << tmp___localTime3 << "ms ");
int64_t tmp___startTime4 = ewol::getTime();
#endif
	ewol::openGL::pop();
#ifdef SCENE_BRUT_PERFO_TEST
float tmp___localTime4 = (float)(ewol::getTime() - tmp___startTime4) / 1000.0f;
EWOL_DEBUG("      SCENE444  : " << tmp___localTime4 << "ms ");
#endif
}
/*
vec2 ege::widget::Scene::calculateDeltaAngle(const vec2& _posScreen) {
	double ratio = m_size.x() / m_size.y();
	vec2 pos = vec2(m_size.x()/-2.0, m_size.y()/-2.0) + _posScreen;
	
	double xmax = tan(m_angleView/2.0);
	double ymax = xmax / ratio;
	
	double newX = pos.x() * xmax / m_size.x()*2.0;
	double newY = pos.y() * ymax / m_size.y()*2.0;
	
	double angleX = atan(newX);
	double angleY = atan(newY);
	
	return vec2(angleX,
	            angleY);
}
*/
/*
vec3 ege::widget::Scene::convertScreenPositionInMapPosition(const vec2& _posScreen) {
	return m_camera->projectOnZGround(calculateDeltaAngle(_posScreen));
}
*/
void ege::widget::Scene::onParameterChangeValue(const ewol::parameter::Ref& _paramPointer) {
	ewol::Widget::onParameterChangeValue(_paramPointer);
	/*
	if (_paramPointer == m_isRunning) {
		// nothing to do ...
	}
	*/
}



