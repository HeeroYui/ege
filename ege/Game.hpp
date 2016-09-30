/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Matrix4.hpp>
#include <vector>
#include <ewol/debug.hpp>
#include <ege/Camera.hpp>
#include <ewol/widget/Widget.hpp>
#include <ewol/openGL/openGL.hpp>
#include <ewol/resource/Manager.hpp>
#include <ege/ElementGame.hpp>
#include <ewol/Dimension.hpp>

class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;
class btDynamicsWorld;
#include <LinearMath/btScalar.h>
class btVector3;
#include <ewol/widget/Widget.hpp>

namespace ege {
	enum gameStatus {
		gameStart,
		gamePause,
		gameStop
	};
	class Game : public ewol::Object {
		protected:
			Game();
			void init();
		public:
			~Game()
		protected:
			ege::PhysicEngine m_physicEngine; //!< physic engine interface
			ege::AudioEngine m_AudioEngine; //!< physic engine interface
			ege::IAEngine m_iAEngine; //!< physic engine interface
	}
}
