/** @file
 * @author Edouard DUPIN
 * @copyright 2013, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <ege/camera/Camera.hpp>

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
				virtual void setEye(const vec3& _eye);
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
				virtual void setTarget(const vec3& _target);
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
				virtual void setAngle(float _angle);
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
				virtual void drawDebug(ememory::SharedPtr<ewol::resource::Colored3DObject> _draw, ememory::SharedPtr<ege::Camera> _camera);
				virtual float getTetha();
				virtual float getPsy();
		};
	}
}

