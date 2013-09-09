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
	class Particule
	{
		private:
			etk::UString m_name; //!< name of the particule
			bool m_standalone; //!< if true, no control of the particule...
		public:
			/**
			 * @brief Constructor.
			 * @param[in] _name Name of the particule.
			 * @param[in] _standalone The particule are created and have there own life (no dynamic control)
			 */
			Particule(const etk::UString& _name, bool _standalone) : m_name(_name) { };
			/**
			 * @brief Destructor.
			 */
			~Particule(void) { };
			/**
			 * @brief Get the particule type name.
			 * @return the particule name.
			 */
			const etk::UString& GetName(void) { return m_name; };
			/**
			 * @brief Get the standalone status.
			 * @return true if standalone
			 */
			bool GetStandalone(void) { return m_standalone; };
			/**
			 * @brief Update the paticule properties
			 * @param[in] _delta Delta time from the previous call
			 */
			virtual void Update(float _delta) { };
		// note : For multiple instance only (standalone==false)
			/**
			 *
			 */
			/*
			virtual int32_t Add(void) { return -1; };
			virtual void SetLife(int32_t _id, float _life);
			virtual void SetLevel(int32_t _id, float _level);
			virtual void SetPosition(int32_t _id, const vec3& _pos);
			virtual void SetAngleSpeed(int32_t _id, const vec4& _angle);
			virtual void SetMoveSpeed(int32_t _id, const vec3& _speed);
			*/
	};
};

#endif


