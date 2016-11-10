/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license APACHE-2 (see license file)
 */

#include <ewol/ewol.hpp>
#include <appl/debug.hpp>
#include <appl/Windows.hpp>
#include <ewol/widget/Label.hpp>
#include <ewol/object/Manager.hpp>
#include <ege/widget/Scene.hpp>
#include <ege/camera/View.hpp>
#include <etk/tool.hpp>
#include <ege/elements/ElementBase.hpp>
#include <ege/elements/ElementPhysic.hpp>
#include <ege/physicsShape/PhysicsBox.hpp>
#include <ege/physicsShape/PhysicsSphere.hpp>

appl::Windows::Windows() :
  m_angleTetha(0),
  m_anglePsy(0) {
	addObjectType("appl::Windows");
	propertyTitle.setDirectCheck("example ege: RayTest");
}

static ememory::SharedPtr<ege::resource::Mesh> createViewBoxStar() {
	ememory::SharedPtr<ege::resource::Mesh> out = ege::resource::Mesh::create("viewBoxStar", "DATA:texturedNoMaterial.prog");
	if (out != nullptr) {
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
	m_camera = ememory::makeShared<ege::camera::View>(vec3(30,30,-100), vec3(0,0,0));
	m_camera->setEye(vec3(100*std::sin(m_angleTetha),100*std::cos(m_angleTetha),40*std::cos(m_anglePsy)));
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
		tmpWidget->signalDisplayDebug.connect(sharedFromThis(), &appl::Windows::onCallbackDisplayDebug);
	}
	ememory::SharedPtr<ege::resource::Mesh> myMesh;
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
		//ememory::SharedPtr<ege::ElementBase> element = ememory::makeShared<ege::ElementBase>(m_env);
		ememory::SharedPtr<ege::ElementPhysic> element = ememory::makeShared<ege::ElementPhysic>(m_env);
		// add physic interface:
		ememory::SharedPtr<ege::PhysicsBox> physic = ememory::makeShared<ege::PhysicsBox>();
		physic->setSize(vec3(3.2,3.2,3.2));
		myMesh->addPhysicElement(physic);
		
		element->setMesh(myMesh);
		element->createRigidBody(4000000);
		element->setPosition(vec3(20,10,10));
		
		m_env->addElement(element);
	}
	myMesh = ege::resource::Mesh::createCube(3);
	if (myMesh != nullptr) {
		//element = ememory::makeShared<ege::ElementBase>(m_env);
		ememory::SharedPtr<ege::ElementPhysic> element = ememory::makeShared<ege::ElementPhysic>(m_env);
		
		// add physic interface:
		ememory::SharedPtr<ege::PhysicsSphere> physic = ememory::makeShared<ege::PhysicsSphere>();
		physic->setRadius(4.5f);
		myMesh->addPhysicElement(physic);
		
		
		element->setMesh(myMesh);
		element->createRigidBody(4000000);
		element->setPosition(vec3(20,-10,10));
		
		element->iaEnable();
		
		m_env->addElement(element);
		
	}
	m_env->propertyStatus.set(ege::gameStart);
}

bool appl::Windows::onEventInput(const ewol::event::Input& _event) {
	static float ploppp=1;
	if (_event.getId() == 1) {
		vec2 pos = relativePosition(_event.getPos());
		ege::Ray ray = m_camera->getRayFromScreenPosition(pos, m_size);
		m_ray = ray;
		APPL_DEBUG("pos=" << pos << " ray = " << ray);
		m_destination = ray.testRay(m_env->getPhysicEngine());
		std::pair<ememory::SharedPtr<ege::Element>, std::pair<vec3,vec3>> result = ray.testRayObject(m_env->getPhysicEngine());
		if (result.first != nullptr) {
			APPL_INFO("Select Object :" << result.first->getUID());
		}
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
		if (_event.getStatus() == gale::key::status::down) {
			m_oldScreenPos = relativePosition(_event.getPos());
			return true;
		} else if (_event.getStatus() == gale::key::status::move) {
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

void appl::Windows::onCallbackDisplayDebug(const ememory::SharedPtr<ewol::resource::Colored3DObject>& _obj) {
	ememory::SharedPtr<ewol::resource::Colored3DObject> obj = ememory::constPointerCast<ewol::resource::Colored3DObject>(_obj);
	mat4 mat;
	mat.identity();
	// Display ray line
	if (true) {
		static std::vector<vec3> vertices;
		if (m_ray.getOrigin() != vec3(0,0,0)) {
			vertices.push_back(m_ray.getOrigin());
			vertices.push_back(m_ray.getOrigin()+m_ray.getDirection()*50);
			// prevent Ray removing with empty
			m_ray.setOrigin(vec3(0,0,0));
		}
		if (vertices.size() > 250) {
			vertices.erase(vertices.begin(), vertices.begin()+vertices.size()-250);
		}
		obj->drawLine(vertices, etk::Color<float>(0.0, 1.0, 0.0, 0.8), mat);
	}
	// display normal impact line
	if (true) {
		static std::vector<vec3> vertices;
		if (m_destination.second != vec3(0,0,0)) {
			vertices.push_back(m_destination.first);
			vertices.push_back(m_destination.first + m_destination.second*20);
			m_destination.second = vec3(0,0,0);
		}
		if (vertices.size() > 250) {
			vertices.erase(vertices.begin(), vertices.begin()+vertices.size()-250);
		}
		obj->drawLine(vertices, etk::Color<float>(1.0, 0.0, 0.0, 0.8), mat);
	}
}

