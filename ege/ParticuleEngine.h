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
			etk::Vector<Particule*> m_particuleList;
		public:
			void Clear(void);
			void Add(Particule* _particule, const char* _particuleType);
			void Update(float _deltaTime);
			void Draw(void);
	};
};

#endif


