/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_ELEMENT_GAME_IA_H__
#define __EGE_ELEMENT_GAME_IA_H__

#include <ege/ElementGame.h>

namespace ege
{
	class ElementGameIA : public ege::ElementGame
	{
		private:
			class localIA : public btActionInterface
			{
				private:
					ElementGameIA& m_element;
				public:
					/**
					 * @brief Constructor
					 */
					localIA(ElementGameIA& element) : m_element(element) { };
					/**
					 * @brief Destructor
					 */
					virtual ~localIA(void) { };
					
					// herited function
					void debugDraw(btIDebugDraw* debugDrawer) { };
					// herited function
					void updateAction(btCollisionWorld* collisionWorld, btScalar step)
					{
						m_element.IAAction(step);
					}
			};
			localIA m_IA;
		public:
			/**
			 * @brief Constructor
			 */
			ElementGameIA(ege::Environement& _env);
			/**
			 * @brief Destructor
			 */
			virtual ~ElementGameIA(void);
			/**
			 * @brief Periodic call for intelligence artificial.
			 * @param[in] step : step of time in s
			 */
			virtual void IAAction(float _step)=0;
			
		private:
			bool m_isDynamicStarted;
		public:
			virtual void DynamicEnable(void);
			virtual void DynamicDisable(void);
	};
};

#endif


