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
#include <etk/Vector.h>
#include <ewol/debug.h>
#include <ege/Camera.h>
#include <ewol/widget/Widget.h>
#include <ewol/renderer/openGL.h>
#include <ewol/renderer/ResourceManager.h>
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
	class Scene : public ewol::Widget
	{
		public:
			static const char * const eventPlayTimeChange;
			static const char * const eventKillEnemy;
		protected:
			ege::Environement m_env;
		public:
			/**
			 * @brief Constructor of the widget classes
			 * @return (no execption generated (not managed in embended platform))
			 */
			Scene(void);
			/**
			 * @brief Destructor of the widget classes
			 */
			virtual ~Scene(void);
		private:
			float m_gameTime; //!< time of the game running
		protected:
			float m_angleView;
			vec2 m_centerButtonStartPos;
			vec2 m_leftButtonStartPos;
			vec3 m_finger_StartPosMoving;
			bool m_finger_DoubleTouch;
			vec2 m_finger_1Position;
			vec2 m_finger_2Position;
			float m_finger_oldDistance;
			float m_finger_oldAngle;
			///this is the most important class
			btDefaultCollisionConfiguration* m_collisionConfiguration;
			btCollisionDispatcher* m_dispatcher;
			btBroadphaseInterface* m_broadphase;
			btConstraintSolver* m_solver;
			btDynamicsWorld* m_dynamicsWorld;
		// camera section
			ege::Camera m_camera; //!< Display point of view.
			vec3 m_cameraMovePointStart; //!< Position where the mouse start to move
		// Other elements
			bool m_isRunning; //!< the display is running (not in pause)
			float m_ratioTime; //!< Ratio time for the speed of the game ...
			uint32_t m_walk; //!< Wolk properties
			bool m_debugMode;
			ewol::Colored3DObject* m_debugDrawing;  //!< for the debug draw elements
			// Note : This is only for temporary elements : on the display
			etk::Vector<ege::Environement::ResultNearestElement> m_displayElementOrdered;
			/**
			 * @brief Set the scene in pause for a while
			 */
			void Pause(void);
			/**
			 * @brief Resume the scene activity
			 */
			void Resume(void);
			/**
			 * @brief Toggle between pause and running
			 */
			void PauseToggle(void);
			void DebugToggle(void) { m_debugMode = m_debugMode?false:true; };
		protected:
			// Derived function
			virtual void ScenePeriodicCall(int64_t localTime, int32_t deltaTime) { };
		// camera properties :
		private:
			float m_zoom;
		public:
			vec2 CalculateDeltaAngle(const vec2& posScreen);
			vec3 ConvertScreenPositionInMapPosition(const vec2& posScreen);
			/**
			 * @brief Get the current camera reference for the scene rendering
			 */
			ege::Camera& GetCamera(void) { return m_camera; };
			/**
			 * @brief Set the curent Time Ratio (default 1)
			 */
			void SetRatioTime(float newRatio) { m_ratioTime = newRatio; };
			
			void renderscene(int pass);
			void DrawOpenGL(btScalar* m, 
			                const btCollisionShape* shape,
			                const btVector3& color,
			                int32_t	debugMode,
			                const btVector3& worldBoundsMin,
			                const btVector3& worldBoundsMax);
			void DrawSphere(btScalar radius, int lats, int longs, mat4& transformationMatrix, etk::Color<float>& tmpColor);
			void GetElementAroundNewElement(vec3 sourcePosition, etk::Vector<ege::Environement::ResultNearestElement>& resultList);
			
		protected: // Derived function
			virtual void OnDraw(void);
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "appl::Game"; };
			virtual void SystemDraw(const ewol::DrawProperty& _displayProp);
			virtual void OnRegenerateDisplay(void);
			virtual void PeriodicCall(const ewol::EventTime& _event);
			virtual bool OnEventInput(const ewol::EventInput& _event);
			virtual bool OnEventEntry(const ewol::EventEntry& _event);
			virtual void OnGetFocus(void);
			virtual void OnLostFocus(void);
	};
};

#endif


