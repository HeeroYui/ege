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

appl::Windows::Windows() :
  m_angleTetha(0),
  m_anglePsy(0) {
	addObjectType("appl::Windows");
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
	setTitle("example ege : RayTest");
	
	getObjectManager().periodicCall.bind(shared_from_this(), &appl::Windows::onCallbackPeriodicUpdateCamera);
	
	m_env = ege::Environement::create();
	// Create basic Camera
	m_camera = std::make_shared<ege::camera::View>(vec3(30,30,-100), vec3(0,0,0));
	m_camera->setEye(vec3(100*std::sin(m_angleTetha),100*std::cos(m_angleTetha),40*std::cos(m_anglePsy)));
	m_env->addCamera("basic", m_camera);
	
	std::shared_ptr<ege::widget::Scene> tmpWidget = ege::widget::Scene::create(m_env);
	if (tmpWidget == nullptr) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		tmpWidget->setExpand(bvec2(true,true));
		tmpWidget->setFill(bvec2(true,true));
		tmpWidget->setCamera("basic");
		setSubWidget(tmpWidget);
		tmpWidget->signalDisplayDebug.bind(shared_from_this(), &appl::Windows::onCallbackDisplayDebug);
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
		/*
		std::shared_ptr<ege::PhysicsBox> physic = std::make_shared<ege::PhysicsBox>();
		physic->setSize(vec3(3.2,3.2,3.2));
		*/
		std::shared_ptr<ege::PhysicsSphere> physic = std::make_shared<ege::PhysicsSphere>();
		physic->setRadius(4.2f);
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
		physic->setRadius(3.3f);
		myMesh->addPhysicElement(physic);
		
		
		element->setMesh(myMesh);
		element->createRigidBody(4000000);
		element->setPosition(vec3(20,-10,10));
		
		element->iaEnable();
		
		m_env->addElement(element);
		
	}
}


void appl::Windows::onCallbackPeriodicUpdateCamera(const ewol::event::Time& _event) {
	static float offset = 0;
	offset += 0.01;
	static float offset2 = 0;
	//offset2 += 0.003;
	//m_camera->setEye(vec3(100*std::sin(offset),100*std::cos(offset),40*std::cos(offset2)));
}



bool appl::Windows::onEventInput(const ewol::event::Input& _event) {
	static float ploppp=1;
	if (_event.getId() == 1) {
		vec2 pos = relativePosition(_event.getPos());
		ege::Ray ray = m_camera->getRayFromScreenPosition(pos, m_size);
		m_ray = ray;
		APPL_INFO("pos=" << pos << " ray = " << ray);
		m_destination = ray.testRay(m_env->getPhysicEngine());
		return true;
	} else if (_event.getId() == 4) {
		ploppp += 0.2f;
		m_camera->setEye(vec3(100*std::sin(m_angleTetha),100*std::cos(m_angleTetha),40*std::cos(m_anglePsy))*ploppp);
	} else if (_event.getId() == 5) {
		ploppp -= 0.2f;
		if (ploppp == 0) {
			ploppp = 1.0f;
		}
		m_camera->setEye(vec3(100*std::sin(m_angleTetha),100*std::cos(m_angleTetha),40*std::cos(m_anglePsy))*ploppp);
	} else if (_event.getId() == 3) {
		if (_event.getStatus() == ewol::key::statusDown) {
			m_oldScreenPos = relativePosition(_event.getPos());
			return true;
		} else if (_event.getStatus() == ewol::key::statusMove) {
			vec2 pos = relativePosition(_event.getPos());
			m_angleTetha -= (m_oldScreenPos.x()-pos.x())*0.05f;
			m_anglePsy += (m_oldScreenPos.y()-pos.y())*0.05f;
			m_camera->setEye(vec3(100*std::sin(m_angleTetha),100*std::cos(m_angleTetha),40*std::cos(m_anglePsy))*ploppp);
			m_oldScreenPos = relativePosition(_event.getPos());
			return true;
		}
	}
	return false;
}

void appl::Windows::onCallbackDisplayDebug(const std::shared_ptr<ewol::resource::Colored3DObject>& _obj) {
	etk::Color<float> tmpColor(0.0, 1.0, 0.0, 0.3);
	etk::Color<float> tmpColor2(1.0, 0.0, 0.0, 0.3);
	static std::vector<vec3> vertices;
	static std::vector<vec3> vertices2;
	//vertices.push_back(m_ray.getOrigin());//+vec3(1,0,0));
	//vertices.push_back(vec3(100,0,0));//m_ray.getOrigin() + m_ray.getDirection()*1000);
	//vertices.push_back(m_ray.getOrigin() + m_ray.getDirection()*1000);
	#if 0
		vertices.push_back(vec3(0,0,0));
		vertices.push_back(m_ray.getDirection()*50);
	#else
		vertices.push_back(m_ray.getOrigin());
		vertices.push_back(m_ray.getOrigin()+m_ray.getDirection()*100);
	#endif
	if (vertices.size() > 50) {
		vertices.erase(vertices.begin(), vertices.begin()+vertices.size()-50);
	}
	if (m_destination.second != vec3(0,0,0)) {
		vertices2.push_back(m_destination.first);
		vertices2.push_back(m_destination.first + m_destination.second*100);
		m_destination.second = vec3(0,0,0);
	}
	if (vertices2.size() > 50) {
		vertices2.erase(vertices2.begin(), vertices2.begin()+vertices2.size()-50);
	}
	mat4 mat;
	mat.identity();
	mat.translate(vec3(0,0,0));
	_obj->drawLine(vertices, tmpColor, mat);
	_obj->drawLine(vertices2, tmpColor2, mat);
}

