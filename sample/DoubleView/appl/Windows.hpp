/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license APACHE-2 (see license file)
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
			float m_angleTetha;
			float m_anglePsy;
			vec2 m_oldScreenPos;
	};
}

