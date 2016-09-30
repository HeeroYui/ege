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
#include <ege/camera/Camera.hpp>
#include <ewol/widget/Widget.hpp>
#include <gale/renderer/openGL/openGL.hpp>
#include <gale/resource/Manager.hpp>
#include <ege/elements/Element.hpp>
#include <gale/Dimension.hpp>

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
#include <esignal/Signal.hpp>

namespace ege {
	namespace widget {
		class Scene : public ewol::Widget {
			public:
				// signals
				esignal::Signal<ememory::SharedPtr<ewol::resource::Colored3DObject>/*, ememory::SharedPtr<ege::Camera>*/> signalDisplayDebug; //!< emit a signal to the application to draw the debug (@ref setDebugPhysic)
				// properties
				eproperty::Value<bool> propertyDebugPhysic; //!< display Physic Debug
				eproperty::Value<bool> propertyDebugApplication; //!< display Application Debug
			protected:
				ememory::SharedPtr<ege::Environement> m_env;
				ememory::SharedPtr<ewol::resource::Colored3DObject> m_debugDrawProperty;
			public:
			protected:
				/**
				 * @brief Constructor of the widget classes
				 * @return (no execption generated (not managed in embended platform))
				 */
				Scene();
				void init();
			public:
				void setEnv(ememory::SharedPtr<ege::Environement> _env) {
					m_env = _env;
				}
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
			public: // Derived function
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual void onRegenerateDisplay();
				virtual void periodicCall(const ewol::event::Time& _event);
				virtual void calculateSize();
		};
	}
}

