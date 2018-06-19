/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ege/debug.hpp>

#include <ege/Component.hpp>

#include <ege/camera/Camera.hpp>

namespace ege {
	namespace particule {
		class Engine;
		class Component : public ege::Component {
			public:
				virtual const etk::String& getType() const override;
			protected:
				ege::particule::Engine* m_particuleEngine;
				const char* m_particuleType;
			public:
				/**
				 * @brief Constructor.
				 * @param[in] _particuleEngine reference on the particule engine ...
				 * @param[in] _particuleType Type of the particule (set null if you did not want to use the respowner ...)
				 */
				Component(ege::particule::Engine* _particuleEngine, const char* _particuleType = null);
				/**
				 * @brief Destructor.
				 */
				virtual ~Component() = default;
				/**
				 * @brief init the particule
				 */
				virtual void init() { };
				/**
				 * @brief Un-init the particule
				 */
				virtual void UnInit() { };
				/**
				 * @brief update the paticule properties
				 * @param[in] _delta Delta time from the previous call
				 */
				virtual void update(float _delta) { };
				/**
				 * @brief draw the current particule
				 */
				virtual void draw(const ege::Camera& _camera) { };
				/**
				 * @brief Check if the element might be removed
				 * @return true : The element might be removed
				 * @return false : The element might be keeped
				 */
				virtual bool needRemove() {
					return false;
				};
				/**
				 * @brief get the type of the particule
				 * @return Type of the current particule
				 */
				const char* getParticuleType() {
					return m_particuleType;
				};
				/**
				 * @brief When the particule arrive to his end of life, this function is called.
				 */
				virtual void onEnd() {};
		};
	}
}