/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ege/physicsShape/PhysicsShape.hpp>

namespace ege {
	class PhysicsConvexHull : public ege::PhysicsShape {
		public:
			PhysicsConvexHull() {};
			virtual ~PhysicsConvexHull() {};
		public:
			virtual bool parse(const char* _line);
			virtual void display() {};
		public:
			virtual enum ege::PhysicsShape::type getType() const {
				return ege::PhysicsShape::convexHull;
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
			virtual const ege::PhysicsConvexHull* toConvexHull() const {
				return this;
			};
			virtual ege::PhysicsConvexHull* toConvexHull() {
				return this;
			};
	};
}

