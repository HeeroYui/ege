/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license MPL v2.0 (see license file)
 */

#include <ewol/ewol.hpp>
#include <appl/debug.hpp>
#include <appl/Windows.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/widget/Sizer.hpp>
#include <ewol/object/Manager.hpp>
#include <ege/widget/Scene.hpp>
#include <ege/camera/View.hpp>
#include <etk/tool.hpp>
#include <ege/elements/Element.hpp>
#include <ege/physicsShape/PhysicsBox.hpp>
#include <ege/physicsShape/PhysicsSphere.hpp>
#include <ege/position/Component.hpp>
#include <ege/render/Component.hpp>
#include <ege/physics/Component.hpp>
#include <ege/Ray.hpp>

appl::Windows::Windows() {
	addObjectType("appl::Windows");
	propertyTitle.setDirectCheck("example ege : Collision");
}


static ememory::SharedPtr<ege::resource::Mesh> createViewBoxStar() {
	ememory::SharedPtr<ege::resource::Mesh> out = ege::resource::Mesh::create("viewBoxStar", "DATA:texturedNoMaterial.prog");
	if (out == nullptr) {
		return out;
	}
	ememory::SharedPtr<ege::Material> material = ememory::makeShared<ege::Material>();
	// set the element material properties :
	material->setAmbientFactor(vec4(1,1,1,1));
	material->setDiffuseFactor(vec4(0,0,0,1));
	material->setSpecularFactor(vec4(0,0,0,1));
	material->setShininess(1);
	// 1024  == > 1<<9
	// 2048  == > 1<<10
	// 4096  == > 1<<11
	int32_t size = 1<<11;
	//material->setTexture0(""); //"
	material->setTexture0Magic(ivec2(size,size));
	out->addMaterial("basics", material);
	//material->setImageSize(ivec2(size,size));
	egami::Image* myImage = material->get();
	if (myImage == nullptr) {
		return out;
	}
	myImage->clear(etk::color::black);
	ivec2 tmpPos;
	for (int32_t iii=0; iii<6000; iii++) {
		tmpPos.setValue(etk::tool::frand(0,size), etk::tool::frand(0,size)) ;
		myImage->set(tmpPos, etk::color::white);
	}
	material->flush();
	// basis on cube :
	out->createViewBox("basics", 1000/* distance */);
	// generate the VBO
	out->generateVBO();
	return out;
}




void appl::Windows::init() {
	ewol::widget::Windows::init();
	
	getObjectManager().periodicCall.connect(sharedFromThis(), &appl::Windows::onCallbackPeriodicCheckCollision);
	
	m_env = ege::Environement::create();
	// set the debug property on the engines
	m_env->getEngine("render")->properties.set("debug-normal", "true");
	m_env->getEngine("physics")->properties.set("debug-AABB", "true");
	m_env->getEngine("physics")->properties.set("debug-shape", "true");
	// Create basic Camera
	m_camera = ememory::makeShared<ege::camera::View>(vec3(30,30,-100), vec3(0,0,0));
	m_camera->setEye(vec3(100*std::sin(m_angleTetha),100*std::cos(m_angleTetha),40*std::cos(m_anglePsy)));
	m_env->addCamera("basic", m_camera);
	
	ememory::SharedPtr<ege::widget::Scene> tmpWidget = ege::widget::Scene::create();
	if (tmpWidget == nullptr) {
		APPL_CRITICAL("Can not allocate widget ==> display might be in error");
	} else {
		tmpWidget->setEnv(m_env);
		tmpWidget->propertyExpand.set(bvec2(true,true));
		tmpWidget->propertyFill.set(bvec2(true,true));
		tmpWidget->setCamera("basic");
		setSubWidget(tmpWidget);
	}
	
	ememory::SharedPtr<ege::resource::Mesh> myMesh;
	// Create an external box: (no physics)
	myMesh = createViewBoxStar();
	if (myMesh != nullptr) {
		ememory::SharedPtr<ege::Element> element = ememory::makeShared<ege::Element>(m_env);
		// 1st Position component:
		etk::Transform3D transform(vec3(0,0,0), etk::Quaternion::identity());
		ememory::SharedPtr<ege::position::Component> componentPosition = ememory::makeShared<ege::position::Component>(transform);
		element->addComponent(componentPosition);
		// 2nd something to diplay:
		ememory::SharedPtr<ege::render::Component> componentRender = ememory::makeShared<ege::render::Component>(myMesh);
		element->addComponent(componentRender);
		// add it ..
		m_env->addElement(element);
	}
	// create basic gird: (no physics)
	myMesh = ege::resource::Mesh::createGrid(10, vec3(0,0,0), 5);
	if (myMesh != nullptr) {
		ememory::SharedPtr<ege::Element> element = ememory::makeShared<ege::Element>(m_env);
		// 1st Position component:
		etk::Transform3D transform(vec3(0,0,0), etk::Quaternion::identity());
		ememory::SharedPtr<ege::position::Component> componentPosition = ememory::makeShared<ege::position::Component>(transform);
		element->addComponent(componentPosition);
		// 2nd something to diplay:
		ememory::SharedPtr<ege::render::Component> componentRender = ememory::makeShared<ege::render::Component>(myMesh);
		element->addComponent(componentRender);
		// add it ..
		m_env->addElement(element);
	}
	// create cubes ...
	myMesh = ege::resource::Mesh::createCube(3, "basics", etk::color::green);
	if (myMesh != nullptr) {
		ememory::SharedPtr<ege::Element> element = ememory::makeShared<ege::Element>(m_env);
		// add all component:
		// 1st Position component:
		etk::Transform3D transform(vec3(0,0,2), etk::Quaternion::identity());
		// 2nd something to diplay:
		ememory::SharedPtr<ege::render::Component> componentRender = ememory::makeShared<ege::render::Component>(myMesh);
		element->addComponent(componentRender);
		// 3rd some physic:
		ememory::SharedPtr<ege::physics::Component> componentPhysics = ememory::makeShared<ege::physics::Component>(m_env, transform);
		ememory::SharedPtr<ege::PhysicsBox> physic = ememory::makeShared<ege::PhysicsBox>();
		physic->setSize(vec3(3.2,3.2,3.2));
		physic->setMass(300000);
		componentPhysics->addShape(physic);
		componentPhysics->generate();
		element->addComponent(componentPhysics);
		// add it ..
		m_env->addElement(element);
	}
	myMesh = ege::resource::Mesh::createCube(3, "basics", etk::color::orange);
	if (myMesh != nullptr) {
		ememory::SharedPtr<ege::Element> element = ememory::makeShared<ege::Element>(m_env);
		// add all component:
		// 1st Position component:
		etk::Transform3D transform(vec3(20,-10,10), etk::Quaternion::identity());
		//ememory::SharedPtr<ege::position::Component> componentPosition = ememory::makeShared<ege::position::Component>(transform);
		//element->addComponent(componentPosition);
		// 2nd something to diplay:
		ememory::SharedPtr<ege::render::Component> componentRender = ememory::makeShared<ege::render::Component>(myMesh);
		element->addComponent(componentRender);
		// 3rd some physic:
		ememory::SharedPtr<ege::physics::Component> componentPhysics = ememory::makeShared<ege::physics::Component>(m_env, transform);
		ememory::SharedPtr<ege::PhysicsBox> physic = ememory::makeShared<ege::PhysicsBox>();
		physic->setSize(vec3(3.2,3.2,3.2));
		physic->setMass(50000);
		componentPhysics->addShape(physic);
		componentPhysics->generate();
		element->addComponent(componentPhysics);
		// add it ..
		m_env->addElement(element);
	}
	m_env->propertyStatus.set(ege::gameStart);
}
/*
namespace appl {
	class ElementHerit : public ege::ElementPhysic {
		public:
			ElementHerit(const ememory::SharedPtr<ege::Environement>& _env, bool _autoRigidBody=true) :
			  ege::ElementPhysic(_env, _autoRigidBody) {
				setCollisionDetectionStatus(true);
			}
			virtual void onCollisionDetected(const ememory::SharedPtr<ege::Element>& _obj, const vec3& _point, const vec3& _normal) {
				APPL_WARNING("[" << getUID() << "] collision : pos=" << _point << " norm=" <<_normal);
			}
	};
}
*/


bool appl::Windows::onEventInput(const ewol::event::Input& _event) {
	static float ploppp=1;
	if (_event.getId() == 1) {
		if (_event.getStatus() == gale::key::status::down) {
			vec2 pos = relativePosition(_event.getPos());
			ege::Ray ray = m_camera->getRayFromScreenPosition(pos, m_size);
			
			ememory::SharedPtr<ege::resource::Mesh> myMesh;
			myMesh = ege::resource::Mesh::createCube(1, "basics", etk::color::orange);
			if (myMesh != nullptr) {
				ememory::SharedPtr<ege::Element> element = ememory::makeShared<ege::Element>(m_env);
				// add all component:
				// 1st Position component:
				etk::Transform3D transform(ray.getOrigin(), etk::Quaternion::identity());
				//ememory::SharedPtr<ege::position::Component> componentPosition = ememory::makeShared<ege::position::Component>(transform);
				//element->addComponent(componentPosition);
				// 2nd something to diplay:
				ememory::SharedPtr<ege::render::Component> componentRender = ememory::makeShared<ege::render::Component>(myMesh);
				element->addComponent(componentRender);
				// 3rd some physic:
				ememory::SharedPtr<ege::physics::Component> componentPhysics = ememory::makeShared<ege::physics::Component>(m_env, transform);
				ememory::SharedPtr<ege::PhysicsBox> physic = ememory::makeShared<ege::PhysicsBox>();
				physic->setSize(vec3(1.1,1.1,1.1));
				physic->setMass(1000);
				componentPhysics->setType(ege::physics::Component::type::bodyDynamic);
				componentPhysics->addShape(physic);
				componentPhysics->generate();
				// set has dynamic object (can move)
				//APPL_CRITICAL("velocity : " << ray.getDirection()*100);
				componentPhysics->setLinearVelocity(ray.getDirection()*100);
				element->addComponent(componentPhysics);
				// add it ..
				m_env->addElement(element);
			}
			return true;
		}
	} else if (_event.getId() == 4) {
		ploppp += 0.01f;
		m_camera->setEye(vec3(100*std::sin(m_angleTetha),100*std::cos(m_angleTetha),80*std::cos(m_anglePsy))*ploppp);
	} else if (_event.getId() == 5) {
		ploppp -= 0.01f;
		if (ploppp == 0) {
			ploppp = 1.0f;
		}
		m_camera->setEye(vec3(100*std::sin(m_angleTetha),100*std::cos(m_angleTetha),80*std::cos(m_anglePsy))*ploppp);
	} else if (_event.getId() == 3) {
		if (_event.getStatus() == gale::key::status::down) {
			m_oldScreenPos = relativePosition(_event.getPos());
			return true;
		} else if (_event.getStatus() == gale::key::status::move) {
			vec2 pos = relativePosition(_event.getPos());
			m_angleTetha -= (m_oldScreenPos.x()-pos.x())*0.05f;
			m_anglePsy += (m_oldScreenPos.y()-pos.y())*0.01f;
			m_camera->setEye(vec3(100*std::sin(m_angleTetha),100*std::cos(m_angleTetha),80*std::cos(m_anglePsy))*ploppp);
			m_oldScreenPos = relativePosition(_event.getPos());
			return true;
		}
	} else if (_event.getId() == 2) {
		if (_event.getStatus() == gale::key::status::down) {
			m_oldScreenPos = relativePosition(_event.getPos());
			return true;
		} else if (_event.getStatus() == gale::key::status::move) {
			vec2 pos = relativePosition(_event.getPos())*0.2;
			pos -= m_oldScreenPos*0.2;
			float cameraAngle = m_camera->getTetha();
			vec3 newPos = vec3(std::sin(cameraAngle)*pos.x() + std::cos(cameraAngle)*pos.y(),
			                   std::cos(cameraAngle)*pos.x() + std::sin(cameraAngle)*pos.y(),
			                   0);
			APPL_ERROR("apply offset = " << newPos << " from pos=" << pos << " angle=" << cameraAngle);
			newPos += m_camera->getTarget();
			newPos.setMin(vec3(200,200,200));
			newPos.setMax(vec3(-200,-200,-200));
			m_camera->setTarget(newPos);
			m_oldScreenPos = relativePosition(_event.getPos());
			return true;
		}
	} else if (_event.getId() == 10) {
		m_camera->setAngle(m_camera->getAngle() + 0.01f);
	} else if (_event.getId() == 11) {
		m_camera->setAngle(m_camera->getAngle() - 0.01f);
	} 
	return false;
}

void appl::Windows::onCallbackPeriodicCheckCollision(const ewol::event::Time& _event) {
	/*
	std::vector<ege::physics::Engine::collisionPoints> list = m_env->getPhysicEngine().getListOfCollision();
	
	if (list.size() != 0) {
		APPL_INFO("num contact =" << list.size());
	}
	for (size_t iii=0;iii<list.size();++iii) {
		APPL_ERROR("    [" << list[iii].elem1->getUID() << "]:point1=" << list[iii].positionElem1 << " [" << list[iii].elem1->getUID() << "]:point2=" << list[iii].positionElem2  << " normal=" << list[iii].normalElem2);
	}
	*/
}



