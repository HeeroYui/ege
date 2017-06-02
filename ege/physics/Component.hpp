/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ege/debug.hpp>

#include <ege/Component.hpp>
#include <etk/math/Transform3D.hpp>
#include <esignal/Signal.hpp>
#include <ephysics/reactphysics3d.h>
#include <ege/resource/Mesh.hpp>
#include <ege/camera/Camera.hpp>

namespace ege {
	class Environement;
	namespace physics {
		class Engine;
		class Component :
		  public ege::Component/*,
		  public rp3d::CollisionCallback*/ {
			public:
				esignal::Signal<etk::Transform3D> signalPosition;
			protected:
				etk::Transform3D m_lastTransformEmit;
			protected:
				ememory::SharedPtr<ege::physics::Engine> m_engine;
				rp3d::RigidBody* m_rigidBody;
				std::vector<rp3d::CollisionShape*> m_listShape;
				std::vector<rp3d::ProxyShape*> m_listProxyShape;
			public:
				/**
				 * @brief Create a basic position component (no orientation and position (0,0,0))
				 */
				Component(ememory::SharedPtr<ege::Environement> _env);
				/**
				 * @brief Create a basic position component
				 * @param[in] _transform transformation of the position
				 */
				Component(ememory::SharedPtr<ege::Environement> _env, const etk::Transform3D& _transform);
				~Component();
			public:
				virtual const std::string& getType() const override;
				
				enum class type {
					bodyDynamic,
					bodyStatic,
					bodyKinematic
				};
				void setType(enum ege::physics::Component::type _type);
				/**
				 * @brief set a new transformation
				 * @param[in] _transform transformation of the position
				 */
				void setTransform(const etk::Transform3D& _transform);
				/**
				 * @brief set a new transformation
				 * @return Transformation of the position
				 */
				etk::Transform3D getTransform() const;
				/**
				 * @brief Get the linear velocity (whole world).
				 * @return The linear velocity vector of the body
				 */
				vec3 getLinearVelocity() const;
				/**
				 * @brief Set the linear velocity (whole world).
				 * @param[in] _linearVelocity The linear velocity vector of the body
				 */
				void setLinearVelocity(const vec3& _linearVelocity);
				/**
				 * @brief Get the linear velocity (local Body).
				 * @return The linear velocity vector of the body
				 */
				vec3 getRelativeLinearVelocity() const;
				/**
				 * @brief Set the linear velocity (local Body).
				 * @param[in] _linearVelocity The linear velocity vector of the body
				 */
				void setRelativeLinearVelocity(const vec3& _linearVelocity);
				/**
				 * @brief Get the angular velocity (whole world).
				 * @return The angular velocity vector of the body
				 */
				vec3 getAngularVelocity() const;
				/**
				 * @brief Set the angular velocity (whole world).
				 * @param[in] _linearVelocity The angular velocity vector of the body
				 */
				void setAngularVelocity(const vec3& _angularVelocity);
				/**
				 * @brief Get the angular velocity (local Body).
				 * @return The angular velocity vector of the body
				 */
				vec3 getRelativeAngularVelocity() const;
				/**
				 * @brief Set the angular velocity (local Body).
				 * @param[in] _linearVelocity The angular velocity vector of the body
				 */
				void setRelativeAngularVelocity(const vec3& _angularVelocity);
				/**
				 * @brief Apply an external force to the body at a given point (in world-space coordinates).
				 *        If the point is not at the center of mass of the body, it will also generate some torque and therefore, change the angular velocity of the body.
				 *        If the body is sleeping, calling this method will wake it up. Note that the force will we added to the sum of the applied forces and that this sum will be reset to zero at the end of each call of the DynamicsWorld::update() method. You can only apply a force to a dynamic body otherwise, this method will do nothing.
				 * @param[in] _force The force to apply on the body
				 * @param[in] _point The point where the force is applied (in world-space coordinates)
				 */
				void applyForce(const vec3& _force,const vec3& _point);
			protected:
				rp3d::Vector3 m_staticForceApplyCenterOfMass;
			public:
				/**
				 * @brief Apply an external force to the body at its center of mass.
				 *        If the body is sleeping, calling this method will wake it up.
				 * @note The force will we added to the sum of the applied forces and that this sum will be reset to zero at the end of each call of the DynamicsWorld::update() method. You can only apply a force to a dynamic body otherwise, this method will do nothing.
				 * @param[in] _force The external force to apply on the center of mass of the body
				 * @param[in] _static The torque will be apply while the user des not call the same function with 0 value ...
				 */
				void applyForceToCenterOfMass(const vec3& _force, bool _static=false);
				/**
				 * @brief Apply an external force to the body at its center of mass.
				 *        If the body is sleeping, calling this method will wake it up.
				 * @note The force is apply with a relative axis of the object
				 * @note The force will we added to the sum of the applied forces and that this sum will be reset to zero at the end of each call of the DynamicsWorld::update() method. You can only apply a force to a dynamic body otherwise, this method will do nothing.
				 * @param[in] _force The external force to apply on the center of mass of the body
				 * @param[in] _static The torque will be apply while the user des not call the same function with 0 value ...
				 */
				void applyRelativeForceToCenterOfMass(const vec3& _force, bool _static=false);
			protected:
				rp3d::Vector3 m_staticTorqueApply;
			public:
				/**
				 * @brief Apply an external torque to the body.
				 *        If the body is sleeping, calling this method will wake it up.
				 * @note The force will we added to the sum of the applied torques and that this sum will be reset to zero at the end of each call of the DynamicsWorld::update() method. You can only apply a force to a dynamic body otherwise, this method will do nothing.
				 * @param[in] _torque The external torque to apply on the body
				 * @param[in] _static The torque will be apply while the user des not call the same function with 0 value ...
				 */
				void applyTorque(const vec3& _torque, bool _static=false);
				/**
				 * @brief Apply an external torque to the body.
				 *        If the body is sleeping, calling this method will wake it up.
				 * @note The torque is apply with a relative axis of the object
				 * @note The force will we added to the sum of the applied torques and that this sum will be reset to zero at the end of each call of the DynamicsWorld::update() method. You can only apply a force to a dynamic body otherwise, this method will do nothing.
				 * @param[in] _torque The external torque to apply on the body
				 * @param[in] _static The torque will be apply while the user des not call the same function with 0 value ...
				 */
				void applyRelativeTorque(const vec3& _torque, bool _static=false);
				
			protected:
				std::vector<ememory::SharedPtr<ege::physics::Shape>> m_shape; //!< collision shape module ... (independent of bullet lib)
			public:
				const std::vector<ememory::SharedPtr<ege::physics::Shape>>& getShape() const;
				void setShape(const std::vector<ememory::SharedPtr<ege::physics::Shape>>& _prop);
				void addShape(const ememory::SharedPtr<ege::physics::Shape>& _shape);
				void generate();
				void drawShape(ememory::SharedPtr<ewol::resource::Colored3DObject> _draw, ememory::SharedPtr<ege::Camera> _camera);
				void drawAABB(ememory::SharedPtr<ewol::resource::Colored3DObject> _draw, ememory::SharedPtr<ege::Camera> _camera);
			private:
				// call done after all cycle update of the physical engine
				void emitAll();
				// call of this function every time the call will be done
				void update(float _delta);
				friend class ege::physics::Engine;
			private:
				/**
				 * @brief Called when a new contact point is found between two bodies that were separated before.
				 * @param[in] _other The other component that have the impact
				 * @param[in] _normal Normal of the impact
				 * @param[in] _pos Position of the impact at the current object
				 * @param[in] _posOther Position of the impact at the other object
				 * @param[in] _penetrationDepth Depth penetration in the object
				 */
				void beginContact(ege::physics::Component* _other, const vec3& _normal, const vec3& _pos, const vec3& _posOther, float _penetrationDepth);
				/**
				 * @brief Called when a new contact point is found between two bodies.
				 * @param[in] _other The other component that have the impact
				 * @param[in] _normal Normal of the impact
				 * @param[in] _pos Position of the impact at the current object
				 * @param[in] _posOther Position of the impact at the other object
				 * @param[in] _penetrationDepth Depth penetration in the object
				 */
				void newContact(ege::physics::Component* _other, const vec3& _normal, const vec3& _pos, const vec3& _posOther, float _penetrationDepth);
		};
	}
}