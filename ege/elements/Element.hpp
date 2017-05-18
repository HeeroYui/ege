/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Matrix4x4.hpp>
#include <vector>
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

#define ELEMENT_SCALE     (1.0f/8.0f)

namespace ege {
	class Element : public ememory::EnableSharedFromThis<Element> {
		protected:
			ememory::SharedPtr<ege::Environement> m_env;
		public:
			/**
			 * @brief Constructor (when constructer is called just add element that did not change.
			 * The objest will be stored in a pool of element and keep a second time if needed  == > redure memory allocation,
			 * when needed, the system will call the init and un-init function...
			 */
			Element(const ememory::SharedPtr<ege::Environement>& _env);
			/**
			 * @brief Destructor
			 */
			virtual ~Element();
		protected:
			std::vector<ememory::SharedPtr<ege::Component>> m_component;
		public:
			void addComponent(const ememory::SharedPtr<ege::Component>& _ref);
			void rmComponent(const ememory::SharedPtr<ege::Component>& _ref);
			void rmComponent(const std::string& _type);
			
			/**
			 * @brief get the element Type description string.
			 * @return A reference on the descriptive string.
			 */
			virtual const std::string& getType() const;
			/**
			 * @brief init the element with the defined properties
			 * @param[in] _property Type of the next element
			 * @param[in] _value pointer on the value type
			 * @return true, the element is corectly initialized.
			 */
			virtual bool init();
			virtual bool initString(const std::string& _description);
			virtual bool initXML(const exml::Node& _node);
			virtual bool initJSON(const ejson::Value& _value);
			virtual bool initVoid(void* _value);
			virtual bool unInit();
		private:
			uint32_t m_uID; //!< This is a reference on a basic element ID
		public:
			/**
			 * @brief get the curent Element Unique ID in the all Game.
			 * @return The requested Unique ID.
			 */
			inline uint32_t getUID() const {
				return m_uID;
			};
		/*
		 * *********************************
		 *     Remove in progress .... [BEGIN]
		 * *********************************
		 */
		protected:
			ememory::SharedPtr<ege::resource::Mesh> m_mesh; //!< Mesh of the Element (can be nullptr)
		public:
			/**
			 * @brief Select a mesh with a specific name.
			 * @param[in] _meshFileName filename of the Mesh.
			 * @note Automaticly load the shape if it is specify in the mesh file
			 * @return true if no error occured
			 */
			virtual bool loadMesh(const std::string& _meshFileName);
			/**
			 * @brief set the the Mesh properties.
			 * @param[in] _mesh The mesh pointer. (nullptr to force the mesh remove ...)
			 * @note : this remove the shape and the mesh properties.
			 * @return true if no error occured
			 */
			virtual bool setMesh(ememory::SharedPtr<ege::resource::Mesh> _mesh);
			/**
			 * @brief get a pointer on the Mesh file.
			 * @return the mesh pointer.
			 */
			inline ememory::SharedPtr<ege::resource::Mesh> getMesh() {
				return m_mesh;
			};
		/*
		 * *********************************
		 *     Remove in progress .... [END]
		 * *********************************
		 */
		protected:
			float m_life; //!< Current life of the object
			float m_lifeMax; //!< Maximum possible life of the element
		public:
			/**
			 * @brief get the curent life ratio [0..1]
			 * @return The proportionnal life
			 */
			float getLifeRatio();
			/**
			 * @brief Check if the element is dead.
			 * @return true if the element does not exist anymore, false otherwise.
			 */
			bool isDead() {
				return (0 >= m_life)?true:false;
			};
			/**
			 * @brief Request if the element might be removed from the system
			 * @return true  == > the object is removed
			 */
			virtual bool needToRemove() {
				return isDead();
			}
			/**
			 * @brief apply a fire on the element at a current power and a specific power.
			 * @param[in] _groupIdSource Source Id of the group, by default all event arrive at all group, buf some event can not be obviously apply at the ennemy like reparing ....
			 * @param[in] _type Type of event on the life propertied
			 * @param[in] _power Power of the event (can be >0 for adding life).
			 * @param[in] _center Some fire decrease in function of space distance...
			 */
			virtual void setFireOn(int32_t _groupIdSource, int32_t _type, float _power, const vec3& _center=vec3(0,0,0));
			/**
			 * @brief Call when the element life change.
			 */
			virtual void onLifeChange() { };
		protected:
			int32_t m_group; //!< Every element has a generic group
		public:
			/**
			 * @brief get the Group of the element.
			 * @return The group ID
			 */
			inline int32_t getGroup() const {
				return m_group;
			};
			/**
			 * @brief set the group of the curent element
			 * @param[in] newGroup The new Group ID of the element.
			 */
			inline void setGroup(int32_t _newGroup) {
				m_group=_newGroup;
			};
		public:
			/**
			 * @brief Can be call tu opdate the list of the element displayed on the scren (example : no display of the hiden triangle)
			 * @param[in] the camera properties
			 * @ note by default nothing to do ...
			 */
			virtual void preCalculationDraw(const ege::Camera& _camera) { };
			/**
			 * @brief draw the curent element (can have multiple display)
			 * @param[in] pass Id of the current pass : [0..?]
			 */
			virtual void draw(int32_t _pass=0) { };
			
			/**
			 * @brief draw the current life of the element
			 */
			// TODO : Remove this ...
			virtual void drawLife(ememory::SharedPtr<ewol::resource::Colored3DObject> _draw, ememory::SharedPtr<ege::Camera> _camera);
			
		protected:
			// For debug only ...
			ewol::compositing::Text m_debugText;
		public:
			/**
			 * @brief Debug display of the current element
			 * @param[in,out] _draw Basic system to draw the debug shape and informations
			 * @param[in] _camera Current camera for display
			 */
			virtual void drawDebug(ememory::SharedPtr<ewol::resource::Colored3DObject> _draw, ememory::SharedPtr<ege::Camera> _camera);
			/**
			 * @brief Debug display of the current element normal face
			 * @param[in,out] _draw Basic system to draw the debug shape and informations
			 * @param[in] _camera Current camera for display
			 */
			virtual void drawNormalDebug(ememory::SharedPtr<ewol::resource::Colored3DObject> _draw, ememory::SharedPtr<ege::Camera> _camera);
			
			/**
			 * @brief get the theoric position. Sometimes, the element has move due to an explosion or something else, then its real position in not the one that woult it be at the end ...
			 * @return the theoric position
			 */
			virtual vec3 getPositionTheoric() {
				return getPosition();
			};
			/**
			 * @brief get the current position of the element
			 * @return the 3D position.
			 */
			virtual const vec3& getPosition();
			/**
			 * @brief set the current position of the element
			 * @param[in] _pos set the 3D position.
			 */
			virtual void setPosition(const vec3& _pos) {};
			/**
			 * @brief Event arrive when an element has been remove from the system  == > this permit to keep pointer of ennemy, and not search them every cycle ...
			 * @param[in] _removedElement Pointer on the element removed.
			 */
			virtual void elementIsRemoved(ememory::SharedPtr<ege::Element> _removedElement) { };
		protected:
			bool m_fixe; //!< is a fixed element  == > used for placement of every elements
		public:
			/**
			 * @brief get the element if it is fixed or not. if the element is fixed this is for tower, and all thing does not really move
			 * @return true : The element is fixed.
			 */
			inline bool isFixed() {
				return m_fixe;
			};
		protected:
			float m_radius; //!< Radius of the element (all element have a radius, if  == 0 ==> then ghost ...
		public:
			/**
			 * @brief get the current space needed by the element in the workspace
			 * @return The dimention needed.
			 */
			inline float getRadius() {
				return m_radius;
			};
			/**
			 * @brief, call when the element is removed (call only one time)
			 */
			virtual void onDestroy() {};
			/**
			 * @brief set the elment in the physique engine
			 */
			virtual void dynamicEnable() {};
			/**
			 * @brief remove this element from the physique engine
			 */
			virtual void dynamicDisable() {};
		
	};
}


