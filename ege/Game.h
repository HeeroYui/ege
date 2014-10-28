/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_SCENE_H__
#define __EGE_SCENE_H__

#include <etk/types.h>
#include <etk/math/Vector3D.h>
#include <etk/math/Matrix4.h>
#include <vector>
#include <ewol/debug.h>
#include <ege/Camera.h>
#include <ewol/widget/Widget.h>
#include <ewol/openGL/openGL.h>
#include <ewol/resource/Manager.h>
#include <ege/ElementGame.h>
#include <ewol/Dimension.h>

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
#include <ewol/widget/Widget.h>

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
			ewol::object::ParamList<enum gameStatus> m_status; //!< the display is running (not in pause)
		public:
			/**
			 * @brief Get the game status.
			 * @return the current status.
			 */
			enum gameStatus getGameStatus() {
				return m_status.get()
			}
			/**
			 * @brief Set the game status.
			 * @param[in] _value New game status.
			 */
			enum gameStatus setGameStatus(enum gameStatus _value) {
				return m_status.set(_value)
			}
		protected:
			ewol::object::ParamList<float> m_ratio; //!< Speed ratio
		public:
			/**
			 * @brief Get the game speed ratio.
			 * @return the current ratio.
			 */
			float getSpeedRatio() {
				return m_ratio.get()
			}
			/**
			 * @brief Set the game ratio.
			 * @param[in] _value New game ratio.
			 */
			enum gameStatus setSpeedRatio(float _value) {
				return m_ratio.set(_value)
			}
		protected:
			ege::PhysicEngine m_physicEngine; //!< physic engine interface
			ege::AudioEngine m_AudioEngine; //!< physic engine interface
			ege::IAEngine m_iAEngine; //!< physic engine interface
			std::map<std::string, std::shared_ptr<ege::Camera>> m_listCamera; //!< list of all camera in the world
		
	}
}


#endif
