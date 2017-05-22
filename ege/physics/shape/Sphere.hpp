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
			class Sphere : public ege::physics::Shape {
				public:
					Sphere() {};
					virtual ~Sphere() {};
				public:
					virtual bool parse(const char* _line);
					virtual void display() {};
				public:
					virtual enum ege::physics::Shape::type getType() const {
						return ege::physics::Shape::type::sphere;
					};
				private:
					float m_radius; // props["radius"] = obj.scale.x
				public:
					float getRadius() const {
						return m_radius;
					};
					void setRadius(float _radius) {
						m_radius = _radius;
					};
				private:
					virtual const ege::physics::shape::Sphere* toSphere() const {
						return this;
					};
					virtual ege::physics::shape::Sphere* toSphere() {
						return this;
					};
			};
		}
	}
}

