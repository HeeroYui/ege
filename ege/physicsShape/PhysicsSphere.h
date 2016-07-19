/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once


#include <etk/types.h>
#include <ege/physicsShape/PhysicsShape.h>


namespace ege {
	class PhysicsSphere : public ege::PhysicsShape {
		public:
			PhysicsSphere() {};
			virtual ~PhysicsSphere() {};
		public:
			virtual bool parse(const char* _line);
			virtual void display() {};
		public:
			virtual enum ege::PhysicsShape::type getType() const {
				return ege::PhysicsShape::sphere;
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
			virtual const ege::PhysicsSphere* toSphere() const {
				return this;
			};
			virtual ege::PhysicsSphere* toSphere() {
				return this;
			};
	};
}

