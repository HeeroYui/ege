/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_ELEMENT_PHYSIC_H__
#define __EGE_ELEMENT_PHYSIC_H__

#include <etk/types.h>
#include <etk/math/Vector3D.h>
#include <etk/math/Matrix4.h>
#include <vector>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/openGL/openGL.h>
#include <ewol/resource/Colored3DObject.h>
#include <ege/resource/Mesh.h>
#include <ege/camera/Camera.h>
#include <ewol/compositing/Text.h>
#include <ege/Environement.h>
#include <ege/elements/Element.h>

#include <LinearMath/btDefaultMotionState.h>

#define INDEX_RIGHT_AXIS   (0)
#define INDEX_FORWARD_AXIS (1)
#define INDEX_UP_AXIS      (2)

#define ELEMENT_SCALE     (1.0f/8.0f)

namespace ege {
	class ElementPhysic : public ege::Element {
		private:
			static void FunctionFreeShape(void* _pointer);
		protected:
			btRigidBody* m_body; //!< all the element have a body  == > otherwise it will be not manage with this system...
		public:
			void createRigidBody(float _mass=400000000.0f);
		public:
			/**
			 * @brief Constructor (when constructer is called just add element that did not change.
			 * The objest will be stored in a pool of element and keep a second time if needed  == > redure memory allocation,
			 * when needed, the system will call the init and un-init function...
			 */
			ElementPhysic(const std::shared_ptr<ege::Environement>& _env, bool _autoRigidBody=true);
			/**
			 * @brief Destructor
			 */
			virtual ~ElementPhysic();
			/**
			 * @brief get the element Type description string.
			 * @return A reference on the descriptive string.
			 */
			virtual const std::string& getType() const;
		protected:
			btCollisionShape* m_shape; //!< shape of the element (set a copy here to have the debug display of it)
		public:
			/**
			 * @brief set the shape properties.
			 * @param[in] _shape The shape pointer.
			 * @note : this remove the shape properties.
			 * @return true if no error occured
			 */
			bool setShape(btCollisionShape* _shape);
			/**
			 * @brief get a pointer on the bullet collision shape.
			 * @return the collision pointer.
			 */
			inline btCollisionShape* getShape() {
				return m_shape;
			};
		private:
			/**
			 * @brief remove the curent selected shape.
			 */
			void removeShape();
		public:
			virtual bool setMesh(const std::shared_ptr<ege::resource::Mesh>& _mesh);
			/**
			 * @brief draw the curent element (can have multiple display)
			 * @param[in] pass Id of the current pass : [0..?]
			 */
			virtual void draw(int32_t _pass=0);
			
			/**
			 * @brief draw the current life of the element
			 */
			// virtual void drawLife(const std::shared_ptr<ewol::resource::Colored3DObject>& _draw, const std::shared_ptr<ege::Camera>& _camera);
			
			/**
			 * @brief get the theoric position. Sometimes, the element has move due to an explosion or something else, then its real position in not the one that woult it be at the end ...
			 * @return the theoric position
			 */
			virtual vec3 getPositionTheoric() {
				return getPosition();
			};
			/**
			 * @brief set the current Theoric position of the element
			 * @param[in] set the 3D position.
			 */
			virtual void setPositionTheoric(const vec3& _pos) { };
		
			/**
			 * @brief get the current speed of the element
			 * @return the 3D speed.
			 */
			const vec3& getSpeed();
		
			/**
			 * @brief get the current mass of the element
			 * @return the mass in kG.
			 */
			const float getInvMass();
			
			virtual void setMass(float _value);
			virtual void setLinearVelocity(const vec3& _value);
			virtual void setTorqueImpulse(const vec3& _value);
			virtual void setAngularVelocity(const vec3& _value);
			
			
			
		protected:
			bool m_elementInPhysicsSystem;
		public:
			virtual void dynamicEnable();
			virtual void dynamicDisable();
		private:
			class localIA : public btActionInterface {
				private:
					ege::ElementPhysic& m_element;
				public:
					/**
					 * @brief Constructor
					 */
					localIA(ElementPhysic& _element) :
					  m_element(_element) {
						
					};
					/**
					 * @brief Destructor
					 */
					virtual ~localIA() {
						
					};
				public: // herited function
					void debugDraw(btIDebugDraw* _debugDrawer) {
						
					};
					void updateAction(btCollisionWorld* _collisionWorld, btScalar _step) {
						m_element.iaAction(_step);
					};
			};
			localIA* m_IA;
		public:
			/**
			 * @brief enable periodic call Of this object for processing Artificial Intelligence
			 */
			void iaEnable();
			/**
			 * @brief disable periodic call Of this object for processing Artificial Intelligence
			 */
			void iaDisable();
			/**
			 * @brief periodic call for intelligence artificial.
			 * @param[in] step : step of time in s
			 */
			virtual void iaAction(float _step) { };
			/**
			 * @brief, call when the element is removed (call only one time
			 */
			virtual void onDestroy() {};
			virtual const vec3& getPosition();
			virtual void setPosition(const vec3& _pos);
			virtual void drawDebug(const std::shared_ptr<ewol::resource::Colored3DObject>& _draw, const std::shared_ptr<ege::Camera>& _camera);
		protected:
			void drawShape(const btCollisionShape* _shape,
			               const std::shared_ptr<ewol::resource::Colored3DObject>& _draw,
			               mat4 _transformationMatrix,
			               std::vector<vec3> _tmpVertices);
	};
};

#endif


