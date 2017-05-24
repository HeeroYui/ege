/** @file
 * @author Edouard DUPIN
 * @copyright 2013, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ege/camera/View.hpp>

#include <ewol/widget/Widget.hpp>
#include <ewol/event/Entry.hpp>
#include <ewol/event/Input.hpp>

namespace ege {
	namespace camera {
		/**
		 * The control position of the camera is principali a basic camera control to prototype an idea ...
		 */
		class ControlBase {
			protected:
				vec3 m_destinationCameraOffset;
				float m_angleTetha;
				float m_anglePsy;
				float m_distance;
				vec2 m_oldScreenPos;
				esignal::Connection m_PCH; //!< Periodic Call Handle to remove it when needed
			public:
				/**
				 * @brief Constructor.
				 */
				ControlBase();
			private:
				ememory::SharedPtr<ege::camera::View> m_camera;
			public:
				void setCamera(const ememory::SharedPtr<ege::camera::View>& _camera);
				bool onEventEntry(const ewol::event::Entry& _event);
				bool onEventInput(const ewol::event::Input& _event, const vec2& _relativePosition);
				/**
				 * @brief Periodic call to update grapgic display
				 * @param[in] _event Time generic event
				 */
				void periodicCall(const ewol::event::Time& _event);
		};
	}
}

