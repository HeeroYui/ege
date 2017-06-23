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
		  public ege::Component,
		  public rp3d::CollisionCallback {
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
				 * @brief Get the linear velocity.
				 * @return The linear velocity vector of the body
				 */
				vec3 getLinearVelocity() const;
				/**
				 * @brief Set the linear velocity.
				 * @param[in] _linearVelocity The linear velocity vector of the body
				 */
				void setLinearVelocity(const vec3& _linearVelocity);
				/**
				 * @brief Get the angular velocity.
				 * @return The angular velocity vector of the body
				 */
				vec3 getAngularVelocity() const;
				/**
				 * @brief Set the angular velocity.
				 * @param[in] _linearVelocity The angular velocity vector of the body
				 */
				void setAngularVelocity(const vec3& _angularVelocity);
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
				void emitAll();
				friend class ege::physics::Engine;
			private:
				// herited from rp3d::CollisionCallback to notify the contact on the current rigid body
				void notifyContact(const rp3d::ContactPointInfo& _contactPointInfo) override;
		};
	}
}