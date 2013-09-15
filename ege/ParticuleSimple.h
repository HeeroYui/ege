/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_PARTICULE_SIMPLE_H__
#define __EGE_PARTICULE_SIMPLE_H__

namespace ege {
	class ParticuleSimple;
};

#include <etk/UString.h>
#include <etk/math/Vector2D.h>
#include <etk/math/Vector3D.h>
#include <etk/math/Vector4D.h>
#include <ege/Environement.h>
#include <ege/Particule.h>


namespace ege {
	/**
	 * @brief The particule class is an element with no control, when it will be created,
	 *        it does not have any control, for example smoke or reactor generation ...
	 *        or explosion particule ...
	 */
	class ParticuleSimple : public Particule
	{
		public:
			/**
			 * @brief Constructor.
			 * @param[in] _name Name of the particule.
			 * @param[in] _standalone The particule are created and have there own life (no dynamic control)
			 */
			ParticuleSimple(ege::ParticuleEngine& _particuleEngine, const char* _particuleType);
			/**
			 * @brief Destructor.
			 */
			virtual ~ParticuleSimple(void) { };
		public: // herited elements:
			virtual void Update(float _delta);
			//virtual void Draw(void) { };
			virtual bool NeedRemove(void);
			virtual void Init(void);
		protected:
			float m_lifeFull;
			float m_life;
			float m_level;
			vec3 m_pos;
			float m_angle;
			vec3 m_speed;
			vec3 m_scale;
			vec3 m_scaleExpand;
		public:
			/**
			 *
			 */
			virtual void SetLife(float _life);
			virtual void SetLevel(float _level);
			virtual void SetPosition(const vec3& _pos);
			virtual void SetAngle(float _angle);
			virtual void SetMoveSpeed(const vec3& _speed);
			virtual void SetScale(const vec3& _scale);
			virtual void SetScaleExpend(const vec3& _scaleExpand);
	};
};

#endif


