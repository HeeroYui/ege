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
			class Capsule : public ege::physics::Shape {
				public:
					Capsule() {};
					virtual ~Capsule() {};
				public:
					virtual bool parse(const char* _line);
					virtual void display() {};
				public:
					virtual enum ege::physics::Shape::type getType() const {
						return ege::physics::Shape::type::capsule;
					};
				private:
					float m_radius;
				public:
					float getRadius() const {
						return m_radius;
					};
				private:
					float m_height;
				public:
					float getHeight() const {
						return m_height;
					};
				public:
					virtual const ege::physics::shape::Capsule* toCapsule() const {
						return this;
					};
					virtual ege::physics::shape::Capsule* toCapsule() {
						return this;
					};
			};
		}
	}
}

