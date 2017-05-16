/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

namespace ege {
	class Environement;
	class ElementInteraction;
};
#include <ege/camera/Camera.hpp>
#include <ege/ParticuleEngine.hpp>
#include <etk/types.hpp>
#include <vector>
#include <etk/math/Vector3D.hpp>
#include <ejson/ejson.hpp>
#include <exml/exml.hpp>
#include <ewol/object/Object.hpp>
#include <esignal/Signal.hpp>
#include <ewol/event/Time.hpp>
#include <eproperty/Value.hpp>
#include <ege/resource/Mesh.hpp>
#include <ege/physics/Engine.hpp>

namespace ege {
	class Element;
	class Environement;
	typedef ememory::SharedPtr<ege::Element> (*createElement_tf)(const ememory::SharedPtr<ege::Environement>& _env);
	
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
			// Signals
			esignal::Signal<float> signalPlayTimeChange;
			// properties:
			eproperty::List<enum gameStatus> propertyStatus; //!< the display is running (not in pause)
			eproperty::Value<float> propertyRatio; //!< Speed ratio
		private:
			ege::physics::Engine m_physicEngine; //!< EGE physic engine interface.
		public:
			ege::physics::Engine& getPhysicEngine() {
				return m_physicEngine;
			}
		private:
			ege::render::Engine m_renderEngine; //!< EGE rendering engine interface.
		public:
			ege::render::Engine& getRenderEngine() {
				return m_renderEngine;
			}
		private:
			ege::particule::Engine m_particuleEngine; //!< EGE particule engine interface.
		public:
			ege::particule::Engine& getParticuleEngine() {
				return m_particuleEngine;
			}
		private:
			ege::ia::Engine m_iaEngine; //!< EGE Artificial inteligence engine interface.
		public:
			ege::ia::Engine& getIAEngine() {
				return m_iaEngine;
			}
		private:
			std::vector<ememory::SharedPtr<ege::Element>> m_listElement; //!< List of all element added in the Game
		protected:
			Environement();
		public:
			DECLARE_FACTORY(Environement);
			virtual ~Environement() { };
		protected:
		protected:
			std::map<std::string, ememory::SharedPtr<ege::Camera>> m_listCamera; //!< list of all camera in the world
		public:
			/**
			 * @brief Add a camera in the camera pool.
			 * @param[in] _name Name of the camera.
			 * @param[in] _camera Pointer on the camera to add.
			 */
			void addCamera(const std::string& _name, const ememory::SharedPtr<ege::Camera>& _camera);
			/**
			 * @brief Get a specific camera.
			 * @param[in] _name Name of the camera.
			 * @return A pointer on the camera requested.
			 */
			ememory::SharedPtr<ege::Camera> getCamera(const std::string& _name);
			/**
			 * @brief Get List of all camera.
			 * @return All the camera registerred.
			 */
			std::map<std::string, ememory::SharedPtr<ege::Camera>> getCameraList() const {
				return m_listCamera;
			}
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
			ememory::SharedPtr<ege::Element> createElement(const std::string& _type, const std::string& _description, bool _autoAddElement=true);
			ememory::SharedPtr<ege::Element> createElement(const std::string& _type, const ejson::Value& _value, bool _autoAddElement=true);
			ememory::SharedPtr<ege::Element> createElement(const std::string& _type, const exml::Node& _node, bool _autoAddElement=true);
			ememory::SharedPtr<ege::Element> createElement(const std::string& _type, void* _data, bool _autoAddElement=true);
			ememory::SharedPtr<ege::Element> createElement(const std::string& _type, bool _autoAddElement=true);
		public:
			class ResultNearestElement {
				public:
					ememory::SharedPtr<ege::Element> element;
					float dist;
			};
			#if 0
			/**
			 * @brief set the curent world
			 * @param[in] _newWorld Pointer on the current world
			 */
			void setDynamicWorld(const ememory::SharedPtr<btDynamicsWorld>& _newWorld) {
				m_dynamicsWorld=_newWorld;
			};
			/**
			 * @brief get the curent world
			 * @return pointer on the current world
			 */
			ememory::SharedPtr<btDynamicsWorld> getDynamicWorld() {
				return m_dynamicsWorld;
			};
			#endif
			/**
			 * @breif get a reference on the curent list of element games
			 * @return all element list
			 */
			std::vector<ememory::SharedPtr<ege::Element>>& getElement() {
				return m_listElement;
			};
			/**
			 * @brief get the nearest Element
			 * @param[in] _sourceRequest Pointer on the element that request this.
			 * @param[in] _distance Maximum distance search  == > return the element distance
			 * @return Pointer on the neares element OR nullptr
			 */
			ememory::SharedPtr<ege::Element> getElementNearest(ememory::SharedPtr<ege::Element> _sourceRequest, float& _distance);
			
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
			void addElement(ememory::SharedPtr<ege::Element> _newElement);
			/**
			 * @brief remove an element on the list availlable.
			 * @param[in] _removeElement Element to remove.
			 */
			void rmElement(ememory::SharedPtr<ege::Element> _removeElement);
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
		protected:
			int64_t m_gameTime; //!< time of the game running
		public:
			esignal::Connection m_periodicCallConnection;
		private:
			void onCallbackPeriodicCall(const ewol::event::Time& _event);
		protected:
			std::vector<ememory::SharedPtr<ege::resource::Mesh>> m_listMeshToDrawFirst;
		public:
			void addStaticMeshToDraw(const ememory::SharedPtr<ege::resource::Mesh>& _mesh) {
				m_listMeshToDrawFirst.push_back(_mesh);
			}
			std::vector<ememory::SharedPtr<ege::resource::Mesh>>& getStaticMeshToDraw() {
				return m_listMeshToDrawFirst;
			}
			virtual void onChangePropertyStatus();
	};
}


