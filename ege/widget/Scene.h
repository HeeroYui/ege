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
	namespace widget {
		class Scene : public ewol::Widget {
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
				Scene(bool _setAutoBullet=true, bool _setAutoCamera=true);
				/**
				 * @brief Destructor of the widget classes
				 */
				virtual ~Scene(void);
				void setBulletConfig(btDefaultCollisionConfiguration* _collisionConfiguration=NULL,
				                     btCollisionDispatcher* _dispatcher=NULL,
				                     btBroadphaseInterface* _broadphase=NULL,
				                     btConstraintSolver* _solver=NULL,
				                     btDynamicsWorld* _dynamicsWorld=NULL);
				void setCamera(ege::Camera* _camera=NULL);
			private:
				float m_gameTime; //!< time of the game running
			protected:
				float m_angleView;
				///this is the most important class
				btDefaultCollisionConfiguration* m_collisionConfiguration;
				btCollisionDispatcher* m_dispatcher;
				btBroadphaseInterface* m_broadphase;
				btConstraintSolver* m_solver;
				btDynamicsWorld* m_dynamicsWorld;
			// camera section
				ege::Camera* m_camera; //!< display point of view.
			// Other elements
				bool m_isRunning; //!< the display is running (not in pause)
				float m_ratioTime; //!< Ratio time for the speed of the game ...
				// Note : This is only for temporary elements : on the display
				std::vector<ege::Environement::ResultNearestElement> m_displayElementOrdered;
			public:
				/**
				 * @brief set the scene in pause for a while
				 */
				void pause(void);
				/**
				 * @brief resume the scene activity
				 */
				void resume(void);
				/**
				 * @brief Toggle between pause and running
				 */
				void pauseToggle(void);
			protected:
				bool m_debugMode;
				ewol::resource::Colored3DObject* m_debugDrawing;  //!< for the debug draw elements
			public:
				/**
				 * @brief Toggle the debug mode  == > usefull for DEBUG only ...
				 */
				void debugToggle(void) {
					m_debugMode = m_debugMode?false:true;
				};
			protected:
				// Derived function
				virtual void ScenePeriodicCall(int64_t _localTime, int32_t _deltaTime) { };
			public:
				vec2 calculateDeltaAngle(const vec2& _posScreen);
				vec3 convertScreenPositionInMapPosition(const vec2& _posScreen);
				/**
				 * @brief get the current camera reference for the scene rendering
				 */
				ege::Camera& getCamera(void) {
					return *m_camera;
				};
				/**
				 * @brief set the curent Time Ratio (default 1)
				 */
				void setRatioTime(float _newRatio) {
					m_ratioTime = _newRatio;
				};
				
				void renderscene(int pass);
				void drawOpenGL(btScalar* m, 
				                const btCollisionShape* _shape,
				                const btVector3& _color,
				                int32_t	_debugMode,
				                const btVector3& _worldBoundsMin,
				                const btVector3& _worldBoundsMax);
				void drawSphere(btScalar _radius,
				                int _lats,
				                int _longs,
				                mat4& _transformationMatrix,
				                etk::Color<float>& _tmpColor);
				void getElementAroundNewElement(vec3 _sourcePosition,
				                                std::vector<ege::Environement::ResultNearestElement>& _resultList);
				
			protected: // Derived function
				virtual void onDraw(void);
			public: // Derived function
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual void onRegenerateDisplay(void);
				virtual void periodicCall(const ewol::event::Time& _event);
		};
	};
};

#endif


