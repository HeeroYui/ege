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
			class ConvexHull : public ege::physics::Shape {
				public:
					ConvexHull() {};
					virtual ~ConvexHull() {};
				public:
					virtual bool parse(const char* _line);
					virtual void display() {};
				public:
					virtual enum ege::physics::Shape::type getType() const {
						return ege::physics::Shape::type::convexHull;
					};
				private:
					vec3 m_scale;
				public:
					vec3 getScale() const {
						return m_scale;
					};
				private:
					std::vector<vec3> m_points;
				public:
					const std::vector<vec3>& getPointList() const {
						return m_points;
					};
				public:
					virtual const ege::physics::shape::ConvexHull* toConvexHull() const {
						return this;
					};
					virtual ege::physics::shape::ConvexHull* toConvexHull() {
						return this;
					};
			};
		}
	}
}

