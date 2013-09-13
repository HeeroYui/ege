/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/Environement.h>

#ifndef __EGE_PARTICULE_ENGINE_H__
#define __EGE_PARTICULE_ENGINE_H__

namespace ege {
	class ParticuleEngine;
};

#include <etk/UString.h>
#include <etk/Vector.h>
#include <ege/Particule.h>

namespace ege {
	class ParticuleEngine
	{
		private:
			ege::Environement& m_env;
		public:
			ParticuleEngine(ege::Environement& _env); // note : Need the engine to register has an dynamic element ... (the first ...)
			~ParticuleEngine(void);
		private:
			etk::Vector<Particule*> m_particuleList; //!< all particule created and active
			etk::Vector<Particule*> m_particuleRemoved; //!< removed particule
		public:
			/**
			 * @brief Clear the particule engine
			 */
			void Clear(void);
			/**
			 * @brief Add a particule in the engine (internal acces only)
			 * @param[in] _particule Pointer on the particule to add
			 */
			void Add(Particule* _particule);
		private:
			/**
			 * @brief Add a particule in the removed section ==> this not delete the particule, but just set it in an other list
			 * @param[in] _particule Pointer on the particule to add
			 */
			void AddRemoved(Particule* _particule);
		public:
			/**
			 * @brief Update particule properties
			 * @param[in] _deltaTime delta time to process
			 */
			void Update(float _deltaTime);
			/**
			 * @brief Draw all the active Particule
			 * @param[in] _camera Reference on the current camera
			 */
			void Draw(const ege::Camera& _camera);
			/**
			 * @brief Get a particue with his type, we get particule that has been already removed, otherwise, you will create new
			 * @param[in] _particuleType Particule type, this chek only the pointer not the data.
			 * @return NULL, the particule has not been removed from the created pool
			 * @return The pointer on the requested element (an Init has been done).
			 * @note If you did not want to use respawn set type at NULL.
			 */
			Particule* Respown(const char* _particuleType);
			
	};
};

#endif


