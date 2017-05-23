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
#include <vector>
#include <ewol/debug.hpp>
#include <ege/camera/Camera.hpp>
#include <ewol/widget/Widget.hpp>
#include <gale/renderer/openGL/openGL.hpp>
#include <gale/resource/Manager.hpp>
#include <gale/Dimension.hpp>
#include <ephysics/reactphysics3d.h>
#include <ege/physics/Component.hpp>
#include <eproperty/Value.hpp>
#include <ege/Ray.hpp>



namespace ege {
	namespace physics {
		class Engine:
		  public ege::Engine,
		  public rp3d::EventListener {
			public:
				eproperty::Value<bool> propertyDebugAABB;
				eproperty::Value<bool> propertyDebugShape;
			private:
				rp3d::DynamicsWorld* m_dynamicsWorld;
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
				rp3d::DynamicsWorld* getDynamicWorld() {
					return m_dynamicsWorld;
				}
			protected:
				std::vector<ememory::SharedPtr<ege::physics::Component>> m_component;
				//TODO :  set it not in ewol ...
				ememory::SharedPtr<ewol::resource::Colored3DObject> m_debugDrawProperty;
			public:
				const std::string& getType() const override;
				void componentRemove(const ememory::SharedPtr<ege::Component>& _ref) override;
				void componentAdd(const ememory::SharedPtr<ege::Component>& _ref) override;
				void update(const echrono::Duration& _delta) override;
				void renderDebug(const echrono::Duration& _delta, const ememory::SharedPtr<ege::Camera>& _camera) override;
			private:
				// herited from rp3D::EventListener
				void beginContact(const rp3d::ContactPointInfo& _contact) override;
				void newContact(const rp3d::ContactPointInfo& _contact) override;
			public:
				/**
				 * @brief Test a rayCasting on the physic engine
				 * @param[in] _ray Ray top test
				 * @return Impact position and normal of the impact (if normal == vec3(0,0,0) then no object impact...
				 */
				std::pair<vec3,vec3> testRay(const ege::Ray& _ray);
				/**
				 * @brief Test a rayCasting on the physic engine
				 * @param[in] _ray Ray top test
				 * @return Impact Component, position and normal of the impact (if normal == vec3(0,0,0) then no object impact...
				 */
				std::pair<ememory::SharedPtr<ege::Component>, std::pair<vec3,vec3>> testRayObject(const ege::Ray& _ray);
		};
	}
}

