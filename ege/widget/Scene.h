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
#include <ege/camera/Camera.h>
#include <ewol/widget/Widget.h>
#include <gale/renderer/openGL/openGL.h>
#include <gale/resource/Manager.h>
#include <ege/elements/Element.h>
#include <gale/Dimension.h>

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
#include <ewol/signal/Signal.h>

namespace ege {
	namespace widget {
		class Scene : public ewol::Widget {
			protected:
				std::shared_ptr<ege::Environement> m_env;
				std::shared_ptr<ewol::resource::Colored3DObject> m_debugDrawProperty;
			public:
				ewol::Signal<std::shared_ptr<ewol::resource::Colored3DObject>/*, std::shared_ptr<ege::Camera>*/> signalDisplayDebug; //!< emit a signal to the application to draw the debug (@ref setDebugPhysic)
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
				std::string m_cameraName; //!< current camera name
			public:
				/** 
				 * @brief Select a Camera for the display.
				 * @param[in] _cameraName New name of the camera.
				 */
				void setCamera(const std::string& _cameraName);
				/** 
				 * @brief Get the current camera name to display the environnement.
				 * @return the current camera name.
				 */
				const std::string& getCamera() const {
					return m_cameraName;
				}
			protected:
				// Note : This is only for temporary elements : on the display
				std::vector<ege::Environement::ResultNearestElement> m_displayElementOrdered;
			protected: // Derived function
				virtual void onDraw();
				virtual void onParameterChangeValue(const ewol::parameter::Ref& _paramPointer);
			public: // Derived function
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual void onRegenerateDisplay();
				virtual void periodicCall(const ewol::event::Time& _event);
				virtual void calculateSize(const vec2& _available);
			protected:
				ewol::parameter::Value<bool> m_debugPhysic; //!< display Physic Debug
			public:
				/**
				 * @brief Set the debug display of the physic engine
				 * @param[in] _status new status of the debug
				 */
				void setDebugPhysic(bool _status) {
					m_debugPhysic.set(_status);
				}
				/**
				 * @brief Get the debug display status of the physic engine
				 * @return The current status of the debug.
				 */
				bool getDebugPhysic() {
					return m_debugPhysic.get();
				}
			protected:
				ewol::parameter::Value<bool> m_debugApplication; //!< display Application Debug
			public:
				/**
				 * @brief Set the debug display of the application
				 * @param[in] _status new status of the debug
				 */
				void setDebugApplication(bool _status) {
					m_debugApplication.set(_status);
				}
				/**
				 * @brief Get the debug display status of the application
				 * @return The current status of the debug.
				 */
				bool getDebugApplication() {
					return m_debugApplication.get();
				}
		};
	};
};

#endif


