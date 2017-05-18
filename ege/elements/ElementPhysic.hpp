/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Matrix4x4.hpp>
#include <vector>
#include <ewol/debug.hpp>
#include <ewol/widget/Widget.hpp>
#include <gale/renderer/openGL/openGL.hpp>
#include <ewol/resource/Colored3DObject.hpp>
#include <ege/resource/Mesh.hpp>
#include <ege/camera/Camera.hpp>
#include <ewol/compositing/Text.hpp>
#include <ege/Environement.hpp>
#include <ege/elements/Element.hpp>
#include <ephysics/reactphysics3d.h>


#define INDEX_RIGHT_AXIS   (0)
#define INDEX_FORWARD_AXIS (1)
#define INDEX_UP_AXIS      (2)

#define ELEMENT_SCALE     (1.0f/8.0f)

namespace ege {
	class ElementPhysic : public ege::Element {
		private:
			static void FunctionFreeShape(void* _pointer);
		protected:
			rp3d::RigidBody* m_body; //!< all the element have a body  == > otherwise it will be not manage with this system...
		public:
			void createRigidBody(float _mass=400000000.0f, bool _static=false);
		public:
			/**
			 * @brief Constructor (when constructer is called just add element that did not change.
			 * The object will be stored in a pool of element and keep a second time if needed  == > redure memory allocation,
			 * when needed, the system will call the init and un-init function...
			 * @param[in] _env glabal ege environement
			 * @param[in] _autoRigidBody add a basic rigid body (not availlable after befor setting a shape)
			 */
			ElementPhysic(const ememory::SharedPtr<ege::Environement>& _env, bool _autoRigidBody=true);
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
			//btCollisionShape* m_shape; //!< shape of the element (set a copy here to have the debug display of it)
		public:
			/**
			 * @brief set the shape properties.
			 * @param[in] _shape The shape pointer.
			 * @note : this remove the shape properties.
			 * @return true if no error occured
			 */
			//bool setShape(btCollisionShape* _shape);
			/**
			 * @brief get a pointer on the bullet collision shape.
			 * @return the collision pointer.
			 */
			 /*
			inline btCollisionShape* getShape() {
				return m_shape;
			};
			*/
		private:
			/**
			 * @brief remove the curent selected shape.
			 */
			//void removeShape();
		public:
			virtual bool setMesh(ememory::SharedPtr<ege::resource::Mesh> _mesh);
			/**
			 * @brief draw the curent element (can have multiple display)
			 * @param[in] pass Id of the current pass : [0..?]
			 */
			virtual void draw(int32_t _pass=0);
			
			/**
			 * @brief draw the current life of the element
			 */
			// virtual void drawLife(const ememory::SharedPtr<ewol::resource::Colored3DObject>& _draw, const ememory::SharedPtr<ege::Camera>& _camera);
			virtual void drawNormalDebug(ememory::SharedPtr<ewol::resource::Colored3DObject> _draw, ememory::SharedPtr<ege::Camera> _camera);
		// TODO : Remove this ...
		protected:
			vec3 m_theoricPosition;
		public:
			/**
			 * @brief get the theoric position. Sometimes, the element has move due to an explosion or something else, then its real position in not the one that woult it be at the end ...
			 * @return the theoric position
			 */
			virtual vec3 getPositionTheoric() {
				return m_theoricPosition;
			};
			/**
			 * @brief set the current Theoric position of the element
			 * @param[in] set the 3D position.
			 */
			virtual void setPositionTheoric(const vec3& _pos) { m_theoricPosition = _pos; };
		
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
			
			//btQuaternion getOrientation() const;
			
		protected:
			bool m_elementInPhysicsSystem;
		public:
			virtual void dynamicEnable();
			virtual void dynamicDisable();
		private:
			class localIA {
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
					/*
					void debugDraw(btIDebugDraw* _debugDrawer) {
						
					};
					void updateAction(btCollisionWorld* _collisionWorld, btScalar _step) {
						m_element.iaAction(_step);
					};
					*/
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
			virtual void drawDebug(ememory::SharedPtr<ewol::resource::Colored3DObject> _draw, ememory::SharedPtr<ege::Camera> _camera);
		protected:
			void drawShape(/*const btCollisionShape* _shape,*/
			               ememory::SharedPtr<ewol::resource::Colored3DObject> _draw,
			               mat4 _transformationMatrix,
			               std::vector<vec3> _tmpVertices);
		protected:
			bool m_detectCollisionEnable; //!< physic collision detect enable.
		public:
			/**
			 * @brief get collision status of the object.
			 * @return the collision status.
			 */
			bool getCollisionDetectionStatus() {
				return m_detectCollisionEnable;
			}
			/**
			 * @brief Change enable status of the object.
			 * @param[in] _status new requested status.
			 */
			void setCollisionDetectionStatus(bool _status=true);
			/**
			 * @brief when a collision is detected with an other object (just after calculate data update
			 * @param[in] _obj the Other object
			 * @param[in] _point Position of the impact in the global world
			 * @param[in] _normal Normal of the impact
			 */
			virtual void onCollisionDetected(const ememory::SharedPtr<ege::Element>& _obj, const vec3& _point, const vec3& _normal) {};
	};
}


