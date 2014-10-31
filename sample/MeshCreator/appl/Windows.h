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

namespace appl {
	class Windows : public ewol::widget::Windows {
		private:
			std::shared_ptr<ege::Environement> m_env;
		protected:
			Windows();
			void init();
		public:
			DECLARE_FACTORY(Windows);
			virtual ~Windows() { };
	};
};


#endif