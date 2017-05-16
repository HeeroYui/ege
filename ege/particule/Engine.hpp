/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once
namespace ege {
	class Environement;
	class Particule;
};
#include <etk/types.hpp>
#include <vector>
#include <ege/camera/Camera.hpp>

namespace ege {
	class ParticuleEngine {
		private:
			ege::Environement* m_env;
		public:
			ParticuleEngine(ege::Environement* _env); // note : need the engine to register has an dynamic element ... (the first ...)
			~ParticuleEngine();
		private:
			std::vector<Particule*> m_particuleList; //!< all particule created and active
			std::vector<Particule*> m_particuleRemoved; //!< removed particule
		public:
			/**
			 * @brief clear the particule engine
			 */
			void clear();
			/**
			 * @brief add a particule in the engine (internal acces only)
			 * @param[in] _particule Pointer on the particule to add
			 */
			void add(Particule* _particule);
		private:
			/**
			 * @brief add a particule in the removed section  == > this not delete the particule, but just set it in an other list
			 * @param[in] _particule Pointer on the particule to add
			 */
			void addRemoved(Particule* _particule);
		public:
			/**
			 * @brief update particule properties
			 * @param[in] _deltaTime delta time to process
			 */
			void update(float _deltaTime);
			/**
			 * @brief draw all the active Particule
			 * @param[in] _camera Reference on the current camera
			 */
			void draw(const ege::Camera& _camera);
			/**
			 * @brief get a particue with his type, we get particule that has been already removed, otherwise, you will create new
			 * @param[in] _particuleType Particule type, this chek only the pointer not the data.
			 * @return nullptr, the particule has not been removed from the created pool
			 * @return The pointer on the requested element (an init has been done).
			 * @note If you did not want to use respawn set type at nullptr.
			 */
			Particule* respown(const char* _particuleType);
			
	};
}


