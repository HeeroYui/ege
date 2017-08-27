/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ege/Engine.hpp>

namespace ege {
	namespace physics {
		class Engine;
	};
};
#include <etk/types.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Matrix4x4.hpp>
#include <etk/Vector.hpp>
#include <ewol/debug.hpp>
#include <ege/camera/Camera.hpp>
#include <ewol/widget/Widget.hpp>
#include <gale/renderer/openGL/openGL.hpp>
#include <gale/resource/Manager.hpp>
#include <gale/Dimension.hpp>
#include <ephysics/ephysics.hpp>
#include <ege/physics/Component.hpp>
#include <eproperty/Value.hpp>
#include <ege/Ray.hpp>



namespace ege {
	namespace physics {
		class Engine:
		  public ege::Engine,
		  public ephysics::EventListener {
			public:
				eproperty::Value<bool> propertyDebugAABB;
				eproperty::Value<bool> propertyDebugShape;
			private:
				ephysics::DynamicsWorld* m_dynamicsWorld;
				float m_accumulator; // limit call of the step rendering
			public:
				Engine(ege::Environement* _env);
				~Engine();
			public:
				/**
				 * @brief Set the gravity axis of the physic engine
				 * @param[in] _axePower energy of this gravity
				 */
				void setGravity(const vec3& _axePower);
				ephysics::DynamicsWorld* getDynamicWorld() {
					return m_dynamicsWorld;
				}
			protected:
				etk::Vector<ememory::SharedPtr<ege::physics::Component>> m_component;
				//TODO :  set it not in ewol ...
				ememory::SharedPtr<ewol::resource::Colored3DObject> m_debugDrawProperty;
			public:
				const etk::String& getType() const override;
				void componentRemove(const ememory::SharedPtr<ege::Component>& _ref) override;
				void componentAdd(const ememory::SharedPtr<ege::Component>& _ref) override;
				void update(const echrono::Duration& _delta) override;
				void renderDebug(const echrono::Duration& _delta, const ememory::SharedPtr<ege::Camera>& _camera) override;
			private:
				// herited from rp3D::EventListener
				void beginContact(const ephysics::ContactPointInfo& _contact) override;
				void newContact(const ephysics::ContactPointInfo& _contact) override;
			public:
				/**
				 * @brief Test a rayCasting on the physic engine
				 * @param[in] _ray Ray top test
				 * @return Impact position and normal of the impact (if normal == vec3(0,0,0) then no object impact...
				 */
				etk::Pair<vec3,vec3> testRay(const ege::Ray& _ray);
				/**
				 * @brief Test a rayCasting on the physic engine
				 * @param[in] _ray Ray top test
				 * @return Impact Component, position and normal of the impact (if normal == vec3(0,0,0) then no object impact...
				 */
				etk::Pair<ememory::SharedPtr<ege::Component>, etk::Pair<vec3,vec3>> testRayObject(const ege::Ray& _ray);
		};
	}
}

