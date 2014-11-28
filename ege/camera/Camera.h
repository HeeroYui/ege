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
#include <ege/Ray.h>


namespace ege {
	class Camera {
		public:
			/**
			 * @brief Constructor.
			 */
			Camera();
			/**
			 * @brief Destructor.
			 */
			virtual ~Camera() {};
		public:
			/**
			 * @brief It is really needed to call the camera periodicly for performing automatic movement
			 * @param[in] _step step time of moving
			 */
			virtual void periodicCall(float _step) {};
		public:
			/**
			 * @brief Configure projection matrix and camera matrix
			 */
			virtual void configureOpenGL();
		protected:
			mat4 m_matrix; //!< transformation matrix.
		public:
			/**
			 * @brief get the transformation matix for the camera.
			 * @return the current transformation matrix
			 */
			const mat4& getMatrixCamera() const {
				return m_matrix;
			};
		protected:
			mat4 m_matrixProjection; //!< projection matrix.
			/**
			 * @bref Update the current projection matrix.
			 */
			virtual void updateProjectionMatrix();
		public:
			/**
			 * @brief get the transformation matix for the camera.
			 * @return the current transformation matrix
			 */
			const mat4& getMatrixProjection() const {
				return m_matrixProjection;
			};
		public:
			virtual vec3 getViewVector() const {
				return vec3(0,0,-1);
			}
			virtual vec3 getEye() const {
				return vec3(0,0,0);
			}
		protected:
			float m_aspectRatio; //!< depending to the display the aspect ratio is simply calculated x/y
		public:
			/**
			 * @brief Set the aspectRatio of the camera:
			 * @param[in] _ratio New aspect ratio.
			 */
			virtual void setAspectRatio(float _ratio);
			/**
			 * @brief Set the screen size to display OpenGl interface
			 * @param[in] _screenSize New screen size.
			 */
			virtual void setSceenSize(const vec2& _screenSize) {
				setAspectRatio(_screenSize.x()/_screenSize.y());
			}
			/**
			 * @brief get the current aspect Ratio.
			 * @return The current aspect ratio.
			 */
			float getAspectRatio() const {
				return m_aspectRatio;
			}
		protected:
			float m_angleView; //!< X angle view of the camera
		public:
			/**
			 * @brief Set the the X angle view of the camera:
			 * @param[in] _angleRad New angle view in X of the camera.
			 */
			virtual void setXAngleView(float _angleRad);
			/**
			 * @brief Set the the Y angle view of the camera:
			 * @param[in] _angleRad New angle view in Y of the camera.
			 * @note Pay attention that the AspectRatio parameter is set before
			 */
			virtual void setYAngleView(float _angleRad) {
				setXAngleView(_angleRad * m_aspectRatio);
			}
			/**
			 * @brief get the current X angle view.
			 * @return the current X angle view.
			 */
			float getXAngleView() const {
				return m_angleView;
			}
		protected:
			float m_zFar; //!< Z camera far view
		public:
			/**
			 * @brief Set the Z far distane of the camera:
			 * @param[in] _distance New end distance view.
			 */
			virtual void setZFar(float _distance);
			/**
			 * @brief Get the Z far distane of the camera:
			 * @return the current end distance view.
			 */
			float getZFar() {
				return m_zFar;
			}
		protected:
			float m_zNear; //!< Z camera near view
		public:
			/**
			 * @brief Set the Z near distane of the camera:
			 * @param[in] _distance New start distance view.
			 */
			virtual void setZNear(float _distance);
			/**
			 * @brief Get the Z Near distane of the camera:
			 * @return the current start distance view.
			 */
			float getZNear() {
				return m_zNear;
			}
		public:
			/**
			 * Get ray from the camera with the screen offset.
			 * @param[in] _offset Offset in the screen [-1..1]
			 * @return the ray requested.
			 */
			virtual ege::Ray getRayFromScreen(const vec2& _offset);
			/**
			 * Get ray from the camera with the screen offset.
			 * @param[in] _position positin the screen
			 * @param[in] _size size of the screen
			 * @return the ray requested.
			 */
			ege::Ray getRayFromScreenPosition(const vec2& _position, const vec2& _size);
		protected:
			/**
			 * @brief get Tetha and psy angle from a position:
			 * @param[in] _vect Vector to transform in the euler angle
			 * @return x: tetha; y: psy
			 */
			vec2 tansformPositionToAngle(vec3 _vect);

	};
};

#endif

