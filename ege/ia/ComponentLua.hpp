/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ege/ia/Component.hpp>
#include <luaWrapper/luaWrapper.hpp>

namespace ege {
	namespace ia {
		class ComponentLua : public ege::ia::Component {
			public:
				ComponentLua(const etk::String& _fileName);
				void update(float _delta) override;
			private:
				luaWrapper::Lua m_engine;
		};
	}
}