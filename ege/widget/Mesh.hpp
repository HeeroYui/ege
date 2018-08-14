/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ewol/widget/Widget.hpp>
#include <ege/resource/Mesh.hpp>

namespace ege {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class Mesh :public ewol::Widget {
			public:
				esignal::Signal<> signalPressed;
			private:
				// mesh name :
				etk::String m_meshName;
				ememory::SharedPtr<ege::resource::Mesh> m_object;
				// mesh display properties:
				vec3 m_position;
				vec3 m_angle;
				vec3 m_angleSpeed;
				float m_cameraDistance;
			protected:
				Mesh();
				void init() override; // automatic considering in the appl Data older
			public:
				virtual ~Mesh();
			public:
				void onRegenerateDisplay() override;
				void systemDraw(const ewol::DrawProperty& _displayProp) override;
				void onDraw() override;
				bool onEventInput(const ewol::event::Input& _event) override;
			protected:
				esignal::Connection m_PCH; //!< Periodic call handle to remove it when needed
				/**
				 * @brief Periodic call to update grapgic display
				 * @param[in] _event Time generic event
				 */
				void periodicCall(const ewol::event::Time& _event);
			public:
				/**
				 * @brief set a mesh name file
				 * @param[in] _filename Name of the new mesh
				 */
				void setFile(const etk::String& _filename);
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