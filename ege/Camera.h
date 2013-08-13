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
		private:
			vec3 m_eye; //!< position where the camera see
			float m_angleZ;
			float m_angleTeta;
			float m_distance;
			mat4 m_matrix; //!< transformation matrix.
			/**
			 * @brief Update the matrix property
			 */
			void Update(void);
			// these element is calculated when we get the mattrix:
			vec3 m_calculatedOrigin;
			vec3 m_calculatedViewVector;
			float m_offsetFactor; //!< this variable is used to move the camera to the top position of the system ==> automaticly
			bool m_forceViewTop;
		public:
			/**
			 * @brief Constructor.
			 * @param[in] _eye Position of the camera destination view.
			 * @param[in] _angleZ Z rotation angle.
			 * @param[in] _angleTeta Angle of the camera inclinason.
			 * @param[in] _distance distance to the eye point
			 */
			Camera(vec3 _eye=vec3(0,0,0), float _angleZ=0, float _angleTeta=0, float _distance=10);
			/**
			 * @brief Set the position of the camera.
			 * @param[in] pos Position of the camera.
			 */
			void SetEye(vec3 _eye);
			/**
			 * @brief Get the curent Camera Eye position.
			 * @return the current position.
			 */
			const vec3& GetEye(void) const { return m_eye; };
			/**
			 * @brief Get the curent Camera origin position.
			 * @return the current position.
			 */
			const vec3& GetOrigin(void) const { return m_calculatedOrigin; };
			const vec3& GetViewVector(void) const { return m_calculatedViewVector; };
			/**
			 * @brief Set the angle on the camera
			 * @param[in] _angleZ Rotation angle in Z
			 */
			void SetAngleZ(float _angleZ);
			/**
			 * @brief Get the curent Camera angles.
			 * @return the current angles Z.
			 */
			float GetAngleZ(void) const { return m_angleZ; };
			/**
			 * @brief Set the angle on the camera
			 * @param[in] _angleTeta Rotation angle in Teta
			 */
			void SetAngleTeta(float _angleTeta);
			/**
			 * @brief Get the curent Camera angles.
			 * @return the current angles Teta.
			 */
			float GetAngleTeta(void) const { return m_angleTeta; };
			/**
			 * @brief Set the angle on the camera
			 * @param[in] _distance Distance to the eye
			 */
			void SetDistance(float _distance);
			/**
			 * @brief Get the curent Camera angles.
			 * @return the current distance to the eye.
			 */
			float GetDistance(void) const { return m_distance; };
			/**
			 * @brief Get the transformation matix for the camera.
			 * @return the current transformation matrix
			 */
			const mat4& GetMatrix(void) const { return m_matrix; };
			
			
			vec3 projectOnZGround(const vec2& _cameraDeltaAngle, float _zValue=0.0f);
			
			/**
			 * @brief It is really needed to call the camera periodicly for performing automatic movement
			 * @param[in] step step time of moving
			 */
			void PeriodicCall(float step);
			/**
			 * @brief change camera mode of view ==> force to the top view
			 * @param[in] _newState The new state of this mode...
			 */
			void SetForcingViewTop(bool _newState) { m_forceViewTop = _newState; };
	};
};

#endif

