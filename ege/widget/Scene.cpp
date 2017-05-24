/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/widget/Scene.hpp>
#include <echrono/Steady.hpp>
#include <echrono/Duration.hpp>

#include <cmath>
#include <ege/debug.hpp>
#include <ewol/ewol.hpp>
#include <ewol/object/Manager.hpp>
#include <gale/renderer/openGL/openGL.hpp>
#include <etk/math/Matrix4x4.hpp>

namespace etk {
	template<> std::string to_string<ememory::SharedPtr<ewol::resource::Colored3DObject> >(const ememory::SharedPtr<ewol::resource::Colored3DObject>& _value) {
		return "{{ERROR}}";
	}
};

ege::widget::Scene::Scene() :
  signalDisplayDebug(this, "drawDebug", "Call to draw debug after all entitys"),
  propertyDebugPhysic(this, "debugPhysic",
                            false,
                            "Display debug of the physic interface"),
  propertyDebugNormal(this, "debugMeshNormal",
                            false,
                            "Display debug of the mesh normal of each face"),
  propertyDebugApplication(this, "debugApplication",
                                 false,
                                 "Display debug of the application"),
  m_cameraName("default") {
	addObjectType("ege::widget::Scene");
}

void ege::widget::Scene::init() {
	ewol::Widget::init();
	setKeyboardRepeate(false);
	propertyCanFocus.set(true);
	m_PCH = getObjectManager().periodicCall.connect(this, &ege::widget::Scene::periodicCall);
	m_debugDrawProperty = ewol::resource::Colored3DObject::create();
}

ege::widget::Scene::~Scene() {
	
}

void ege::widget::Scene::onRegenerateDisplay() {
	if (needRedraw() == true) {
		// Update the curent camera with the aspect ratio
		if (m_env != nullptr) {
			ememory::SharedPtr<ege::Camera> camera = m_env->getCamera(m_cameraName);
			if (camera != nullptr) {
				camera->setSceenSize(getSize());
			}
		}
	}
}

//#define SCENE_DISPLAY_SPEED

//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef SCENE_DISPLAY_SPEED
	static echrono::Steady g_startTime;
	static int32_t g_counterNbTimeDisplay = 0;
#endif

#define NUMBER_OF_SUB_PASS  (0)


void ege::widget::Scene::onDraw() {
	#ifdef SCENE_DISPLAY_SPEED
		g_counterNbTimeDisplay++;
		g_startTime = echrono::Steady::now();
	#endif
	gale::openGL::clearColor(etk::color::black);
	m_env->render(echrono::Duration(1.0/60.0), m_cameraName);
	signalDisplayDebug.emit(m_debugDrawProperty);
	#ifdef SCENE_DISPLAY_SPEED
		echrono::Duration localTime = echrono::Steady::now() - g_startTime;
		if (localTime>1) {
			EWOL_ERROR("      scene : " << localTime << " " << g_counterNbTimeDisplay);
		} else {
			EWOL_DEBUG("      scene : " << localTime << " " << g_counterNbTimeDisplay);
		}
	#endif
}

void ege::widget::Scene::periodicCall(const ewol::event::Time& _event) {
	markToRedraw();
}

//#define SCENE_BRUT_PERFO_TEST

void ege::widget::Scene::systemDraw(const ewol::DrawProperty& _displayProp) {
	#ifdef SCENE_BRUT_PERFO_TEST
		int64_t tmp___startTime0 = ewol::getTime();
	#endif
	//EGE_INFO("DRAW ...");
	gale::openGL::bindBuffer(0);
	gale::openGL::push();
	// here we invert the reference of the standard openGl view because the reference in the common display is Top left and not buttom left
	gale::openGL::setViewPort(m_origin, m_size);
	// configure render with the camera...
	ememory::SharedPtr<ege::Camera> camera = m_env->getCamera(m_cameraName);
	if (camera != nullptr) {
		camera->configureOpenGL();
	}
	onDraw();
	gale::openGL::pop();
	#ifdef SCENE_BRUT_PERFO_TEST
		float tmp___localTime1 = (float)(ewol::getTime() - tmp___startTime0) / 1000.0f;
		EWOL_DEBUG("      SCENE render  : " << tmp___localTime1 << "ms ");
	#endif
}


void ege::widget::Scene::setCamera(const std::string& _cameraName) {
	if (m_cameraName == _cameraName) {
		return;
	}
	m_cameraName = _cameraName;
	// Update camera aspect ratio:
	ememory::SharedPtr<ege::Camera> camera = m_env->getCamera(m_cameraName);
	if (camera != nullptr) {
		camera->setSceenSize(m_size);
	}
}

void ege::widget::Scene::calculateSize() {
	ewol::Widget::calculateSize();
	// Update camera aspect ratio:
	ememory::SharedPtr<ege::Camera> camera = m_env->getCamera(m_cameraName);
	if (camera != nullptr) {
		camera->setSceenSize(m_size);
	}
}

//#include <esignal/details/Signal.hxx>
//ESIGNAL_DECLARE_SIGNAL(ememory::SharedPtr<ewol::resource::Colored3DObject>);

