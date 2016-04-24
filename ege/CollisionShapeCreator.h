/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ege/resource/Mesh.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

namespace ege {
	namespace collision {
		btCollisionShape* createShape(const std::shared_ptr<ege::resource::Mesh>& _mesh);
	}
}

