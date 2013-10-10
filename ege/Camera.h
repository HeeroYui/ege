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

namespace ege
{
	class Camera
	{
		protected:
			mat4 m_matrix; //!< transformation matrix.
			float m_offsetFactor; //!< this variable is used to move the camera to the top position of the system  == > automaticly
			/**
			 * @brief update the matrix property
			 */
			void update(void);
		public:
			/**
			 * @brief Constructor.
			 * @param[in] _eye Position of the camera destination view.
			 * @param[in] _angleZ Z rotation angle.
			 * @param[in] _angleTeta Angle of the camera inclinason.
			 * @param[in] _distance distance to the eye point
			 */
			Camera(const vec3& _eye=vec3(0,0,0), float _angleZ=0, float _angleTeta=0, float _distance=10);
		protected:
			vec3 m_eye; //!< position where the camera see
		public:
			/**
			 * @brief set the position of the camera.
			 * @param[in] pos Position of the camera.
			 */
			void setEye(const vec3& _eye);
			/**
			 * @brief get the curent Camera Eye position.
			 * @return the current position.
			 */
			const vec3& getEye(void) const {
				return m_eye;
			};
		protected:
			vec3 m_calculatedOrigin;
		public:
			/**
			 * @brief get the curent Camera origin position.
			 * @return the current position.
			 */
			const vec3& getOrigin(void) const {
				return m_calculatedOrigin;
			};
		protected:
			vec3 m_calculatedViewVector;
		public:
			/**
			 * @brief Get the camera viewing vector.
			 * @return the current position.
			 */
			const vec3& getViewVector(void) const {
				return m_calculatedViewVector;
			};
		protected:
			float m_angleZ;
		public:
			/**
			 * @brief set the angle on the camera
			 * @param[in] _angleZ Rotation angle in Z
			 */
			void setAngleZ(float _angleZ);
			/**
			 * @brief get the curent Camera angles.
			 * @return the current angles Z.
			 */
			float getAngleZ(void) const {
				return m_angleZ;
			};
		protected:
			float m_angleTeta;
		public:
			/**
			 * @brief set the angle on the camera
			 * @param[in] _angleTeta Rotation angle in Teta
			 */
			void setAngleTeta(float _angleTeta);
			/**
			 * @brief get the curent Camera angles.
			 * @return the current angles Teta.
			 */
			float getAngleTeta(void) const {
				return m_angleTeta;
			};
		protected:
			float m_distance;
		public:
			/**
			 * @brief set the angle on the camera
			 * @param[in] _distance Distance to the eye
			 */
			void setDistance(float _distance);
			/**
			 * @brief get the curent Camera angles.
			 * @return the current distance to the eye.
			 */
			float getDistance(void) const {
				return m_distance;
			};
			/**
			 * @brief get the transformation matix for the camera.
			 * @return the current transformation matrix
			 */
			const mat4& getMatrix(void) const {
				return m_matrix;
			};
		protected:
			bool m_forceViewTop;
		public:
			/**
			 * @brief change camera mode of view  == > force to the top view
			 * @param[in] _newState The new state of this mode...
			 */
			void setForcingViewTop(bool _newState) {
				m_forceViewTop = _newState;
			};
			/**
			 * @brief It is really needed to call the camera periodicly for performing automatic movement
			 * @param[in] _step step time of moving
			 */
			void periodicCall(float _step);
			vec3 projectOnZGround(const vec2& _cameraDeltaAngle, float _zValue=0.0f);
			
	};
};

#endif

