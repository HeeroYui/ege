/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 * 
 */

#ifndef __EGE_COLLISION_SHAPE_CREATOR_H__
#define __EGE_COLLISION_SHAPE_CREATOR_H__

#include <etk/types.h>
#include <ewol/resources/Mesh.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

namespace ege {
	namespace collision {
		btCollisionShape* CreateShape(const ewol::Mesh* _mesh);
	};
};
#endif

