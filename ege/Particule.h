/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_PARTICULE_H__
#define __EGE_PARTICULE_H__

namespace ege {
	class Particule;
};

#include <etk/UString.h>
#include <ege/Environement.h>


namespace ege {
	/**
	 * @brief The particule class is an element with no control, when it will be created,
	 *        it does not have any control, for example smoke or reactor generation ...
	 *        or explosion particule ...
	 */
	class Particule
	{
		private:
			ege::ParticuleEngine& m_particuleEngine;
		public:
			/**
			 * @brief Constructor.
			 * @param[in] _env reference on the envorionement ...
			 */
			Particule(ege::ParticuleEngine& _particuleEngine, const char* _particuleType);
			/**
			 * @brief Destructor.
			 */
			virtual ~Particule(void) { };
			/**
			 * @brief Init the particule
			 */
			virtual void Init(void) { };
			/**
			 * @brief Un-init the particule
			 */
			virtual void UnInit(void) { };
			/**
			 * @brief Update the paticule properties
			 * @param[in] _delta Delta time from the previous call
			 */
			virtual void Update(float _delta) { };
			/**
			 * @brief Draw the current particule
			 */
			virtual void Draw(void) { };
			/**
			 * @brief Check if the element might be removed
			 * @return true : The element might be removed
			 * @return false : The element might be keeped
			 */
			virtual bool NeedRemove(void) { return false; };
	};
};

#endif


