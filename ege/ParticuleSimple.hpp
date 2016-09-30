/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

namespace ege {
	class ParticuleSimple;
};

#include <etk/types.hpp>
#include <etk/math/Vector2D.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Vector4D.hpp>
#include <ege/Environement.hpp>
#include <ege/Particule.hpp>


namespace ege {
	/**
	 * @brief The particule class is an element with no control, when it will be created,
	 *        it does not have any control, for example smoke or reactor generation ...
	 *        or explosion particule ...
	 */
	class ParticuleSimple : public Particule {
		public:
			/**
			 * @brief Constructor.
			 * @param[in] _name Name of the particule.
			 * @param[in] _standalone The particule are created and have there own life (no dynamic control)
			 */
			ParticuleSimple(ege::ParticuleEngine* _particuleEngine, const char* _particuleType);
			/**
			 * @brief Destructor.
			 */
			virtual ~ParticuleSimple() { };
		public: // herited elements:
			virtual void update(float _delta);
			//virtual void draw() { };
			virtual bool needRemove();
			virtual void init();
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
			virtual void setLife(float _life);
			virtual void setLevel(float _level);
			virtual void setPosition(const vec3& _pos);
			virtual void setAngle(float _angle);
			virtual void setMoveSpeed(const vec3& _speed);
			virtual void setScale(const vec3& _scale);
			virtual void setScaleExpend(const vec3& _scaleExpand);
	};
}


