/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license APACHE-2 (see license file)
 */

#ifndef __APPL_WINDOWS_H__
#define __APPL_WINDOWS_H__

#include <ewol/widget/Windows.h>
#include <ege/Environement.h>
#include <ege/camera/View.h>
#include <ewol/resource/Colored3DObject.h>

namespace appl {
	class Windows : public ewol::widget::Windows {
		private:
			std::shared_ptr<ege::Environement> m_env;
			std::shared_ptr<ege::camera::View> m_camera;
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
			void onCallbackPeriodicCheckCollision(const ewol::event::Time& _event);
	};
};


#endif