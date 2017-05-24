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
	propertyTitle.setDirectCheck("example ege: MeshCreator");
}

static ememory::SharedPtr<ege::resource::Mesh> createViewBoxStar() {
	ememory::SharedPtr<ege::resource::Mesh> out = ege::resource::Mesh::create("---", "DATA:texturedNoMaterial.prog");
	if (out != nullptr) {
		ememory::SharedPtr<ege::Material> material = ememory::makeShared<ege::Material>();
		// set the entity material properties :
		material->setAmbientFactor(vec4(1,1,1,1));
		material->setDiffuseFactor(vec4(0,0,0,1));
		material->setSpecularFactor(vec4(0,0,0,1));
		material->setShininess(1);
		material->setTexture0(""); //"
		out->addMaterial("basics", material);
		// 1024  == > 1<<9
		// 2048  == > 1<<10
		// 4096  == > 1<<11
		int32_t size = 1<<11;
		material->setImageSize(ivec2(size,size));
		egami::Image* myImage = material->get();
		if (nullptr == myImage) {
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

static ememory::SharedPtr<ege::resource::Mesh> createMars() {
	ememory::SharedPtr<ege::resource::Mesh> out = ege::resource::Mesh::create("---");
	if (out != nullptr) {
		ememory::SharedPtr<ege::Material> material = ememory::makeShared<ege::Material>();
		material->setAmbientFactor(vec4(0.112f,0.112f,0.112f,1.0f));
		material->setDiffuseFactor(vec4(0.512f,0.512f,0.512f,1.0f));
		material->setSpecularFactor(vec4(0.5f,0.5f,0.5f,1.0f));
		material->setShininess(96.078431f);
		material->setTexture0("DATA:texture_mars.png");
		out->addMaterial("basics", material);
		out->createIcoSphere("basics", 16, 3);
		out->generateVBO();
		//m_env->addStaticMeshToDraw(myMesh);
	}
	return out;
}

void appl::Windows::init() {
	ewol::widget::Windows::init();
	
	getObjectManager().periodicCall.connect(sharedFromThis(), &appl::Windows::onCallbackPeriodicUpdateCamera);
	
	m_env = ege::Environement::create();
	// Create basic Camera
	m_camera = ememory::makeShared<ege::camera::View>(vec3(30,-100,30), vec3(50,0,0));
	m_env->addCamera("basic", m_camera);
	
	ememory::SharedPtr<ege::widget::Scene> tmpWidget = ege::widget::Scene::create();
	if (tmpWidget == nullptr) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
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
	if (myMesh != nullptr) {
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
	if (true) {
		myMesh = createMars();
		if (myMesh != nullptr) {
			ememory::SharedPtr<ege::Entity> entity = ememory::makeShared<ege::Entity>(m_env);
			// 1st Position component:
			etk::Transform3D transform(vec3(5,0,0), etk::Quaternion::identity());
			ememory::SharedPtr<ege::position::Component> componentPosition = ememory::makeShared<ege::position::Component>(transform);
			entity->addComponent(componentPosition);
			// 2nd something to diplay:
			ememory::SharedPtr<ege::render::Component> componentRender = ememory::makeShared<ege::render::Component>(myMesh);
			entity->addComponent(componentRender);
			// add it ..
			m_env->addEntity(entity);
		}
	}
	m_env->propertyStatus.set(ege::gameStart);
}


void appl::Windows::onCallbackPeriodicUpdateCamera(const ewol::event::Time& _event) {
	static float offset = 0;
	offset += 0.01;
	static float offset2 = 0;
	offset2 += 0.003;
	m_camera->setEye(vec3(100*std::sin(offset),40*std::cos(offset2),100*std::cos(offset))+vec3(50,0,0));
}


