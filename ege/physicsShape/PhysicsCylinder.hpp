/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once


#include <etk/types.hpp>
#include <ege/physicsShape/PhysicsShape.hpp>


namespace ege {
	class PhysicsCylinder : public ege::PhysicsShape {
		public:
			PhysicsCylinder() {};
			virtual ~PhysicsCylinder() {};
		public:
			virtual bool parse(const char* _line);
			virtual void display() {};
		public:
			virtual enum ege::PhysicsShape::type getType() const {
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
}

