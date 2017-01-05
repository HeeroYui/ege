/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ewol/widget/Windows.hpp>
#include <ege/Environement.hpp>
#include <ege/camera/View.hpp>
#include <ewol/resource/Colored3DObject.hpp>

namespace appl {
	class Windows : public ewol::widget::Windows {
		private:
			ememory::SharedPtr<ege::Environement> m_env;
			ememory::SharedPtr<ege::camera::View> m_camera;
		protected:
			Windows();
			void init();
		public:
			DECLARE_FACTORY(Windows);
			virtual ~Windows() { };
		private:
			bool onEventInput(const ewol::event::Input& _event);
			void onCallbackDisplayDebug(const ememory::SharedPtr<ewol::resource::Colored3DObject>& _obj);
			ege::Ray m_ray;
			float m_angleTetha;
			float m_anglePsy;
			vec2 m_oldScreenPos;
			std::pair<vec3,vec3> m_destination;
	};
}

