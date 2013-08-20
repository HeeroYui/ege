/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_ENVIRONEMENT_H__
#define __EGE_ENVIRONEMENT_H__

#include <etk/UString.h>
#include <BulletDynamics/Dynamics/btActionInterface.h>
class btDynamicsWorld;

#include <etk/Vector.h>
#include <etk/math/Vector3D.h>
#include <ejson/ejson.h>
#include <exml/exml.h>

namespace ege {
	class ElementGame;
	class Environement;
	typedef ege::ElementGame* (*createElement_string_tf)(ege::Environement& _env, const etk::UString& _description);
	typedef ege::ElementGame* (*createElement_ejson_tf)(ege::Environement& _env, const ejson::Value* _value);
	typedef ege::ElementGame* (*createElement_exml_tf)(ege::Environement& _env, const exml::Node* _node);
	
	class ElementInteraction
	{
		protected:
			int32_t m_type;
		public:
			int32_t GetType(void) { return m_type; };
		protected:
			int32_t m_groupSource;
		public:
			int32_t GetSourceGroup(void) { return m_groupSource; };
		protected:
			etk::Vector<int32_t> m_groupDestination;
		public:
			const etk::Vector<int32_t>& GetDestinationGroup(void) { return m_groupDestination; };
			void AddGroupDestination(int32_t _id) { m_groupDestination.PushBack(_id); };
		protected:
			vec3 m_positionSource;
		public:
			const vec3& GetSourcePosition(void) { return m_positionSource; };
		public:
			ElementInteraction(int32_t _type, int32_t _groupSource, const vec3& _pos) : 
				m_type(_type),
				m_groupSource(_groupSource),
				m_positionSource(_pos)
			{ };
		public:
			virtual void ApplyEvent(ege::ElementGame& _element) { };
	};
	
	class Environement
	{
		private:
			btDynamicsWorld* m_dynamicsWorld; //!< curent system world description
			etk::Vector<ege::ElementGame*> m_listElementGame; //!< List of all element added in the Game
		public:
			Environement(void) : m_dynamicsWorld(NULL) { };
			virtual ~Environement(void) { };
		public:
			/**
			 * @brief Add a creator element system
			 * @param[in] _type Type of the element.
			 * @param[in] _creator Function pointer that reference the element creating.
			 */
			static void AddCreator(const etk::UString& _type, ege::createElement_string_tf _creator);
			static void AddCreator(const etk::UString& _type, ege::createElement_ejson_tf _creator);
			static void AddCreator(const etk::UString& _type, ege::createElement_exml_tf _creator);
			/**
			 * @brief Create an element on the curent scene.
			 * @param[in] _type Type of the element that might be created.
			 * @param[in] _description String that describe the content of the element properties.
			 * @param[in] _autoAddElement this permit to add the element if it is created ==> no more action ...
			 * @return NULL if an error occured OR the pointer on the element and it is already added on the system.
			 * @note Pointer is return in case of setting properties on it...
			 */
			ege::ElementGame* CreateElement(const etk::UString& _type, bool _autoAddElement=true);
			ege::ElementGame* CreateElement(const etk::UString& _type, const etk::UString& _description, bool _autoAddElement=true);
			ege::ElementGame* CreateElement(const etk::UString& _type, const ejson::Value* _value, bool _autoAddElement=true);
			ege::ElementGame* CreateElement(const etk::UString& _type, const exml::Node* _node, bool _autoAddElement=true);
		protected:
			ege::ElementGame* CreateElement(const etk::UString& _type, const etk::UString* _description, const ejson::Value* _value, const exml::Node* _node, bool _autoAddElement);
		public:
			class ResultNearestElement
			{
				public:
					ege::ElementGame* element;
					float dist;
			};
			/**
			 * @brief Set the curent world
			 * @param[in] _newWorld Pointer on the current world
			 */
			void SetDynamicWorld(btDynamicsWorld* _newWorld) { m_dynamicsWorld=_newWorld; };
			/**
			 * @brief Get the curent world
			 * @return pointer on the current world
			 */
			btDynamicsWorld* GetDynamicWorld(void) { return m_dynamicsWorld; };
			/**
			 * @breif Get a reference on the curent list of element games
			 * @return all element list
			 */
			etk::Vector<ege::ElementGame*>& GetElementGame(void) { return m_listElementGame; };
			/**
			 * @brief Get the nearest Element
			 * @param[in] _sourceRequest Pointer on the element that request this.
			 * @param[in] _distance Maximum distance search ==> return the element distance
			 * @return Pointer on the neares element OR NULL
			 */
			ege::ElementGame* GetElementNearest(ege::ElementGame* _sourceRequest, float& _distance);
			
			void GetElementNearest(const vec3& _sourcePosition, float _distanceMax, etk::Vector<ege::Environement::ResultNearestElement>& _resultList);
			void GetElementNearestFixed(const vec3& _sourcePosition, float _distanceMax, etk::Vector<ege::Environement::ResultNearestElement>& _resultList);
			/**
			 * @brief Add an element on the list availlable.
			 * @param[in] _newElement Element to add.
			 */
			void AddElementGame(ege::ElementGame* _newElement);
			/**
			 * @brief Remove an element on the list availlable.
			 * @param[in] _removeElement Element to remove.
			 */
			void RmElementGame(ege::ElementGame* _removeElement);
			/**
			 * @brief Get the element order from the nearest to the farest, and remove all element that are not in the camera angle and axes.
			 * @param[in,out] _resultList List of the element ordered.
			 * @param[in] _position Camera position in the space.
			 * @param[in] _direction Camera direction of the view.
			 */
			void GetOrderedElementForDisplay(etk::Vector<ege::Environement::ResultNearestElement>& _resultList, const vec3& _position, const vec3& _direction);
			/**
			 * @brief Generate an event on all the sub element of the game ==> usefull for explosion, or lazer fire ...
			 * @param[in] _event event that might be apply ...
			 */
			void GenerateInteraction(ege::ElementInteraction& _event);
	};
};

#endif


