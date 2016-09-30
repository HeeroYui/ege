/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ege/resource/Mesh.hpp>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

namespace ege {
	namespace collision {
		btCollisionShape* createShape(const ememory::SharedPtr<ege::resource::Mesh>& _mesh);
	}
}

