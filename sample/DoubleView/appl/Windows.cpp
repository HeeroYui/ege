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

appl::Windows::Windows() {
	addObjectType("appl::Windows");
	propertyTitle.setDirectCheck("example ege : DoubleView");
}


static ememory::SharedPtr<ege::resource::Mesh> createViewBoxStar() {
	ememory::SharedPtr<ege::resource::Mesh> out = ege::resource::Mesh::create("viewBoxStar", "DATA:texturedNoMaterial.prog");
	if (out != null) {
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
		if (null == myImage) {
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
	}
	return out;
}




void appl::Windows::init() {
	ewol::widget::Windows::init();
	
	m_env = ege::Environement::create();
	// Create basic Camera
	m_camera = ememory::makeShared<ege::camera::View>();
	m_env->addCamera("basic", m_camera);
	m_cameraControler.setCamera(m_camera);
	// Create basic Camera
	ememory::SharedPtr<ege::camera::View> camera2 = ememory::makeShared<ege::camera::View>(vec3(100,0,0), vec3(0,0,0));
	m_env->addCamera("front", camera2);
	// Create basic Camera
	ememory::SharedPtr<ege::camera::View> camera3 = ememory::makeShared<ege::camera::View>(vec3(20,100,20), vec3(0,0,0));
	m_env->addCamera("top", camera3);
	// Create basic Camera
	ememory::SharedPtr<ege::camera::View> camera4 = ememory::makeShared<ege::camera::View>(vec3(0,0,100), vec3(0,0,0));
	m_env->addCamera("left", camera4);
	
	ememory::SharedPtr<ewol::widget::Sizer> tmpSizerVert = ewol::widget::Sizer::create();
	if (tmpSizerVert == null) {
		APPL_CRITICAL("Can not allocate widget ==> display might be in error");
	} else {
		tmpSizerVert->propertyMode.set(ewol::widget::Sizer::modeVert);
		setSubWidget(tmpSizerVert);
		ememory::SharedPtr<ege::widget::Scene> tmpWidget = ege::widget::Scene::create();
		if (tmpWidget == null) {
			APPL_CRITICAL("Can not allocate widget ==> display might be in error");
		} else {
			tmpWidget->setEnv(m_env);
			tmpWidget->propertyExpand.set(bvec2(true,true));
			tmpWidget->propertyFill.set(bvec2(true,true));
			tmpWidget->setCamera("basic");
			tmpSizerVert->subWidgetAdd(tmpWidget);
		}
		ememory::SharedPtr<ewol::widget::Sizer> tmpSizerHori = ewol::widget::Sizer::create();
		if (tmpSizerHori == null) {
			APPL_CRITICAL("Can not allocate widget ==> display might be in error");
		} else {
			tmpSizerHori->propertyMode.set(ewol::widget::Sizer::modeHori);
			tmpSizerVert->subWidgetAdd(tmpSizerHori);
			tmpWidget = ege::widget::Scene::create();
			if (tmpWidget == null) {
				APPL_CRITICAL("Can not allocate widget ==> display might be in error");
			} else {
				tmpWidget->setEnv(m_env);
				tmpWidget->propertyExpand.set(bvec2(true,true));
				tmpWidget->propertyFill.set(bvec2(true,true));
				tmpWidget->setCamera("front");
				tmpSizerHori->subWidgetAdd(tmpWidget);
			}
			tmpWidget = ege::widget::Scene::create();
			if (tmpWidget == null) {
				APPL_CRITICAL("Can not allocate widget ==> display might be in error");
			} else {
				tmpWidget->setEnv(m_env);
				tmpWidget->propertyExpand.set(bvec2(true,true));
				tmpWidget->propertyFill.set(bvec2(true,true));
				tmpWidget->setCamera("top");
				tmpSizerHori->subWidgetAdd(tmpWidget);
			}
		}
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
	myMesh = ege::resource::Mesh::createCube(3, "basics", etk::color::orange);
	if (myMesh != null) {
		ememory::SharedPtr<ege::Entity> entity = ememory::makeShared<ege::Entity>(m_env);
		// add all component:
		// 1st Position component:
		etk::Transform3D transform(vec3(-20,10,0), etk::Quaternion::identity());
		//ememory::SharedPtr<ege::position::Component> componentPosition = ememory::makeShared<ege::position::Component>(transform);
		//entity->addComponent(componentPosition);
		// 2nd something to diplay:
		ememory::SharedPtr<ege::render::Component> componentRender = ememory::makeShared<ege::render::Component>(myMesh);
		entity->addComponent(componentRender);
		// 3rd some physic:
		ememory::SharedPtr<ege::physics::Component> componentPhysics = ememory::makeShared<ege::physics::Component>(m_env, transform);
		ememory::SharedPtr<ege::physics::shape::Box> physic = ememory::makeShared<ege::physics::shape::Box>();
		physic->setSize(vec3(3.01,3.01,3.01));
		physic->setMass(50000);
		componentPhysics->addShape(physic);
		componentPhysics->generate();
		entity->addComponent(componentPhysics);
		// add it ..
		m_env->addEntity(entity);
	}
	myMesh = ege::resource::Mesh::createSphere(4, "basics", etk::color::blue);
	if (myMesh != null) {
		ememory::SharedPtr<ege::Entity> entity = ememory::makeShared<ege::Entity>(m_env);
		// add all component:
		// 1st Position component:
		etk::Transform3D transform(vec3(20,10,0), etk::Quaternion::identity());
		//ememory::SharedPtr<ege::position::Component> componentPosition = ememory::makeShared<ege::position::Component>(transform);
		//entity->addComponent(componentPosition);
		// 2nd something to diplay:
		ememory::SharedPtr<ege::render::Component> componentRender = ememory::makeShared<ege::render::Component>(myMesh);
		entity->addComponent(componentRender);
		// 3rd some physic:
		ememory::SharedPtr<ege::physics::Component> componentPhysics = ememory::makeShared<ege::physics::Component>(m_env, transform);
		ememory::SharedPtr<ege::physics::shape::Sphere> physic = ememory::makeShared<ege::physics::shape::Sphere>();
		physic->setRadius(4.01);
		physic->setMass(500000);
		componentPhysics->addShape(physic);
		componentPhysics->generate();
		entity->addComponent(componentPhysics);
		// add it ..
		m_env->addEntity(entity);
	}
	m_env->propertyStatus.set(ege::gameStart);
}

bool appl::Windows::onEventEntry(const ewol::event::Entry& _event) {
	if (m_cameraControler.onEventEntry(_event) == true) {
		return true;
	}
	return false;
}

bool appl::Windows::onEventInput(const ewol::event::Input& _event) {
	if (m_cameraControler.onEventInput(_event, relativePosition(_event.getPos())) == true) {
		return true;
	}
	return false;
}

