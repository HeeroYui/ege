/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/ElementGameIA.h>

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




ege::ElementGameIA::ElementGameIA(ege::Environement& _env) :
	ElementGame(_env),
	m_IA(*this),
	m_isDynamicStarted(false)
{
	
}

ege::ElementGameIA::~ElementGameIA(void)
{
	DynamicDisable();
}


void ege::ElementGameIA::DynamicEnable(void)
{
	ege::ElementGame::DynamicEnable();
	if (false == m_isDynamicStarted) {
		m_env.GetDynamicWorld()->addAction(&m_IA);
		m_isDynamicStarted = true;
	}
}

void ege::ElementGameIA::DynamicDisable(void)
{
	ege::ElementGame::DynamicDisable();
	if (true == m_isDynamicStarted) {
		m_env.GetDynamicWorld()->removeAction(&m_IA);
		m_isDynamicStarted = false;
	}
}




