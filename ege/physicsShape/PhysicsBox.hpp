/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once


#include <etk/types.hpp>
#include <ege/physicsShape/PhysicsShape.hpp>

namespace ege {
	class PhysicsBox : public ege::PhysicsShape {
		public:
			PhysicsBox() {};
			virtual ~PhysicsBox() {};
		public:
			virtual bool parse(const char* _line);
			virtual void display() {};
		public:
			virtual enum ege::PhysicsShape::type getType() const {
				return ege::PhysicsShape::box;
			};
		private:
			vec3 m_size; // Box size property in X, Y and Z
		public:
			const vec3& getSize() const {
				return m_size;
			};
			void setSize(const vec3& _size) {
				m_size = _size;
			}
		public:
			virtual const ege::PhysicsBox* toBox() const {
				return this;
			};
			virtual ege::PhysicsBox* toBox() {
				return this;
			};
	};
}

