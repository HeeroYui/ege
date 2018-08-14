/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/ia/Engine.hpp>
#include <ege/debug.hpp>

ege::ia::Engine::Engine(ege::Environement* _env) :
  ege::Engine(_env) {
	
}

const etk::String& ege::ia::Engine::getType() const {
	static etk::String tmp("ia");
	return tmp;
}

void ege::ia::Engine::componentRemove(const ememory::SharedPtr<ege::Component>& _ref) {
	ememory::SharedPtr<ege::ia::Component> ref = ememory::dynamicPointerCast<ege::ia::Component>(_ref);
	for (auto it=m_component.begin();
	     it != m_component.end();
	     ++it) {
		if (*it == ref) {
			it->reset();
			return;
		}
	}
}

void ege::ia::Engine::componentAdd(const ememory::SharedPtr<ege::Component>& _ref) {
	ememory::SharedPtr<ege::ia::Component> ref = ememory::dynamicPointerCast<ege::ia::Component>(_ref);
	EGE_WARNING("ADD COMPONENT " << uint64_t(ref.get()) );
	if (ref == null) {
		return;
	}
	#if DEBUG
		for (auto it=m_component.begin();
		     it != m_component.end();
		     ++it) {
			if (*it != null) {
				if (*it == ref) {
					EGE_ERROR("Try Add multiple time the same Component in the IA Engine " << uint64_t(ref.get()) );
				}
			}
		}
	#endif
	for (auto it=m_component.begin();
	     it != m_component.end();
	     ++it) {
		if (*it == null) {
			*it = ref;
			return;
		}
	}
	m_component.pushBack(ref);
}

// Constant physics time step ==> 10 time / seconds
// TODO: set it configurable, some games need more, and soem other just need really less ==> or set it configurable with the internal component
static const float timeStep = 5.0;

void ege::ia::Engine::update(const echrono::Duration& _delta) {
	float deltaTime = _delta.toSeconds();
	// Add the time difference in the accumulator
	m_accumulator += deltaTime;
	// While there is enough accumulated time to take one or several physics steps
	while (m_accumulator >= timeStep) {
		EGE_WARNING("Generate for " << m_accumulator << " / " << timeStep << "  for:" << m_component.size());
		// call every object to usdate their constant forces applyed
		for (auto &it: m_component) {
			// check null  pointer
			if (it == null) {
				// no pointer null are set in the output list ...
				continue;
			}
			it->update(timeStep);
		}
		// Decrease the accumulated time
		m_accumulator -= timeStep;
	}
}
