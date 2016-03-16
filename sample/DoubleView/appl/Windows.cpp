/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license APACHE-2 (see license file)
 */

#include <ewol/ewol.h>
#include <appl/debug.h>
#include <appl/Windows.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Sizer.h>
#include <ewol/object/Manager.h>
#include <ege/widget/Scene.h>
#include <ege/camera/View.h>
#include <etk/tool.h>
#include <ege/elements/ElementBase.h>
#include <ege/elements/ElementPhysic.h>
#include <ege/physicsShape/PhysicsBox.h>
#include <ege/physicsShape/PhysicsSphere.h>

#undef __class__
#define __class__ "Windows"

appl::Windows::Windows() {
	addObjectType("appl::Windows");
	propertyTitle.setDirectCheck("example ege : DoubleView");
}


static std::shared_ptr<ege::resource::Mesh> createViewBoxStar() {
	std::shared_ptr<ege::resource::Mesh> out = ege::resource::Mesh::create("viewBoxStar", "DATA:texturedNoMaterial.prog");
	if (out != nullptr) {
		std::shared_ptr<ege::Material> material = std::make_shared<ege::Material>();
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




void appl::Windows::init() {
	ewol::widget::Windows::init();
	
	m_env = ege::Environement::create();
	// Create basic Camera
	m_camera = std::make_shared<ege::camera::View>(vec3(30,30,-100), vec3(0,0,0));
	m_camera->setEye(vec3(100*std::sin(m_angleTetha),100*std::cos(m_angleTetha),40*std::cos(m_anglePsy)));
	m_env->addCamera("basic", m_camera);
	// Create basic Camera
	std::shared_ptr<ege::camera::View> camera2 = std::make_shared<ege::camera::View>(vec3(100,0,0), vec3(0,0,0));
	m_env->addCamera("front", camera2);
	// Create basic Camera
	std::shared_ptr<ege::camera::View> camera3 = std::make_shared<ege::camera::View>(vec3(20,20,100), vec3(0,0,0));
	m_env->addCamera("top", camera3);
	// Create basic Camera
	std::shared_ptr<ege::camera::View> camera4 = std::make_shared<ege::camera::View>(vec3(0,100,0), vec3(0,0,0));
	m_env->addCamera("left", camera4);
	
	std::shared_ptr<ewol::widget::Sizer> tmpSizerVert = ewol::widget::Sizer::create();
	if (tmpSizerVert == nullptr) {
		APPL_CRITICAL("Can not allocate widget ==> display might be in error");
	} else {
		tmpSizerVert->propertyMode.set(ewol::widget::Sizer::modeVert);
		setSubWidget(tmpSizerVert);
		std::shared_ptr<ege::widget::Scene> tmpWidget = ege::widget::Scene::create();
		if (tmpWidget == nullptr) {
			APPL_CRITICAL("Can not allocate widget ==> display might be in error");
		} else {
			tmpWidget->setEnv(m_env);
			tmpWidget->propertyExpand.set(bvec2(true,true));
			tmpWidget->propertyFill.set(bvec2(true,true));
			tmpWidget->setCamera("basic");
			tmpSizerVert->subWidgetAdd(tmpWidget);
		}
		std::shared_ptr<ewol::widget::Sizer> tmpSizerHori = ewol::widget::Sizer::create();
		if (tmpSizerHori == nullptr) {
			APPL_CRITICAL("Can not allocate widget ==> display might be in error");
		} else {
			tmpSizerHori->propertyMode.set(ewol::widget::Sizer::modeHori);
			tmpSizerVert->subWidgetAdd(tmpSizerHori);
			tmpWidget = ege::widget::Scene::create();
			if (tmpWidget == nullptr) {
				APPL_CRITICAL("Can not allocate widget ==> display might be in error");
			} else {
				tmpWidget->setEnv(m_env);
				tmpWidget->propertyExpand.set(bvec2(true,true));
				tmpWidget->propertyFill.set(bvec2(true,true));
				tmpWidget->setCamera("front");
				tmpSizerHori->subWidgetAdd(tmpWidget);
			}
			tmpWidget = ege::widget::Scene::create();
			if (tmpWidget == nullptr) {
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
	
	std::shared_ptr<ege::resource::Mesh> myMesh;
	// Create an external box :
	myMesh = createViewBoxStar();
	if (myMesh != nullptr) {
		m_env->addStaticMeshToDraw(myMesh);
	}
	// create basic gird:
	myMesh = ege::resource::Mesh::createGrid(10, vec3(0,0,0), 5);
	if (myMesh != nullptr) {
		m_env->addStaticMeshToDraw(myMesh);
	}
	myMesh = ege::resource::Mesh::createCube(3);
	if (myMesh != nullptr) {
		//std::shared_ptr<ege::ElementBase> element = std::make_shared<ege::ElementBase>(m_env);
		std::shared_ptr<ege::ElementPhysic> element = std::make_shared<ege::ElementPhysic>(m_env);
		// add physic interface:
		std::shared_ptr<ege::PhysicsBox> physic = std::make_shared<ege::PhysicsBox>();
		physic->setSize(vec3(3.2,3.2,3.2));
		myMesh->addPhysicElement(physic);
		
		element->setMesh(myMesh);
		element->createRigidBody(4000000);
		element->setPosition(vec3(20,10,10));
		
		m_env->addElement(element);
	}
	myMesh = ege::resource::Mesh::createCube(3);
	if (myMesh != nullptr) {
		//element = std::make_shared<ege::ElementBase>(m_env);
		std::shared_ptr<ege::ElementPhysic> element = std::make_shared<ege::ElementPhysic>(m_env);
		
		// add physic interface:
		std::shared_ptr<ege::PhysicsSphere> physic = std::make_shared<ege::PhysicsSphere>();
		physic->setRadius(4.5f);
		myMesh->addPhysicElement(physic);
		
		
		element->setMesh(myMesh);
		element->createRigidBody(4000000);
		element->setPosition(vec3(20,-10,10));
		
		element->iaEnable();
		
		m_env->addElement(element);
		
	}
}

bool appl::Windows::onEventInput(const ewol::event::Input& _event) {
	static float ploppp=1;
	if (_event.getId() == 4) {
		ploppp += 0.01f;
		m_camera->setEye(vec3(100*std::sin(m_angleTetha),100*std::cos(m_angleTetha),80*std::cos(m_anglePsy))*ploppp);
	} else if (_event.getId() == 5) {
		ploppp -= 0.01f;
		if (ploppp == 0) {
			ploppp = 1.0f;
		}
		m_camera->setEye(vec3(100*std::sin(m_angleTetha),100*std::cos(m_angleTetha),80*std::cos(m_anglePsy))*ploppp);
	} else if (_event.getId() == 3) {
		if (_event.getStatus() == gale::key::status_down) {
			m_oldScreenPos = relativePosition(_event.getPos());
			return true;
		} else if (_event.getStatus() == gale::key::status_move) {
			vec2 pos = relativePosition(_event.getPos());
			m_angleTetha -= (m_oldScreenPos.x()-pos.x())*0.05f;
			m_anglePsy += (m_oldScreenPos.y()-pos.y())*0.01f;
			m_camera->setEye(vec3(100*std::sin(m_angleTetha),100*std::cos(m_angleTetha),80*std::cos(m_anglePsy))*ploppp);
			m_oldScreenPos = relativePosition(_event.getPos());
			return true;
		}
	} else if (_event.getId() == 2) {
		if (_event.getStatus() == gale::key::status_down) {
			m_oldScreenPos = relativePosition(_event.getPos());
			return true;
		} else if (_event.getStatus() == gale::key::status_move) {
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

