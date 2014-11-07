/**
 * @author Edouard DUPIN
 * 
 * @copyright 2013, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_CAMERA_VIEW_H__
#define __EGE_CAMERA_VIEW_H__

#include <ege/camera/Camera.h>

namespace ege {
	namespace camera {
		class View : public Camera {
			protected:
				/**
				 * @brief update the matrix property
				 */
				void update();
			public:
				/**
				 * @brief Constructor.
				 */
				View(const vec3& _eye=vec3(0,0,0), const vec3& _target=vec3(0,0,1), const vec3& _up=vec3(1,0,0));
				/**
				 * @brief Destructor.
				 */
				~View() {}
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
				virtual vec3 getEye() const {
					return m_eye;
				};
			protected:
				vec3 m_target; //!< origin of the camera
			public:
				/**
				 * @brief set a new Camera target position.
				 * @param[in] _target New camera target position.
				 */
				void setTarget(const vec3& _target);
				/**
				 * @brief Get the curent Camera target position.
				 * @return The target position.
				 */
				const vec3& getTarget() const {
					return m_target;
				};
			protected:
				vec3 m_up; //!< rotation angle of the camera (in rad) through the axis origin->eye
			public:
				/**
				 * @brief Set the up camera axis.
				 * @param[in] _up camera up axis.
				 */
				void setUp(const vec3& _up);
				/**
				 * @brief Get the up camera axis.
				 * @return the up camera axis.
				 */
				const vec3& getUp() const {
					return m_up;
				};
			protected:
				virtual vec3 getViewVector() const;
		};
	};
};

#endif

