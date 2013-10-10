/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_ELEMENT_GAME_H__
#define __EGE_ELEMENT_GAME_H__

#include <etk/types.h>
#include <etk/math/Vector3D.h>
#include <etk/math/Matrix4.h>
#include <etk/Vector.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/renderer/openGL.h>
#include <ewol/resources/ResourceManager.h>
#include <ege/Camera.h>
#include <ewol/compositing/Text.h>
#include <ege/Environement.h>

#define INDEX_RIGHT_AXIS   (0)
#define INDEX_FORWARD_AXIS (1)
#define INDEX_UP_AXIS      (2)

#define ELEMENT_SCALE     (1.0f/8.0f)

namespace ege
{
	class ElementGame
	{
		private:
			static void FunctionFreeShape(void* _pointer);
		protected:
			ege::Environement& m_env;
		protected:
			btRigidBody* m_body; //!< all the element have a body  == > otherwise it will be not manage with this system...
		public:
			/**
			 * @brief Constructor (when constructer is called just add element that did not change.
			 * The objest will be stored in a pool of element and keep a second time if needed  == > redure memory allocation,
			 * when needed, the system will call the init and un-init function...
			 */
			ElementGame(ege::Environement& _env);
			/**
			 * @brief Destructor
			 */
			virtual ~ElementGame(void);
			/**
			 * @brief get the element Type description string.
			 * @return A reference on the descriptive string.
			 */
			virtual const etk::UString& getType(void) const;
			/**
			 * @brief init the element with the defined properties
			 * @param[in] _property Type of the next element
			 * @param[in] _value pointer on the value type
			 * @return true, the element is corectly initialized.
			 */
			virtual bool init(property_te _property, void* _value) {
				return false;
			};
			virtual bool unInit(void) {
				return true;
			};
		private:
			uint32_t m_uID; //!< This is a reference on a basic element ID
		public:
			/**
			 * @brief get the curent Element Unique ID in the all Game.
			 * @return The requested Unique ID.
			 */
			inline uint32_t getUID(void) const {
				return m_uID;
			};
		private:
			ewol::Mesh* m_mesh; //!< Mesh of the Element (can be NULL)
			btCollisionShape* m_shape; //!< shape of the element (set a copy here to have the debug display of it)
		public:
			/**
			 * @brief Select a mesh with a specific name.
			 * @param[in] _meshFileName filename of the Mesh.
			 * @note Automaticly load the shape if it is specify in the mesh file
			 * @return true if no error occured
			 */
			bool loadMesh(const etk::UString& _meshFileName);
			/**
			 * @brief set the the Mesh properties.
			 * @param[in] _mesh The mesh pointer. (NULL to force the mesh remove ...)
			 * @note : this remove the shape and the mesh properties.
			 * @return true if no error occured
			 */
			bool setMesh(ewol::Mesh* _mesh);
			/**
			 * @brief set the shape properties.
			 * @param[in] _shape The shape pointer.
			 * @note : this remove the shape properties.
			 * @return true if no error occured
			 */
			bool setShape(btCollisionShape* _shape);
			/**
			 * @brief get a pointer on the Mesh file.
			 * @return the mesh pointer.
			 */
			inline ewol::Mesh* getMesh(void) {
				return m_mesh;
			};
			/**
			 * @brief get a pointer on the bullet collision shape.
			 * @return the collision pointer.
			 */
			inline btCollisionShape* getShape(void) {
				return m_shape;
			};
		private:
			/**
			 * @brief remove the curent selected shape.
			 */
			void removeShape(void);
		protected:
			float m_life; //!< Current life of the object
			float m_lifeMax; //!< Maximum possible life of the element
		public:
			/**
			 * @brief get the curent life ratio [0..1]
			 * @return The proportionnal life
			 */
			float getLifeRatio(void);
			/**
			 * @brief Check if the element is dead.
			 * @return true if the element does not exist anymore, false otherwise.
			 */
			bool isDead(void) {
				return (0 >= m_life)?true:false;
			};
			/**
			 * @brief Request if the element might be removed from the system
			 * @return true  == > the object is removed
			 */
			virtual bool needToRemove(void) {
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
			 * @brief Call chan the element life change
			 */
			virtual void onLifeChange(void) { };
		protected:
			int32_t m_group; //!< Every element has a generic group
		public:
			/**
			 * @brief get the Group of the element.
			 * @return The group ID
			 */
			inline int32_t getGroup(void) const {
				return m_group;
			};
			/**
			 * @brief set the group of the curent element
			 * @param[in] newGroup The new Group ID of the element.
			 */
			inline void setGroup(int32_t _newGroup) {
				m_group=_newGroup;
			};
		
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
			virtual void draw(int32_t _pass=0);
			
			/**
			 * @brief draw the current life of the element
			 */
			virtual void drawLife(ewol::Colored3DObject* _draw, const ege::Camera& _camera);
			
		protected:
			// For debug only ...
			ewol::Text m_debugText;
		public:
			/**
			 * @brief Debug display of the current element
			 * @param[in,out] draw Basic system to draw the debug shape and informations
			 */
			virtual void drawDebug(ewol::Colored3DObject* _draw, const ege::Camera& _camera);
			
			/**
			 * @brief get the theoric position. Sometimes, the element has move due to an explosion or something else, then its real position in not the one that woult it be at the end ...
			 * @return the theoric position
			 */
			virtual vec3 getPositionTheoric(void) {
				return getPosition();
			};
			/**
			 * @brief set the current Theoric position of the element
			 * @param[in] set the 3D position.
			 */
			virtual void setPositionTheoric(const vec3& _pos) { };
			/**
			 * @brief get the current position of the element
			 * @return the 3D position.
			 */
			const vec3& getPosition(void);
			/**
			 * @brief set the current position of the element
			 * @param[in] _pos set the 3D position.
			 */
			void setPosition(const vec3& _pos);
			/**
			 * @brief get the current speed of the element
			 * @return the 3D speed.
			 */
			const vec3& getSpeed(void);
			/**
			 * @brief get the current mass of the element
			 * @return the mass in kG.
			 */
			const float getInvMass(void);
			/**
			 * @brief Event arrive when an element has been remove from the system  == > this permit to keep pointer of ennemy, and not search them every cycle ...
			 * @param[in] _removedElement Pointer on the element removed.
			 */
			virtual void elementIsRemoved(ege::ElementGame* _removedElement) { };
		protected:
			bool m_fixe; //!< is a fixed element  == > used for placement of every elements
		public:
			/**
			 * @brief get the element if it is fixed or not. if the element is fixed this is for tower, and all thing does not really move
			 * @return true : The element is fixed.
			 */
			inline bool isFixed(void) {
				return m_fixe;
			};
		protected:
			float m_radius; //!< Radius of the element (all element have a radius, if  == 0 ==> then ghost ...
		public:
			/**
			 * @brief get the current space needed by the element in the workspace
			 * @return The dimention needed.
			 */
			inline float getRadius(void)
			{
				return m_radius;
			};
		protected:
			bool m_elementInPhysicsSystem;
		public:
			/**
			 * @brief set the elment in the physique engine
			 */
			void dynamicEnable(void);
			/**
			 * @brief remove this element from the physique engine
			 */
			void dynamicDisable(void);
		private:
			class localIA : public btActionInterface {
				private:
					ege::ElementGame& m_element;
				public:
					/**
					 * @brief Constructor
					 */
					localIA(ElementGame& element) : m_element(element) { };
					/**
					 * @brief Destructor
					 */
					virtual ~localIA(void) {
						
					};
				public: // herited function
					void debugDraw(btIDebugDraw* _debugDrawer) {
						
					};
					void updateAction(btCollisionWorld* _collisionWorld, btScalar _step) {
						m_element.iaAction(_step);
					};
			};
			localIA* m_IA;
		public:
			/**
			 * @brief enable periodic call Of this object for processing Artificial Intelligence
			 */
			void iaEnable(void);
			/**
			 * @brief disable periodic call Of this object for processing Artificial Intelligence
			 */
			void iaDisable(void);
			/**
			 * @brief periodic call for intelligence artificial.
			 * @param[in] step : step of time in s
			 */
			virtual void iaAction(float _step) { };
			/**
			 * @brief, call when the element is removed (call only one time
			 */
			virtual void onDestroy(void) {};
	};
};

#endif


