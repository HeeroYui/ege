/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ewol/widget/Widget.h>
#include <ege/resource/Mesh.h>

namespace ege {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Mesh :public ewol::Widget {
			public:
				esignal::Signal<void> signalPressed;
			private:
				// mesh name :
				std::string m_meshName;
				std::shared_ptr<ege::resource::Mesh> m_object;
				// mesh display properties:
				vec3 m_position;
				vec3 m_angle;
				vec3 m_angleSpeed;
				float m_cameraDistance;
			protected:
				Mesh();
				void init(const std::string& _filename); // automatic considering in the appl Data older
			public:
				virtual ~Mesh();
			public: // Derived function
				virtual void onRegenerateDisplay();
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual void onDraw();
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual void periodicCall(const ewol::event::Time& _event);
			public:
				/**
				 * @brief set a mesh name file
				 * @param[in] _filename Name of the new mesh
				 */
				void setFile(const std::string& _filename);
				/**
				 * @brief set the mesh position
				 * @param[in] _pos The new position of the mesh
				 */
				void setPosition(const vec3& _pos);
				/**
				 * @brief set the mesh angle of view
				 * @param[in] _angle view angle of the mesh
				 */
				void setAngle(const vec3& _angle);
				/**
				 * @brief set the mesh angle speed
				 * @param[in] _speed Radian speed of the mesh
				 */
				void setAngleSpeed(const vec3& _speed);
				/**
				 * @brief set the camera distance of the mesh
				 * @param[in] _distance Diatance of the mesh
				 */
				void setDistance(float _distance);
		};
	}
}