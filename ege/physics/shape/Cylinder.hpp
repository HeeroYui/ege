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
			class Cylinder : public ege::physics::Shape {
				public:
					Cylinder() {};
					virtual ~Cylinder() {};
				public:
					virtual bool parse(const char* _line);
					virtual void display() {};
				public:
					virtual enum ege::physics::Shape::type getType() const {
						return ege::physics::Shape::type::cylinder;
					};
				private:
					vec3 m_size;
				public:
					vec3 getSize() const {
						return m_size;
					};
				public:
					virtual const ege::physics::shape::Cylinder* toCylinder() const {
						return this;
					};
					virtual ege::physics::shape::Cylinder* toCylinder() {
						return this;
					};
			};
		}
	}
}

