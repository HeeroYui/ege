/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Matrix4x4.hpp>
#include <etk/Vector.hpp>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <gale/renderer/openGL/openGL.hpp>
#include <ewol/resource/Colored3DObject.hpp>
#include <ege/resource/Mesh.hpp>
#include <ege/camera/Camera.hpp>
#include <ewol/compositing/Text.hpp>
#include <ege/Environement.hpp>
#include <ege/Component.hpp>

#define INDEX_RIGHT_AXIS   (0)
#define INDEX_FORWARD_AXIS (1)
#define INDEX_UP_AXIS      (2)

#define Entity_SCALE     (1.0f/8.0f)

namespace ege {
	class Entity : public ememory::EnableSharedFromThis<Entity> {
		protected:
			ememory::SharedPtr<ege::Environement> m_env;
		public:
			/**
			 * @brief Constructor (when constructer is called just add Entity that did not change.
			 * The objest will be stored in a pool of Entity and keep a second time if needed  == > redure memory allocation,
			 * when needed, the system will call the init and un-init function...
			 */
			Entity(const ememory::SharedPtr<ege::Environement>& _env);
			/**
			 * @brief Destructor
			 */
			virtual ~Entity();
		protected:
			etk::Vector<ememory::SharedPtr<ege::Component>> m_component;
		public:
			void addComponent(const ememory::SharedPtr<ege::Component>& _ref);
			void rmComponent(const ememory::SharedPtr<ege::Component>& _ref);
			void rmComponent(const etk::String& _type);
			
			/**
			 * @brief get the Entity Type description string.
			 * @return A reference on the descriptive string.
			 */
			virtual const etk::String& getType() const;
			/**
			 * @brief init the Entity with the defined properties
			 * @param[in] _property Type of the next Entity
			 * @param[in] _value pointer on the value type
			 * @return true, the Entity is corectly initialized.
			 */
			virtual bool init();
			virtual bool initString(const etk::String& _description);
			virtual bool initXML(const exml::Node& _node);
			virtual bool initJSON(const ejson::Value& _value);
			virtual bool initVoid(void* _value);
			virtual bool unInit();
		private:
			uint32_t m_uID; //!< This is a reference on a basic Entity ID
		public:
			/**
			 * @brief get the curent Entity Unique ID in the all Game.
			 * @return The requested Unique ID.
			 */
			inline uint32_t getUID() const {
				return m_uID;
			};
		protected:
			float m_life; //!< Current life of the object
			float m_lifeMax; //!< Maximum possible life of the Entity
		public:
			/**
			 * @brief get the curent life ratio [0..1]
			 * @return The proportionnal life
			 */
			float getLifeRatio();
			/**
			 * @brief Check if the Entity is dead.
			 * @return true if the Entity does not exist anymore, false otherwise.
			 */
			bool isDead() {
				return (0 >= m_life)?true:false;
			};
			/**
			 * @brief Request if the Entity might be removed from the system
			 * @return true  == > the object is removed
			 */
			virtual bool needToRemove() {
				return isDead();
			}
			/**
			 * @brief apply a fire on the Entity at a current power and a specific power.
			 * @param[in] _groupIdSource Source Id of the group, by default all event arrive at all group, buf some event can not be obviously apply at the ennemy like reparing ....
			 * @param[in] _type Type of event on the life propertied
			 * @param[in] _power Power of the event (can be >0 for adding life).
			 * @param[in] _center Some fire decrease in function of space distance...
			 */
			virtual void setFireOn(int32_t _groupIdSource, int32_t _type, float _power, const vec3& _center=vec3(0,0,0));
			/**
			 * @brief Call when the Entity life change.
			 */
			virtual void onLifeChange() { };
		protected:
			int32_t m_group; //!< Every Entity has a generic group
		public:
			/**
			 * @brief get the Group of the Entity.
			 * @return The group ID
			 */
			inline int32_t getGroup() const {
				return m_group;
			};
			/**
			 * @brief set the group of the curent Entity
			 * @param[in] newGroup The new Group ID of the Entity.
			 */
			inline void setGroup(int32_t _newGroup) {
				m_group=_newGroup;
			};
		protected:
			// For debug only ...
			//ewol::compositing::Text m_debugText; // ==> this is reall y a bad idea==> it is inneficient ...
		public:
			/**
			 * @brief Debug display of the current Entity
			 * @param[in,out] _draw Basic system to draw the debug shape and informations
			 * @param[in] _camera Current camera for display
			 */
			virtual void drawDebug(ememory::SharedPtr<ewol::resource::Colored3DObject> _draw, ememory::SharedPtr<ege::Camera> _camera);
			/**
			 * @brief Event arrive when an Entity has been remove from the system  == > this permit to keep pointer of ennemy, and not search them every cycle ...
			 * @param[in] _removedEntity Pointer on the Entity removed.
			 */
			virtual void entityIsRemoved(ememory::SharedPtr<ege::Entity> _removedEntity) { };
		protected:
			float m_radius; //!< Radius of the Entity (all Entity have a radius, if  == 0 ==> then ghost ...
		public:
			/**
			 * @brief get the current space needed by the Entity in the workspace
			 * @return The dimention needed.
			 */
			inline float getRadius() {
				return m_radius;
			};
			/**
			 * @brief, call when the Entity is removed (call only one time)
			 */
			virtual void onDestroy() {};
			/**
			 * @brief set the elment in the physique engine
			 */
			virtual void dynamicEnable() {};
			/**
			 * @brief remove this Entity from the physique engine
			 */
			virtual void dynamicDisable() {};
		
	};
}


