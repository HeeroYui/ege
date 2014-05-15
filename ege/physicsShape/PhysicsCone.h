/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_PHYSICS_CONE_H__
#define __EGE_PHYSICS_CONE_H__


#include <etk/types.h>
#include <ege/physicsShape/PhysicsShape.h>


namespace ege {
	class PhysicsCone : public ege::PhysicsShape {
		public:
			PhysicsCone() {};
			virtual ~PhysicsCone() {};
		public:
			virtual bool parse(const char* _line);
			virtual void display() {};
		public:
			virtual enum ege::PhysicsShape::type getType() {
				return ege::PhysicsShape::cone;
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
			virtual const ege::PhysicsCone* toCone() const {
				return this;
			};
			virtual ege::PhysicsCone* toCone() {
				return this;
			};
	};
};


#endif
