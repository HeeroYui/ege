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
			PhysicsCylinder() {};
			virtual ~PhysicsCylinder() {};
		public:
			virtual bool parse(const char* _line);
			virtual void display() {};
		public:
			virtual enum ege::PhysicsShape::type getType() {
				return ege::PhysicsShape::cylinder;
			};
		private:
			vec3 m_size;
		public:
			vec3 getSize() const {
				return m_size;
			};
		public:
			virtual const ege::PhysicsCylinder* toCylinder() const {
				return this;
			};
			virtual ege::PhysicsCylinder* toCylinder() {
				return this;
			};
	};
};

#endif
