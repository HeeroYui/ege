/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_PHYSICS_CONVEX_HULL_H__
#define __EGE_PHYSICS_CONVEX_HULL_H__


#include <etk/types.h>
#include <ege/physicsShape/PhysicsShape.h>


namespace ege {
	class PhysicsConvexHull : public ege::PhysicsShape {
		public:
			PhysicsConvexHull(void) {};
			virtual ~PhysicsConvexHull(void) {};
		public:
			virtual bool parse(const char* _line);
			virtual void display(void) {};
		public:
			virtual enum ege::PhysicsShape::type getType(void) {
				return ege::PhysicsShape::convexHull;
			};
		private:
			vec3 m_scale;
		public:
			vec3 getScale(void) const {
				return m_scale;
			};
		private:
			std::vector<vec3> m_points;
		public:
			const std::vector<vec3>& getPointList(void) const {
				return m_points;
			};
		public:
			virtual const ege::PhysicsConvexHull* toConvexHull(void) const {
				return this;
			};
			virtual ege::PhysicsConvexHull* toConvexHull(void) {
				return this;
			};
	};
};


#endif
