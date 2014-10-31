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
			protected:
				std::shared_ptr<ege::Environement> m_env;
			protected:
				/**
				 * @brief Constructor of the widget classes
				 * @return (no execption generated (not managed in embended platform))
				 */
				Scene();
				void init(std::shared_ptr<ege::Environement> _env);
			public:
				DECLARE_FACTORY(Scene);
				/**
				 * @brief Destructor of the widget classes
				 */
				virtual ~Scene();
			protected:
				std::string m_cameraName;
			public:
				void setCamera(const std::string& _cameraName) {
					m_cameraName = _cameraName;
				}
			protected:
				// Note : This is only for temporary elements : on the display
				std::vector<ege::Environement::ResultNearestElement> m_displayElementOrdered;
			protected:
				//bool m_debugMode;
				//std::shared_ptr<ewol::resource::Colored3DObject> m_debugDrawing;  //!< for the debug draw elements
			public:
				/**
				 * @brief Toggle the debug mode  == > usefull for DEBUG only ...
				 */
				/*
				void debugToggle() {
					m_debugMode = m_debugMode?false:true;
				};
				*/
			protected:
				// Derived function
				//virtual void ScenePeriodicCall(int64_t _localTime, int32_t _deltaTime) { };
			public:
				//vec2 calculateDeltaAngle(const vec2& _posScreen);
				//vec3 convertScreenPositionInMapPosition(const vec2& _posScreen);
				/**
				 * @brief get the current camera reference for the scene rendering
				 */
				/*
				ege::Camera& getCamera() {
					return *m_camera;
				};
				*/
				/*
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
				*/
			protected: // Derived function
				virtual void onDraw();
				virtual void onParameterChangeValue(const ewol::parameter::Ref& _paramPointer);
			public: // Derived function
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual void onRegenerateDisplay();
				virtual void periodicCall(const ewol::event::Time& _event);
		};
	};
};

#endif


