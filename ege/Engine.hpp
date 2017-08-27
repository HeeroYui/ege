/** @file
 * @author Edouard DUPIN
 * @copyright 2017, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <ememory/memory.hpp>

#include <ege/Component.hpp>
#include <echrono/Duration.hpp>
#include <ege/camera/Camera.hpp>
#include <eproperty/Interface.hpp>

namespace ege {
	class Environement;
	class Camera;
	class Engine :
	  public ememory::EnableSharedFromThis<Engine>,
	  public eproperty::Interface {
		protected:
			ege::Environement* m_env;
		public:
			Engine(ege::Environement* _env);
			virtual ~Engine() = default;
		public:
			/**
			 * @brief get the type of the engine
			 * @return the type in string
			 */
			virtual const etk::String& getType() const;
			/**
			 * @brief An ege::Entity component has been removed ==> need remove it in local if needed
			 * @param[in] _ref Referrence on the component
			 */
			virtual void componentRemove(const ememory::SharedPtr<ege::Component>& _ref);
			/**
			 * @brief An ege::Entity component has been added ==> need add it in local if needed
			 * @param[in] _ref Referrence on the component
			 */
			virtual void componentAdd(const ememory::SharedPtr<ege::Component>& _ref);
			/**
			 * @brief Global game engine main cycle of update internal parameters
			 * @param[in] _delta time from the last update
			 */
			virtual void update(const echrono::Duration& _delta);
			/**
			 * @brief Globalgame engine main cycle of draw
			 * @param[in] _delta time from the last render
			 * @param[in] _camera Camera property to render the engine properties ...
			 */
			virtual void render(const echrono::Duration& _delta, const ememory::SharedPtr<ege::Camera>& _camera);
			/**
			 * @brief Globalgame engine main cycle of draw
			 * @param[in] _delta time from the last render
			 * @param[in] _camera Camera property to render the engine properties ...
			 */
			virtual void renderDebug(const echrono::Duration& _delta, const ememory::SharedPtr<ege::Camera>& _camera);
	};
}

