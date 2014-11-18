/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_RAY_H__
#define __EGE_RAY_H__

#include <etk/math/Vector3D.h>

namespace ege {
	class Ray {
		public:
			/**
			 * @brief Contructor
			 * @param _origin The ray's origin.
			 * @param _direction The ray's direction.
			 */
			Ray(const vec3& _origin=vec3(0,0,0), const vec3& _direction=vec3(0,0,1));
			/**
			 * @brief Destructor.
			 */
			~Ray() {};
		private:
			vec3 m_origin; //!< The ray origin position.
		public:
			/**
			 * @brief Set the ray's origin.
			 * @param[in] _origin The new origin.
			 */
			void setOrigin(const vec3& _origin);
			/**
			 * @brief Gets the ray's origin.
			 * @return The ray's origin.
			 */
			const vec3& getOrigin() const {
				return m_origin;
			}
		private:
			vec3 m_direction; //!< The ray direction vector.
		public:
			/**
			 * @brief Sets the ray's direction.
			 * @param[in] _direction The new direction vector.
			 */
			void setDirection(const vec3& _direction);
			/**
			 * @brief Gets the ray's direction.
			 * @return The ray's direction.
			 */
			const vec3& getDirection() const {
				return m_direction;
			}
		public:
			/**
			 * @brief Sets this ray to the specified values.
			 * @param[in] _origin The ray's origin.
			 * @param[in] _direction The ray's direction.
			 */
			void set(const vec3& _origin, const vec3& _direction);
	};
	std::ostream& operator <<(std::ostream& _os, const ege::Ray& _obj);
};

#endif
