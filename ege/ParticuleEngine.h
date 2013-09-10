/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_PARTICULE_ENGINE_H__
#define __EGE_PARTICULE_ENGINE_H__

#include <etk/UString.h>
#include <ege/Particule.h>
#include <ege/Environement.h>

namespace ege {
	class PariculeEngine
	{
		public:
			PariculeEngine(ege::Environement& _env); // note : Need the engine to register has an dynamic element ... (the first ...)
			~PariculeEngine(void);
			Particule* LocalKeep(const etk::UString& _name);
			void LocalRelease(Particule* _particule);
			void Update(float _deltaTime);
			void Draw(void);
	};
};

#endif


