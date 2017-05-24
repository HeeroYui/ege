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
#include <ege/camera/ControlBase.hpp>
#include <ewol/resource/Colored3DObject.hpp>

namespace appl {
	class Windows : public ewol::widget::Windows {
		private:
			ememory::SharedPtr<ege::Environement> m_env;
			ememory::SharedPtr<ege::camera::View> m_camera;
			ege::camera::ControlBase m_cameraControler;
		protected:
			Windows();
			void init() override;
		public:
			DECLARE_FACTORY(Windows);
			virtual ~Windows() { };
		private:
			// need this to forward if to the camera controler ...
			bool onEventEntry(const ewol::event::Entry& _event) override;
			bool onEventInput(const ewol::event::Input& _event) override;
	};
}

