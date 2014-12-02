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
				View(const vec3& _eye=vec3(0,0,0), const vec3& _target=vec3(0,0,1), float _angle=0.0f);
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
				float m_angle; //!< rotation angle of the camera (in rad) through the axis origin->eye
			public:
				/**
				 * @brief Set the camera angle.
				 * @param[in] _angle camera angle.
				 */
				void setAngle(float _angle);
				/**
				 * @brief Get the camera angle.
				 * @return the up camera angle.
				 */
				float getAngle() const {
					return m_angle;
				};
			protected:
				virtual vec3 getViewVector() const;
			public:
				virtual ege::Ray getRayFromScreen(const vec2& _offset);
				virtual void drawDebug(const std::shared_ptr<ewol::resource::Colored3DObject>& _draw, const std::shared_ptr<ege::Camera>& _camera);
				virtual float getTetha();
				virtual float getPsy();
		};
	};
};

#endif

