/** @file
 * @author Edouard DUPIN
 * @copyright 2017, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Matrix4x4.hpp>
#include <vector>
#include <ege/debug.hpp>
#include <ege/render/Component.hpp>


namespace ege {
	namespace render {
		class Engine {
			public:
				Engine() {}
				~Engine() {}
				
				// update cycle
				void update(float _delta) {}
		};
	}
}

