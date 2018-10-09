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
#include <ege/Entity.hpp>
#include <ege/physics/shape/Box.hpp>
#include <ege/physics/shape/Sphere.hpp>
#include <ege/physics/shape/Cylinder.hpp>
#include <ege/physics/shape/Capsule.hpp>
#include <ege/physics/shape/Cone.hpp>
#include <ege/position/Component.hpp>
#include <ege/render/Component.hpp>
#include <ege/physics/Component.hpp>
#include <ege/Ray.hpp>

appl::Windows::Windows() {
	addObjectType("appl::Windows");
	propertyTitle.setDirectCheck("example ege : Collision");
}


static ememory::SharedPtr<ege::resource::Mesh> createViewBoxStar() {
	ememory::SharedPtr<ege::resource::Mesh> out = ege::resource::Mesh::create("viewBoxStar", "DATA:///texturedNoMaterial.prog");
	if (out == null) {
		return out;
	}
	ememory::SharedPtr<ege::Material> material = ememory::makeShared<ege::Material>();
	// set the entity material properties :
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
	if (myImage == null) {
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


// TODO : Assert When a dynamic object is sponed inside a static object
// like:
//             o-------o
//             |       |
//    o--------|   X   |--------o
//    |        |       |        |
//    |        o-------o        |
//    |                         |
//    |            X            |
//    |                         |
//    |                         |
//    |                         |
//    o-------------------------o



void appl::Windows::init() {
	ewol::widget::Windows::init();
	
	m_env = ege::Environement::create();
	// set the debug property on the engines
	m_env->getEngine("render")->properties.set("debug-normal", "true");
	m_env->getEngine("physics")->properties.set("debug-AABB", "true");
	m_env->getEngine("physics")->properties.set("debug-shape", "true");
	
	ememory::SharedPtr<ege::physics::Engine> tmpPhysics = ememory::dynamicPointerCast<ege::physics::Engine>(m_env->getEngine("physics"));
	if (tmpPhysics != null) {
		tmpPhysics->setGravity(vec3(0.0, 0.0, -9.81)); // generic earth gravity
	}
	
	// Create basic Camera
	m_camera = ememory::makeShared<ege::camera::View>();
	m_env->addCamera("basic", m_camera);
	m_cameraControler.setCamera(m_camera);
	
	ememory::SharedPtr<ege::widget::Scene> tmpWidget = ege::widget::Scene::create();
	if (tmpWidget == null) {
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
	if (myMesh != null) {
		ememory::SharedPtr<ege::Entity> entity = ememory::makeShared<ege::Entity>(m_env);
		// 1st Position component:
		etk::Transform3D transform(vec3(0,0,0), etk::Quaternion::identity());
		ememory::SharedPtr<ege::position::Component> componentPosition = ememory::makeShared<ege::position::Component>(transform);
		entity->addComponent(componentPosition);
		// 2nd something to diplay:
		ememory::SharedPtr<ege::render::Component> componentRender = ememory::makeShared<ege::render::Component>(myMesh);
		entity->addComponent(componentRender);
		// add it ..
		m_env->addEntity(entity);
	}
	// create basic gird: (no physics)
	myMesh = ege::resource::Mesh::createGrid(10, vec3(0,0,0), 5);
	if (myMesh != null) {
		ememory::SharedPtr<ege::Entity> entity = ememory::makeShared<ege::Entity>(m_env);
		// 1st Position component:
		etk::Transform3D transform(vec3(0,0,0), etk::Quaternion::identity());
		ememory::SharedPtr<ege::position::Component> componentPosition = ememory::makeShared<ege::position::Component>(transform);
		entity->addComponent(componentPosition);
		// 2nd something to diplay:
		ememory::SharedPtr<ege::render::Component> componentRender = ememory::makeShared<ege::render::Component>(myMesh);
		entity->addComponent(componentRender);
		// add it ..
		m_env->addEntity(entity);
	}
	
	// create cubes (surface) (the colision is bigger than the display)
	myMesh = ege::resource::Mesh::createCube(vec3(50,50,10), "basics", etk::color::green);
	if (myMesh != null) {
		ememory::SharedPtr<ege::Entity> entity = ememory::makeShared<ege::Entity>(m_env);
		// add all component:
		// 1st Position component:
		etk::Transform3D transform(vec3(0,0,-10.01), etk::Quaternion::identity());
		// 2nd something to diplay:
		ememory::SharedPtr<ege::render::Component> componentRender = ememory::makeShared<ege::render::Component>(myMesh);
		entity->addComponent(componentRender);
		// 3rd some physic:
		ememory::SharedPtr<ege::physics::Component> componentPhysics = ememory::makeShared<ege::physics::Component>(m_env, transform);
		ememory::SharedPtr<ege::physics::shape::Box> physic = ememory::makeShared<ege::physics::shape::Box>();
		physic->setSize(vec3(200.01,200.01,10.01));
		componentPhysics->addShape(physic);
		// The entity can not move
		componentPhysics->setType(ege::physics::Component::type::bodyStatic);
		componentPhysics->generate();
		entity->addComponent(componentPhysics);
		// add it ..
		m_env->addEntity(entity);
	}
	
	// create cubes that will move...
	myMesh = ege::resource::Mesh::createCube(vec3(5,5,5), "basics", etk::color::yellow);
	if (myMesh != null) {
		ememory::SharedPtr<ege::Entity> entity = ememory::makeShared<ege::Entity>(m_env);
		// add all component:
		// 1st Position component:
		etk::Transform3D transform(vec3(0,0,10), etk::Quaternion::identity());
		// 2nd something to diplay:
		ememory::SharedPtr<ege::render::Component> componentRender = ememory::makeShared<ege::render::Component>(myMesh);
		entity->addComponent(componentRender);
		// 3rd some physic:
		ememory::SharedPtr<ege::physics::Component> componentPhysics = ememory::makeShared<ege::physics::Component>(m_env, transform);
		ememory::SharedPtr<ege::physics::shape::Box> physic = ememory::makeShared<ege::physics::shape::Box>();
		physic->setSize(vec3(5.01,5.01,5.01));
		physic->setMass(3000);
		componentPhysics->addShape(physic);
		componentPhysics->setType(ege::physics::Component::type::bodyDynamic);
		componentPhysics->generate();
		entity->addComponent(componentPhysics);
		// add it ..
		m_env->addEntity(entity);
		m_entity = entity;
	}
	m_env->propertyStatus.set(ege::gameStart);
}

bool appl::Windows::onEventEntry(const ewol::event::Entry& _event) {
	if (m_cameraControler.onEventEntry(_event) == true) {
		return true;
	}
	if(_event.getType() == gale::key::keyboard::character ) {
		ememory::SharedPtr<ege::physics::Component> physic = ememory::dynamicPointerCast<ege::physics::Component>(m_entity->getComponent("physics"));
		if (physic == null) {
			APPL_ERROR("Can not get the component");
			return false;
		}
		if (_event.getChar() == 'r') {
			if (_event.getStatus() == gale::key::status::up) {
				physic->applyTorque(vec3(0,0,0), true);
			} else if (_event.getStatus() == gale::key::status::down) {
				physic->applyTorque(vec3(0,0,10000000), true);
			}
			return true;
		}
		if (_event.getChar() == 'u') {
			if (_event.getStatus() == gale::key::status::up) {
				physic->applyForceToCenterOfMass(vec3(0,0,0), true);
			} else if (_event.getStatus() == gale::key::status::down) {
				physic->applyForceToCenterOfMass(vec3(0,0,10000000), true);
			}
			return true;
		}
		if (_event.getChar() == 'z') {
			if (_event.getStatus() == gale::key::status::up) {
				physic->applyForceToCenterOfMass(vec3(0,0,0), true);
			} else if (_event.getStatus() == gale::key::status::down) {
				physic->applyForceToCenterOfMass(vec3(0,10000000,0), true);
			}
			return true;
		}
		if (_event.getChar() == 's') {
			if (_event.getStatus() == gale::key::status::up) {
				physic->applyForceToCenterOfMass(vec3(0,0,0), true);
			} else if (_event.getStatus() == gale::key::status::down) {
				physic->applyForceToCenterOfMass(vec3(0,-10000000,0), true);
			}
			return true;
		}
		if (_event.getChar() == 'q') {
			if (_event.getStatus() == gale::key::status::up) {
				physic->applyForceToCenterOfMass(vec3(0,0,0), true);
			} else if (_event.getStatus() == gale::key::status::down) {
				physic->applyForceToCenterOfMass(vec3(10000000,0,0), true);
			}
			return true;
		}
		if (_event.getChar() == 'd') {
			if (_event.getStatus() == gale::key::status::up) {
				physic->applyForceToCenterOfMass(vec3(0,0,0), true);
			} else if (_event.getStatus() == gale::key::status::down) {
				physic->applyForceToCenterOfMass(vec3(-10000000,0,0), true);
			}
			return true;
		}
	}
	return false;
}

bool appl::Windows::onEventInput(const ewol::event::Input& _event) {
	if (m_cameraControler.onEventInput(_event, relativePosition(_event.getPos())) == true) {
		return true;
	}
	if (_event.getId() == 2) {
		if (_event.getStatus() == gale::key::status::down) {
			ememory::SharedPtr<ege::physics::Component> physic = ememory::dynamicPointerCast<ege::physics::Component>(m_entity->getComponent("physics"));
			if (physic == null) {
				APPL_ERROR("Can not get the component");
				return false;
			}
			physic->applyForceToCenterOfMass(vec3(0,0,10000000), true);
		} else if (_event.getStatus() == gale::key::status::up) {
			ememory::SharedPtr<ege::physics::Component> physic = ememory::dynamicPointerCast<ege::physics::Component>(m_entity->getComponent("physics"));
			if (physic == null) {
				APPL_ERROR("Can not get the component");
				return false;
			}
			physic->applyForceToCenterOfMass(vec3(0,0,0), true);
		}
	}
	if (_event.getId() == 1) {
		if (_event.getStatus() == gale::key::status::down) {
			ememory::SharedPtr<ege::physics::Component> physic = ememory::dynamicPointerCast<ege::physics::Component>(m_entity->getComponent("physics"));
			if (physic == null) {
				APPL_ERROR("Can not get the component");
				return false;
			}
			//physic->applyForce(vec3(0,0,10),vec3(0,0,0));
			physic->applyRelativeTorque(vec3(0,0,10000000), true);
		} else if (_event.getStatus() == gale::key::status::up) {
			ememory::SharedPtr<ege::physics::Component> physic = ememory::dynamicPointerCast<ege::physics::Component>(m_entity->getComponent("physics"));
			if (physic == null) {
				APPL_ERROR("Can not get the component");
				return false;
			}
			physic->applyRelativeTorque(vec3(0,0,0), true);
		}
	}
	/*
	if (_event.getId() == 1) {
		if (_event.getStatus() == gale::key::status::down) {
			vec2 pos = relativePosition(_event.getPos());
			ege::Ray ray = m_camera->getRayFromScreenPosition(pos, m_size);
			
			ememory::SharedPtr<ege::resource::Mesh> myMesh;
			myMesh = ege::resource::Mesh::createCube(1, "basics", etk::color::orange);
			if (myMesh != null) {
				ememory::SharedPtr<ege::Entity> entity = ememory::makeShared<ege::Entity>(m_env);
				// add all component:
				// 1st Position component:
				etk::Transform3D transform(ray.getOrigin(), etk::Quaternion::identity());
				//ememory::SharedPtr<ege::position::Component> componentPosition = ememory::makeShared<ege::position::Component>(transform);
				//entity->addComponent(componentPosition);
				// 2nd something to diplay:
				ememory::SharedPtr<ege::render::Component> componentRender = ememory::makeShared<ege::render::Component>(myMesh);
				entity->addComponent(componentRender);
				// 3rd some physic:
				ememory::SharedPtr<ege::physics::Component> componentPhysics = ememory::makeShared<ege::physics::Component>(m_env, transform);
				ememory::SharedPtr<ege::physics::shape::Box> physic = ememory::makeShared<ege::physics::shape::Box>();
				physic->setSize(vec3(1.01,1.01,1.01));
				physic->setMass(1000);
				componentPhysics->setType(ege::physics::Component::type::bodyDynamic);
				componentPhysics->addShape(physic);
				componentPhysics->generate();
				// set has dynamic object (can move)
				//APPL_CRITICAL("velocity : " << ray.getDirection()*100);
				componentPhysics->setLinearVelocity(ray.getDirection()*100);
				entity->addComponent(componentPhysics);
				// add it ..
				m_env->addEntity(entity);
			}
			return true;
		}
	}
	*/
	return false;
}


