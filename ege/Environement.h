/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_ENVIRONEMENT_H__
#define __EGE_ENVIRONEMENT_H__

namespace ege {
	class Environement;
	class ElementInteraction;
};
#include <ege/Camera.h>

#include <etk/types.h>
#include <BulletDynamics/Dynamics/btActionInterface.h>
class btDynamicsWorld;

#include <vector>
#include <etk/math/Vector3D.h>
#include <ejson/ejson.h>
#include <exml/exml.h>
#include <ege/ParticuleEngine.h>

namespace ege {
	enum property {
		typeNone, //!< no element property
		typeString, //!< type element static_cast<std::string*>(xxxxxxx)
		typeJson, //!< type element static_cast<ejson::Value*>(xxxxxxx)
		typeXml, //!< type element static_cast<exml::Node*>(xxxxxxx)
		typeUser1, //!< user type 1
		typeUser2 //!< User type 2
	};
	class ElementGame;
	class Environement;
	typedef ege::ElementGame* (*createElement_tf)(ege::Environement& _env);
	
	class ElementInteraction {
		protected:
			int32_t m_type;
		public:
			int32_t getType() { return m_type; };
		protected:
			int32_t m_groupSource;
		public:
			int32_t getSourceGroup() { return m_groupSource; };
		protected:
			std::vector<int32_t> m_groupDestination;
		public:
			const std::vector<int32_t>& getDestinationGroup() {
				return m_groupDestination;
			};
			void addGroupDestination(int32_t _id) {
				m_groupDestination.push_back(_id);
			};
		protected:
			vec3 m_positionSource;
		public:
			const vec3& getSourcePosition() {
				return m_positionSource;
			};
		public:
			ElementInteraction(int32_t _type, int32_t _groupSource, const vec3& _pos) : 
				m_type(_type),
				m_groupSource(_groupSource),
				m_positionSource(_pos)
			{ };
		public:
			virtual void applyEvent(ege::ElementGame& _element) { };
	};
	
	class Environement {
		private:
			btDynamicsWorld* m_dynamicsWorld; //!< curent system world description
			std::vector<ege::ElementGame*> m_listElementGame; //!< List of all element added in the Game
		public:
			Environement();
			virtual ~Environement() { };
		public:
			/**
			 * @brief add a creator element system
			 * @param[in] _type Type of the element.
			 * @param[in] _creator Function pointer that reference the element creating.
			 */
			static void addCreator(const std::string& _type, ege::createElement_tf _creator);
			/**
			 * @brief Create an element on the curent scene.
			 * @param[in] _type Type of the element that might be created.
			 * @param[in] _description String that describe the content of the element properties.
			 * @param[in] _autoAddElement this permit to add the element if it is created  == > no more action ...
			 * @return NULL if an error occured OR the pointer on the element and it is already added on the system.
			 * @note Pointer is return in case of setting properties on it...
			 */
			ege::ElementGame* createElement(const std::string& _type, bool _autoAddElement=true, enum ege::property _property=ege::typeNone, void* _value=NULL);
			ege::ElementGame* createElement(const std::string& _type, std::string& _description, bool _autoAddElement=true);
			ege::ElementGame* createElement(const std::string& _type, ejson::Value* _value, bool _autoAddElement=true);
			ege::ElementGame* createElement(const std::string& _type, exml::Node* _node, bool _autoAddElement=true);
		public:
			class ResultNearestElement {
				public:
					ege::ElementGame* element;
					float dist;
			};
			/**
			 * @brief set the curent world
			 * @param[in] _newWorld Pointer on the current world
			 */
			void setDynamicWorld(btDynamicsWorld* _newWorld) {
				m_dynamicsWorld=_newWorld;
			};
			/**
			 * @brief get the curent world
			 * @return pointer on the current world
			 */
			btDynamicsWorld* getDynamicWorld() {
				return m_dynamicsWorld;
			};
			/**
			 * @breif get a reference on the curent list of element games
			 * @return all element list
			 */
			std::vector<ege::ElementGame*>& getElementGame() {
				return m_listElementGame;
			};
			/**
			 * @brief get the nearest Element
			 * @param[in] _sourceRequest Pointer on the element that request this.
			 * @param[in] _distance Maximum distance search  == > return the element distance
			 * @return Pointer on the neares element OR NULL
			 */
			ege::ElementGame* getElementNearest(ege::ElementGame* _sourceRequest, float& _distance);
			
			void getElementNearest(const vec3& _sourcePosition,
			                       float _distanceMax,
			                       std::vector<ege::Environement::ResultNearestElement>& _resultList);
			void getElementNearestFixed(const vec3& _sourcePosition,
			                            float _distanceMax,
			                            std::vector<ege::Environement::ResultNearestElement>& _resultList);
			/**
			 * @brief add an element on the list availlable.
			 * @param[in] _newElement Element to add.
			 */
			void addElementGame(ege::ElementGame* _newElement);
			/**
			 * @brief remove an element on the list availlable.
			 * @param[in] _removeElement Element to remove.
			 */
			void rmElementGame(ege::ElementGame* _removeElement);
			/**
			 * @brief get the element order from the nearest to the farest, and remove all element that are not in the camera angle and axes.
			 * @param[in,out] _resultList List of the element ordered.
			 * @param[in] _position Camera position in the space.
			 * @param[in] _direction Camera direction of the view.
			 */
			void getOrderedElementForDisplay(std::vector<ege::Environement::ResultNearestElement>& _resultList, const vec3& _position, const vec3& _direction);
			/**
			 * @brief generate an event on all the sub element of the game  == > usefull for explosion, or lazer fire ...
			 * @param[in] _event event that might be apply ...
			 */
			void generateInteraction(ege::ElementInteraction& _event);
		private:
			ege::ParticuleEngine m_particuleEngine; //!< Particule engine properties
		public:
			/**
			 * @brief get the particule engine reference.
			 * @return The requested reference on the engine
			 */
			ege::ParticuleEngine& getParticuleEngine() {
				return m_particuleEngine;
			};
	};
};

#endif


