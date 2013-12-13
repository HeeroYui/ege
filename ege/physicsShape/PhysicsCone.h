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
			PhysicsCone(void) {};
			virtual ~PhysicsCone(void) {};
		public:
			virtual bool parse(const char* _line);
			virtual void display(void) {};
		public:
			virtual enum ege::PhysicsShape::type getType(void) {
				return ege::PhysicsShape::cone;
			};
		private:
			float m_radius;
		public:
			float getRadius(void) const {
				return m_radius;
			};
		private:
			float m_height;
		public:
			float getHeight(void) const {
				return m_height;
			};
		public:
			virtual const ege::PhysicsCone* toCone(void) const {
				return this;
			};
			virtual ege::PhysicsCone* toCone(void) {
				return this;
			};
	};
};


#endif
