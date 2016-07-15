/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <ege/elements/Element.h>


namespace ege {
	class ElementBase : public ege::Element {
		public:
			/**
			 * @brief Constructor (when constructer is called just add element that did not change.
			 * The objest will be stored in a pool of element and keep a second time if needed  == > redure memory allocation,
			 * when needed, the system will call the init and un-init function...
			 */
			ElementBase(const ememory::SharedPtr<ege::Environement>& _env);
			/**
			 * @brief Destructor
			 */
			virtual ~ElementBase();
			/**
			 * @brief get the element Type description string.
			 * @return A reference on the descriptive string.
			 */
			virtual const std::string& getType() const;
			virtual void draw(int32_t _pass=0);
		private:
			vec3 m_position;
		public:
			virtual const vec3& getPosition();
			virtual void setPosition(const vec3& _pos);
	};
}

