/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_PHYSICS_BOX_H__
#define __EGE_PHYSICS_BOX_H__


#include <etk/types.h>
#include <ege/physicsShape/PhysicsShape.h>

namespace ege {
	class PhysicsBox : public ege::PhysicsShape {
		public:
			PhysicsBox() {};
			virtual ~PhysicsBox() {};
		public:
			virtual bool parse(const char* _line);
			virtual void display() {};
		public:
			virtual enum ege::PhysicsShape::type getType() {
				return ege::PhysicsShape::box;
			};
		private:
			vec3 m_size; // Box size property in X, Y and Z
		public:
			const vec3& getSize() const {
				return m_size;
			};
		public:
			virtual const ege::PhysicsBox* toBox() const {
				return this;
			};
			virtual ege::PhysicsBox* toBox() {
				return this;
			};
	};
};

#endif
