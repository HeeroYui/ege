/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/math/Vector4D.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Quaternion.hpp>
#include <ememory/memory.hpp>


namespace ege {
	namespace physics {
		namespace shape {
			class Box;
			class Cylinder;
			class Capsule;
			class Cone;
			class ConvexHull;
			class Sphere;
			class Concave;
		}
		
		class Shape {
			public:
				static ememory::SharedPtr<ege::physics::Shape> create(const std::string& _name);
			public:
				enum class type {
					unknow,
					box,
					capsule,
					cone,
					convexHull,
					cylinder,
					sphere,
					concave
				};
			public:
				Shape() :
				  m_quaternion(0,0,0,1),
				  m_origin(0,0,0),
				  m_mass(1) { // by default set mass at 1g
					
				}
				virtual ~Shape() = default;
			public:
				virtual enum ege::physics::Shape::type getType() const {
					return ege::physics::Shape::type::unknow;
				}
				
			public:
				virtual bool parse(const char* _line);
				virtual void display() {
					
				}
			private:
				etk::Quaternion m_quaternion;
			public:
				etk::Quaternion getOrientation() const {
					return etk::Quaternion(m_quaternion.x(), m_quaternion.y(), m_quaternion.z(), m_quaternion.w());
				}
			private:
				vec3 m_origin;
			public:
				const vec3& getOrigin() const {
					return m_origin;
				};
			private:
				float m_mass; //!< element mass in "g" then 1000 for 1kg
			public:
				float getMass() const {
					return m_mass;
				}
				void setMass(float _mass) {
					m_mass = _mass;
				}
			public:
				bool isBox() {
					return getType() == ege::physics::Shape::type::box;
				};
				bool isCylinder() {
					return getType() == ege::physics::Shape::type::cylinder;
				};
				bool isCapsule() {
					return getType() == ege::physics::Shape::type::capsule;
				};
				bool isCone() {
					return getType() == ege::physics::Shape::type::cone;
				};
				bool isConvexHull() {
					return getType() == ege::physics::Shape::type::convexHull;
				};
				bool isSphere() {
					return getType() == ege::physics::Shape::type::sphere;
				};
				bool isConcave() {
					return getType() == ege::physics::Shape::type::concave;
				};
				
				virtual const ege::physics::shape::Box* toBox() const {
					return nullptr;
				};
				virtual ege::physics::shape::Box* toBox() {
					return nullptr;
				};
				
				virtual const ege::physics::shape::Cylinder* toCylinder() const {
					return nullptr;
				};
				virtual ege::physics::shape::Cylinder* toCylinder() {
					return nullptr;
				};
				
				virtual const ege::physics::shape::Capsule* toCapsule() const {
					return nullptr;
				};
				virtual ege::physics::shape::Capsule* toCapsule() {
					return nullptr;
				};
				
				virtual const ege::physics::shape::Cone* toCone() const {
					return nullptr;
				};
				virtual ege::physics::shape::Cone* toCone() {
					return nullptr;
				};
				
				virtual const ege::physics::shape::ConvexHull* toConvexHull() const {
					return nullptr;
				};
				virtual ege::physics::shape::ConvexHull* toConvexHull() {
					return nullptr;
				};
				
				virtual const ege::physics::shape::Sphere* toSphere() const {
					return nullptr;
				};
				virtual ege::physics::shape::Sphere* toSphere() {
					return nullptr;
				};
				virtual const ege::physics::shape::Concave* toConcave() const {
					return nullptr;
				};
				virtual ege::physics::shape::Concave* toConcave() {
					return nullptr;
				};
		};
	}
}

