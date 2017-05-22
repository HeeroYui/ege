/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

namespace ege {
	class Environement;
	class EntityInteraction;
};
#include <ege/camera/Camera.hpp>

#include <ege/Engine.hpp>


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

namespace ege {
	class Entity;
	class Environement;
	typedef ememory::SharedPtr<ege::Entity> (*createEntity_tf)(const ememory::SharedPtr<ege::Environement>& _env);
	
	enum gameStatus {
		gameStart,
		gamePause,
		gameStop
	};
	
	class EntityInteraction {
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
			EntityInteraction(int32_t _type, int32_t _groupSource, const vec3& _pos) : 
				m_type(_type),
				m_groupSource(_groupSource),
				m_positionSource(_pos)
			{ };
		public:
			virtual void applyEvent(ege::Entity& _entity) { };
	};
	// TODO : An entity must be created by a local factory...
	class Environement : public ewol::Object {
		public:
			// Signals
			esignal::Signal<float> signalPlayTimeChange;
			// properties:
			eproperty::List<enum gameStatus> propertyStatus; //!< the display is running (not in pause)
			eproperty::Value<float> propertyRatio; //!< Speed ratio
		protected:
			std::vector<ememory::SharedPtr<ege::Engine>> m_engine; //!< EGE sub engine interface (like physique, rendering, audio, ...).
		public:
			void addEngine(const ememory::SharedPtr<ege::Engine>& _ref);
			void rmEngine(const ememory::SharedPtr<ege::Engine>& _ref);
			void rmEngine(const std::string& _type);
			ememory::SharedPtr<ege::Engine> getEngine(const std::string& _type);
			void engineComponentRemove(const ememory::SharedPtr<ege::Component>& _ref);
			void engineComponentAdd(const ememory::SharedPtr<ege::Component>& _ref);
			
		private:
			std::vector<ememory::SharedPtr<ege::Entity>> m_listEntity; //!< List of all entity added in the Game
		protected:
			Environement();
		public:
			DECLARE_FACTORY(Environement);
			virtual ~Environement() { };
		public:
			void render(const echrono::Duration& _delta, const std::string& _camera);
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
			 * @brief add a creator entity system
			 * @param[in] _type Type of the entity.
			 * @param[in] _creator Function pointer that reference the entity creating.
			 */
			static void addCreator(const std::string& _type, ege::createEntity_tf _creator);
			/**
			 * @brief Create an entity on the curent scene.
			 * @param[in] _type Type of the entity that might be created.
			 * @param[in] _description String that describe the content of the entity properties.
			 * @param[in] _autoAddEntity this permit to add the entity if it is created  == > no more action ...
			 * @return nullptr if an error occured OR the pointer on the entity and it is already added on the system.
			 * @note Pointer is return in case of setting properties on it...
			 */
			ememory::SharedPtr<ege::Entity> createEntity(const std::string& _type, const std::string& _description, bool _autoAddEntity=true);
			ememory::SharedPtr<ege::Entity> createEntity(const std::string& _type, const ejson::Value& _value, bool _autoAddEntity=true);
			ememory::SharedPtr<ege::Entity> createEntity(const std::string& _type, const exml::Node& _node, bool _autoAddEntity=true);
			ememory::SharedPtr<ege::Entity> createEntity(const std::string& _type, void* _data, bool _autoAddEntity=true);
			ememory::SharedPtr<ege::Entity> createEntity(const std::string& _type, bool _autoAddEntity=true);
		public:
			class ResultNearestEntity {
				public:
					ememory::SharedPtr<ege::Entity> entity;
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
			 * @breif get a reference on the curent list of entity games
			 * @return all entity list
			 */
			std::vector<ememory::SharedPtr<ege::Entity>>& getEntity() {
				return m_listEntity;
			};
			/**
			 * @brief get the nearest Entity
			 * @param[in] _sourceRequest Pointer on the entity that request this.
			 * @param[in] _distance Maximum distance search  == > return the entity distance
			 * @return Pointer on the neares entity OR nullptr
			 */
			/*
			ememory::SharedPtr<ege::Entity> getEntityNearest(ememory::SharedPtr<ege::Entity> _sourceRequest, float& _distance);
			
			void getEntityNearest(const vec3& _sourcePosition,
			                       float _distanceMax,
			                       std::vector<ege::Environement::ResultNearestEntity>& _resultList);
			void getEntityNearestFixed(const vec3& _sourcePosition,
			                            float _distanceMax,
			                            std::vector<ege::Environement::ResultNearestEntity>& _resultList);
			*/
			/**
			 * @brief add an entity on the list availlable.
			 * @param[in] _newEntity Entity to add.
			 */
			void addEntity(ememory::SharedPtr<ege::Entity> _newEntity);
			/**
			 * @brief remove an entity on the list availlable.
			 * @param[in] _removeEntity Entity to remove.
			 */
			void rmEntity(ememory::SharedPtr<ege::Entity> _removeEntity);
			/**
			 * @brief generate an event on all the sub entity of the game  == > usefull for explosion, or lazer fire ...
			 * @param[in] _event event that might be apply ...
			 */
			void generateInteraction(ege::EntityInteraction& _event);
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


