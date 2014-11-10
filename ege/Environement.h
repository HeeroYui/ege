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
#include <ege/camera/Camera.h>

#include <etk/types.h>
#include <BulletDynamics/Dynamics/btActionInterface.h>
class btDynamicsWorld;

#include <vector>
#include <etk/math/Vector3D.h>
#include <ejson/ejson.h>
#include <exml/exml.h>
#include <ege/ParticuleEngine.h>
#include <ewol/object/Object.h>
#include <ewol/signal/Signal.h>
#include <ewol/event/Time.h>
#include <ewol/parameter/Value.h>
#include <ege/resource/Mesh.h>
#include <ege/physics/Engine.h>

namespace ege {
	enum property {
		typeNone, //!< no element property
		typeString, //!< type element static_cast<std::string*>(xxxxxxx)
		typeJson, //!< type element static_cast<ejson::Value*>(xxxxxxx)
		typeXml, //!< type element static_cast<exml::Node*>(xxxxxxx)
		typeUser1, //!< user type 1
		typeUser2 //!< User type 2
	};
	class Element;
	class Environement;
	typedef std::shared_ptr<ege::Element> (*createElement_tf)(const std::shared_ptr<ege::Environement>& _env);
	
	enum gameStatus {
		gameStart,
		gamePause,
		gameStop
	};
	
	class ElementInteraction {
		protected:
			int32_t m_type;
		public:
			int32_t getType() {
				return m_type;
			};
		protected:
			int32_t m_groupSource;
		public:
			int32_t getSourceGroup() {
				return m_groupSource;
			};
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
			virtual void applyEvent(ege::Element& _element) { };
	};
	
	class Environement : public ewol::Object {
		public:
			// extern event
			ewol::Signal<float> signalPlayTimeChange;
		private:
			//std::shared_ptr<btDynamicsWorld> m_dynamicsWorld; //!< curent system world description
			ege::physics::Engine m_physicEngine; //!< EGE physic engine interface.
			std::vector<std::shared_ptr<ege::Element>> m_listElement; //!< List of all element added in the Game
		protected:
			Environement();
			void init();
		public:
			DECLARE_FACTORY(Environement);
			virtual ~Environement() { };
		protected:
			ewol::parameter::List<enum gameStatus> m_status; //!< the display is running (not in pause)
		public:
			/**
			 * @brief Get the game status.
			 * @return the current status.
			 */
			enum gameStatus getGameStatus() {
				return m_status.get();
			}
			/**
			 * @brief Set the game status.
			 * @param[in] _value New game status.
			 */
			void setGameStatus(enum gameStatus _value) {
				m_status.set(_value);
			}
		protected:
			ewol::parameter::Value<float> m_ratio; //!< Speed ratio
		public:
			/**
			 * @brief Get the game speed ratio.
			 * @return the current ratio.
			 */
			float getSpeedRatio() {
				return m_ratio.get();
			}
			/**
			 * @brief Set the game ratio.
			 * @param[in] _value New game ratio.
			 */
			void setSpeedRatio(float _value) {
				m_ratio.set(_value);
			}
		protected:
			std::map<std::string, std::shared_ptr<ege::Camera>> m_listCamera; //!< list of all camera in the world
		public:
			/**
			 * @brief Add a camera in the camera pool.
			 * @param[in] _name Name of the camera.
			 * @param[in] _camera Pointer on the camera to add.
			 */
			void addCamera(const std::string& _name, const std::shared_ptr<ege::Camera>& _camera);
			/**
			 * @brief Get a specific camera.
			 * @param[in] _name Name of the camera.
			 * @return A pointer on the camera requested.
			 */
			std::shared_ptr<ege::Camera> getCamera(const std::string& _name);
		public:
			/**
			 * @brief Remove all from the current environement
			 */
			void clear();
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
			 * @return nullptr if an error occured OR the pointer on the element and it is already added on the system.
			 * @note Pointer is return in case of setting properties on it...
			 */
			std::shared_ptr<ege::Element> createElement(const std::string& _type, bool _autoAddElement=true, enum ege::property _property=ege::typeNone, void* _value=nullptr);
			std::shared_ptr<ege::Element> createElement(const std::string& _type, std::string& _description, bool _autoAddElement=true);
			std::shared_ptr<ege::Element> createElement(const std::string& _type, ejson::Value* _value, bool _autoAddElement=true);
			std::shared_ptr<ege::Element> createElement(const std::string& _type, exml::Node* _node, bool _autoAddElement=true);
		public:
			class ResultNearestElement {
				public:
					std::shared_ptr<ege::Element> element;
					float dist;
			};
			#if 0
			/**
			 * @brief set the curent world
			 * @param[in] _newWorld Pointer on the current world
			 */
			void setDynamicWorld(const std::shared_ptr<btDynamicsWorld>& _newWorld) {
				m_dynamicsWorld=_newWorld;
			};
			/**
			 * @brief get the curent world
			 * @return pointer on the current world
			 */
			std::shared_ptr<btDynamicsWorld> getDynamicWorld() {
				return m_dynamicsWorld;
			};
			#endif
			ege::physics::Engine& getPhysicEngine() {
				return m_physicEngine;
			}
			/**
			 * @breif get a reference on the curent list of element games
			 * @return all element list
			 */
			std::vector<std::shared_ptr<ege::Element>>& getElement() {
				return m_listElement;
			};
			/**
			 * @brief get the nearest Element
			 * @param[in] _sourceRequest Pointer on the element that request this.
			 * @param[in] _distance Maximum distance search  == > return the element distance
			 * @return Pointer on the neares element OR nullptr
			 */
			std::shared_ptr<ege::Element> getElementNearest(std::shared_ptr<ege::Element> _sourceRequest, float& _distance);
			
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
			void addElement(std::shared_ptr<ege::Element> _newElement);
			/**
			 * @brief remove an element on the list availlable.
			 * @param[in] _removeElement Element to remove.
			 */
			void rmElement(std::shared_ptr<ege::Element> _removeElement);
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
		protected:
			int64_t m_gameTime; //!< time of the game running
		public:
			
		private:
			void periodicCall(const ewol::event::Time& _event);
		protected:
			std::vector<std::shared_ptr<ege::resource::Mesh>> m_listMeshToDrawFirst;
		public:
			void addStaticMeshToDraw(const std::shared_ptr<ege::resource::Mesh>& _mesh) {
				m_listMeshToDrawFirst.push_back(_mesh);
			}
			const std::vector<std::shared_ptr<ege::resource::Mesh>>& getStaticMeshToDraw() {
				return m_listMeshToDrawFirst;
			}
	};
};

#endif


