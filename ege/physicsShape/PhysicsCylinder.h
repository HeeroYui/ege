/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_PHYSICS_CYLINDER_H__
#define __EGE_PHYSICS_CYLINDER_H__


#include <etk/types.h>
#include <ege/physicsShape/PhysicsShape.h>


namespace ege {
	class PhysicsCylinder : public ege::PhysicsShape {
		public:
			PhysicsCylinder(void) {};
			virtual ~PhysicsCylinder(void) {};
		public:
			virtual bool parse(const char* _line);
			virtual void display(void) {};
		public:
			virtual enum ege::PhysicsShape::type getType(void) {
				return ege::PhysicsShape::cylinder;
			};
		private:
			vec3 m_size;
		public:
			vec3 getSize(void) const {
				return m_size;
			};
		public:
			virtual const ege::PhysicsCylinder* toCylinder(void) const {
				return this;
			};
			virtual ege::PhysicsCylinder* toCylinder(void) {
				return this;
			};
	};
};

#endif
