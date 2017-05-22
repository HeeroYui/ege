/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once


#include <etk/types.hpp>
#include <ege/physics/shape/Shape.hpp>

namespace ege {
	namespace physics {
		namespace shape {
			class Box : public ege::physics::Shape {
				public:
					Box() {};
					virtual ~Box() {};
				public:
					virtual bool parse(const char* _line);
					virtual void display() {};
				public:
					virtual enum ege::physics::Shape::type getType() const {
						return ege::physics::Shape::type::box;
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
					virtual const ege::physics::shape::Box* toBox() const {
						return this;
					};
					virtual ege::physics::shape::Box* toBox() {
						return this;
					};
			};
		}
	}
}

