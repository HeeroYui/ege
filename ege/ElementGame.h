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
#include <ewol/renderer/ResourceManager.h>
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
			btRigidBody* m_body; //!< all the element have a body ==> otherwise it will be not manage with this system...
		public:
			/**
			 * @brief Constructor (when constructer is called just add element that did not change.
			 * The objest will be stored in a pool of element and keep a second time if needed ==> redure memory allocation,
			 * when needed, the system will call the Init and un-init function...
			 */
			ElementGame(ege::Environement& _env);
			/**
			 * @brief Destructor
			 */
			virtual ~ElementGame(void);
			/**
			 * @brief Get the element Type description string.
			 * @return A reference on the descriptive string.
			 */
			virtual const etk::UString& GetType(void) const;
			/**
			 * @brief Init the element with the defined properties
			 * @param[in] _description String properties that the element know how to parse it... 
			 * @note : When the useer reques no parameter ==> if will call this one.
			 * @note : In a game developpement the user choice a transmission mode (string, json or xml) and use only one ...
			 * @return true, the element is corectly initialized.
			 */
			virtual bool Init(const etk::UString& _description) { return false; };
			/**
			 * @brief Init the element with the defined properties
			 * @param[in] _value json properties that the element know how to parse it...
			 * @note : in a game developpement the user choice a transmission mode (string, json or xml) and use only one ...
			 * @return true, the element is corectly initialized.
			 */
			virtual bool Init(ejson::Value* _value) { return false; };
			/**
			 * @brief Init the element with the defined properties
			 * @param[in] _node xml properties that the element know how to parse it...
			 * @note : in a game developpement the user choice a transmission mode (string, json or xml) and use only one ...
			 * @return true, the element is corectly initialized.
			 */
			virtual bool Init(exml::Node* _value) { return false; };
			virtual bool UnInit(void) { return true; };
		private:
			uint32_t m_uID; //!< This is a reference on a basic element ID
		public:
			/**
			 * @brief Get the curent Element Unique ID in the all Game.
			 * @return The requested Unique ID.
			 */
			inline uint32_t GetUID(void) const { return m_uID; };
		private:
			ewol::Mesh* m_mesh; //!< Mesh of the Element (can be NULL)
			btCollisionShape* m_shape; //!< shape of the element (set a copy here to have the debug display of it)
		public:
			/**
			 * @brief Select a mesh with a specific name.
			 * @param[in] _meshFileName Filename of the Mesh.
			 * @note Automaticly load the shape if it is specify in the mesh file
			 * @return true if no error occured
			 */
			bool LoadMesh(const etk::UString& _meshFileName);
			/**
			 * @brief Set the the Mesh properties.
			 * @param[in] _mesh The mesh pointer. (NULL to force the mesh remove ...)
			 * @note : this remove the shape and the mesh properties.
			 * @return true if no error occured
			 */
			bool SetMesh(ewol::Mesh* _mesh);
			/**
			 * @brief Set the shape properties.
			 * @param[in] _shape The shape pointer.
			 * @note : this remove the shape properties.
			 * @return true if no error occured
			 */
			bool SetShape(btCollisionShape* _shape);
			/**
			 * @brief Get a pointer on the Mesh file.
			 * @return the mesh pointer.
			 */
			inline ewol::Mesh* GetMesh(void) { return m_mesh; };
			/**
			 * @brief Get a pointer on the bullet collision shape.
			 * @return the collision pointer.
			 */
			inline btCollisionShape* GetShape(void) { return m_shape; };
		private:
			/**
			 * @brief Remove the curent selected shape.
			 */
			void RemoveShape(void);
		protected:
			float m_life; //!< Current life of the object
			float m_lifeMax; //!< Maximum possible life of the element
		public:
			/**
			 * @brief Get the curent life ratio [0..1]
			 * @return The proportionnal life
			 */
			float GetLifeRatio(void);
			/**
			 * @brief Check if the element is dead.
			 * @return true if the element does not exist anymore, false otherwise.
			 */
			bool IsDead(void) { return (0>=m_life)?true:false; };
			/**
			 * @brief Request if the element might be removed from the system
			 * @return true ==> the object is removed
			 */
			virtual bool NeedToRemove(void)
			{
				return IsDead();
			}
			/**
			 * @brief Apply a fire on the element at a current power and a specific power.
			 * @param[in] groupIdSource Source Id of the group, by default all event arrive at all group, buf some event can not be obviously apply at the ennemy like reparing ....
			 * @param[in] type Type of event on the life propertied
			 * @param[in] power Power of the event (can be >0 for adding life).
			 * @param[in] center Some fire decrease in function of space distance...
			 */
			virtual void SetFireOn(int32_t groupIdSource, int32_t type, float power, const vec3& center=vec3(0,0,0));
			/**
			 * @brief Call chan the element life change
			 */
			virtual void OnLifeChange(void) {}
		protected:
			int32_t m_group; //!< Every element has a generic group
		public:
			/**
			 * @brief Get the Group of the element.
			 * @return The group ID
			 */
			inline int32_t GetGroup(void) const { return m_group; };
			/**
			 * @brief Set the group of the curent element
			 * @param[in] newGroup The new Group ID of the element.
			 */
			inline void SetGroup(int32_t _newGroup) { m_group=_newGroup; };
		
			/**
			 * @brief Can be call tu opdate the list of the element displayed on the scren (example : no display of the hiden triangle)
			 * @param[in] the camera properties
			 * @ note by default nothing to do ...
			 */
			virtual void PreCalculationDraw(const ege::Camera& _camera) { };
			/**
			 * @brief Draw the curent element (can have multiple display)
			 * @param[in] pass Id of the current pass : [0..?]
			 */
			virtual void Draw(int32_t _pass=0);
			
			/**
			 * @brief Draw the current life of the element
			 */
			virtual void DrawLife(ewol::Colored3DObject* _draw, const ege::Camera& _camera);
			
		protected:
			// For debug only ...
			ewol::Text m_debugText;
		public:
			/**
			 * @brief Debug display of the current element
			 * @param[in,out] draw Basic system to draw the debug shape and informations
			 */
			virtual void DrawDebug(ewol::Colored3DObject* _draw, const ege::Camera& _camera);
			
			/**
			 * @brief Get the theoric position. Sometimes, the element has move due to an explosion or something else, then its real position in not the one that woult it be at the end ...
			 * @return the theoric position
			 */
			virtual vec3 GetPositionTheoric(void) { return GetPosition(); };
			/**
			 * @brief Set the current Theoric position of the element
			 * @param[in] Set the 3D position.
			 */
			virtual void SetPositionTheoric(const vec3& _pos) { }
			/**
			 * @brief Get the current position of the element
			 * @return the 3D position.
			 */
			const vec3& GetPosition(void);
			/**
			 * @brief Set the current position of the element
			 * @param[in] _pos Set the 3D position.
			 */
			void SetPosition(const vec3& _pos);
			/**
			 * @brief Get the current speed of the element
			 * @return the 3D speed.
			 */
			const vec3& GetSpeed(void);
			/**
			 * @brief Get the current mass of the element
			 * @return the mass in kG.
			 */
			const float GetInvMass(void);
			/**
			 * @brief Event arrive when an element has been remove from the system ==> this permit to keep pointer of ennemy, and not search them every cycle ...
			 * @param[in] _removedElement Pointer on the element removed.
			 */
			virtual void ElementIsRemoved(ege::ElementGame* _removedElement) { };
		protected:
			bool m_fixe; //!< Is a fixed element ==> used for placement of every elements
		public:
			/**
			 * @brief Get the element if it is fixed or not. if the element is fixed this is for tower, and all thing does not really move
			 * @return true : The element is fixed.
			 */
			inline bool IsFixed(void) { return m_fixe; };
		protected:
			float m_radius; //!< Radius of the element (all element have a radius, if ==0 ==> then ghost ...
		public:
			/**
			 * @brief Get the current space needed by the element in the workspace
			 * @return The dimention needed.
			 */
			inline float GetRadius(void)
			{
				return m_radius;
			};
		protected:
			bool m_elementInPhysicsSystem;
		public:
			/**
			 * @brief Set the elment in the physique engine
			 */
			void DynamicEnable(void);
			/**
			 * @brief Remove this element from the physique engine
			 */
			void DynamicDisable(void);
		private:
			class localIA : public btActionInterface
			{
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
					virtual ~localIA(void) { };
					
					// herited function
					void debugDraw(btIDebugDraw* debugDrawer) { };
					// herited function
					void updateAction(btCollisionWorld* collisionWorld, btScalar step)
					{
						m_element.IAAction(step);
					}
			};
			localIA* m_IA;
		public:
			/**
			 * @brief Enable periodic call Of this object for processing Artificial Intelligence
			 */
			void IAEnable(void);
			/**
			 * @brief Disable periodic call Of this object for processing Artificial Intelligence
			 */
			void IADisable(void);
			/**
			 * @brief Periodic call for intelligence artificial.
			 * @param[in] step : step of time in s
			 */
			virtual void IAAction(float _step) { };
	};
};

#endif


