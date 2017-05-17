/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ege/debug.hpp>
#include <ememory/memory.hpp>


namespace ege {
	class Component : public ememory::EnableSharedFromThis<Component> {
		protected:
			
		public:
			virtual const std::string& getType() const;
			
			/**
			 * @brief Evironement notify that a new component is added on the same Element
			 * @param[in] _component New component added
			 */
			virtual void addFriendComponent(const ememory::SharedPtr<ege::Component>& _component) {
				// nothing to do.
			}
			/**
			 * @brief Evironement notify that a component is removed on the same Element
			 * @param[in] _component Old component removed
			 */
			virtual void removeFriendComponent(const ememory::SharedPtr<ege::Component>& _component) {
				// nothing to do.
			}
	};
}