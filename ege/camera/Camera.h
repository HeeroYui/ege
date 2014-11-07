/**
 * @author Edouard DUPIN
 * 
 * @copyright 2013, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_CAMERA_H__
#define __EGE_CAMERA_H__


#include <etk/types.h>
#include <etk/math/Vector3D.h>
#include <etk/math/Vector2D.h>
#include <etk/math/Matrix4.h>

namespace ege {
	class Camera {
		protected:
			mat4 m_matrix; //!< transformation matrix.
		public:
			/**
			 * @brief Constructor.
			 */
			Camera();
			/**
			 * @brief Destructor.
			 */
			virtual ~Camera() {};
			/**
			 * @brief get the transformation matix for the camera.
			 * @return the current transformation matrix
			 */
			const mat4& getMatrix() const {
				return m_matrix;
			};
			/**
			 * @brief It is really needed to call the camera periodicly for performing automatic movement
			 * @param[in] _step step time of moving
			 */
			virtual void periodicCall(float _step) {};
			
			virtual vec3 getViewVector() const {
				return vec3(0,0,-1);
			}
			virtual vec3 getEye() const {
				return vec3(0,0,0);
			}
	};
};

#endif

