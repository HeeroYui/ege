/** @file
 * @author Edouard DUPIN
 * @copyright 2017, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Matrix4x4.hpp>
#include <etk/Vector.hpp>
#include <ege/debug.hpp>
#include <ege/render/Component.hpp>
#include <ege/Engine.hpp>
#include <eproperty/Value.hpp>
#include <ewol/widget/Widget.hpp>


namespace ege {
	namespace render {
		class Engine : public ege::Engine {
			public:
				Engine(ege::Environement* _env);
				~Engine() {}
				eproperty::Value<bool> propertyDebugNormal;
			protected:
				etk::Vector<ememory::SharedPtr<ege::render::Component>> m_component;
				class ResultNearestElement {
					public:
						ememory::SharedPtr<ege::render::Component> element;
						float dist;
				};
				etk::Vector<ege::render::Engine::ResultNearestElement> m_displayElementOrdered;
				//TODO :  set it not in ewol ...
				ememory::SharedPtr<ewol::resource::Colored3DObject> m_debugDrawProperty;
			public:
				const etk::String& getType() const override;
				void componentRemove(const ememory::SharedPtr<ege::Component>& _ref) override;
				void componentAdd(const ememory::SharedPtr<ege::Component>& _ref) override;
				void render(const echrono::Duration& _delta, const ememory::SharedPtr<ege::Camera>& _camera) override;
				void renderDebug(const echrono::Duration& _delta, const ememory::SharedPtr<ege::Camera>& _camera) override;
				void getOrderedElementForDisplay(etk::Vector<ege::render::Engine::ResultNearestElement>& _resultList,
				                                 const vec3& _position,
				                                 const vec3& _direction);
		};
	}
}

