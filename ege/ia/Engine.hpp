/** @file
 * @author Edouard DUPIN
 * @copyright 2017, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once
#include <ege/Engine.hpp>
#include <etk/types.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Matrix4x4.hpp>
#include <vector>
#include <ege/debug.hpp>


namespace ege {
	namespace ia {
		class Engine : public ege::Engine {
			public:
				Engine(ege::Environement* _env);
				~Engine() {}
				
				// update cycle
				void update(float _delta) {}
			public:
				const std::string& getType() const override;
				void componentRemove(const ememory::SharedPtr<ege::Component>& _ref) override;
				void componentAdd(const ememory::SharedPtr<ege::Component>& _ref) override;
		};
	}
}

