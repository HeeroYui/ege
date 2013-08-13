/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_ENVIRONEMENT_H__
#define __EGE_ENVIRONEMENT_H__

#include <BulletDynamics/Dynamics/btActionInterface.h>
class btDynamicsWorld;

#include <etk/Vector.h>
#include <etk/math/Vector3D.h>

namespace ege {
	class ElementGame;
	class Environement
	{
		private:
			btDynamicsWorld* m_dynamicsWorld; //!< curent system world description
			etk::Vector<ege::ElementGame*> m_listElementGame; //!< List of all element added in the Game
		public:
			Environement(void) : m_dynamicsWorld(NULL) { };
			virtual ~Environement(void) { };
			
		public:
			class ResultNearestElement
			{
				public:
					ege::ElementGame* element;
					float dist;
			};
			/**
			 * @brief Set the curent world
			 * @param[in] _newWorld Pointer on the current world
			 */
			void SetDynamicWorld(btDynamicsWorld* _newWorld) { m_dynamicsWorld=_newWorld; };
			/**
			 * @brief Get the curent world
			 * @return pointer on the current world
			 */
			btDynamicsWorld* GetDynamicWorld(void) { return m_dynamicsWorld; };
			/**
			 * @breif Get a reference on the curent list of element games
			 * @return all element list
			 */
			etk::Vector<ege::ElementGame*>& GetElementGame(void) { return m_listElementGame; };
			/**
			 * @brief Get the nearest Element
			 * @param[in] _sourceRequest Pointer on the element that request this.
			 * @param[in] _distance Maximum distance search ==> return the element distance
			 * @return Pointer on the neares element OR NULL
			 */
			ege::ElementGame* GetElementNearest(ege::ElementGame* _sourceRequest, float& _distance);
			
			void GetElementNearest(const vec3& _sourcePosition, float _distanceMax, etk::Vector<ege::Environement::ResultNearestElement>& _resultList);
			void GetElementNearestFixed(const vec3& _sourcePosition, float _distanceMax, etk::Vector<ege::Environement::ResultNearestElement>& _resultList);
			/**
			 * @brief Add an element on the list availlable.
			 * @param[in] _newElement Element to add.
			 */
			void AddElementGame(ege::ElementGame* _newElement);
			/**
			 * @brief Remove an element on the list availlable.
			 * @param[in] _removeElement Element to remove.
			 */
			void RmElementGame(ege::ElementGame* _removeElement);
			/**
			 * @brief Get the element order from the nearest to the farest, and remove all element that are not in the camera angle and axes.
			 * @param[in,out] _resultList List of the element ordered.
			 * @param[in] _position Camera position in the space.
			 * @param[in] _direction Camera direction of the view.
			 */
			void GetOrderedElementForDisplay(etk::Vector<ege::Environement::ResultNearestElement>& _resultList, const vec3& _position, const vec3& _direction);
	};
};

#endif


