/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_PHYSICS_SHAPE_H__
#define __EGE_PHYSICS_SHAPE_H__


#include <etk/types.h>
#include <etk/types.h>
#include <etk/math/Vector4D.h>
#include <etk/math/Vector3D.h>
#include <memory>


namespace ege {
	class PhysicsBox;
	class PhysicsCylinder;
	class PhysicsCapsule;
	class PhysicsCone;
	class PhysicsConvexHull;
	class PhysicsSphere;
	
	class PhysicsShape {
		public:
			static std::shared_ptr<ege::PhysicsShape> create(const std::string& _name);
		public:
			enum type {
				unknow,
				box,
				capsule,
				cone,
				convexHull,
				cylinder,
				sphere
			};
		public:
			PhysicsShape() :
			  m_quaternion(1,0,0,0),
			  m_origin(0,0,0) {
				
			};
			virtual ~PhysicsShape() {
				
			};
		public:
			virtual enum ege::PhysicsShape::type getType() {
				return ege::PhysicsShape::unknow;
			};
			
		public:
			virtual bool parse(const char* _line);
			virtual void display() {
				
			};
		private:
			vec4 m_quaternion;
		public:
			const vec4& getQuaternion() const {
				return m_quaternion;
			};
		private:
			vec3 m_origin;
		public:
			const vec3& getOrigin() const {
				return m_origin;
			};
		public:
			bool isBox() {
				return getType() == ege::PhysicsShape::box;
			};
			bool isCylinder() {
				return getType() == ege::PhysicsShape::cylinder;
			};
			bool isCapsule() {
				return getType() == ege::PhysicsShape::capsule;
			};
			bool isCone() {
				return getType() == ege::PhysicsShape::cone;
			};
			bool isConvexHull() {
				return getType() == ege::PhysicsShape::convexHull;
			};
			bool isSphere() {
				return getType() == ege::PhysicsShape::sphere;
			};
			
			virtual const ege::PhysicsBox* toBox() const {
				return nullptr;
			};
			virtual ege::PhysicsBox* toBox() {
				return nullptr;
			};
			
			virtual const ege::PhysicsCylinder* toCylinder() const {
				return nullptr;
			};
			virtual ege::PhysicsCylinder* toCylinder() {
				return nullptr;
			};
			
			virtual const ege::PhysicsCapsule* toCapsule() const {
				return nullptr;
			};
			virtual ege::PhysicsCapsule* toCapsule() {
				return nullptr;
			};
			
			virtual const ege::PhysicsCone* toCone() const {
				return nullptr;
			};
			virtual ege::PhysicsCone* toCone() {
				return nullptr;
			};
			
			virtual const ege::PhysicsConvexHull* toConvexHull() const {
				return nullptr;
			};
			virtual ege::PhysicsConvexHull* toConvexHull() {
				return nullptr;
			};
			
			virtual const ege::PhysicsSphere* toSphere() const {
				return nullptr;
			};
			virtual ege::PhysicsSphere* toSphere() {
				return nullptr;
			};
	};
};


#endif

