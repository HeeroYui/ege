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
#include <ege/widget/Scene.h>
#include <etk/tool.h>

#undef __class__
#define __class__ "Windows"

appl::Windows::Windows() {
	addObjectType("appl::Windows");
}
void appl::Windows::init() {
	ewol::widget::Windows::init();
	setTitle("example ege : MeshCreator");
	
	m_env = ege::Environement::create();
	// Create basic Camera
	m_env->addCamera("basic", std::make_shared<ege::Camera>());
	
	std::shared_ptr<ege::widget::Scene> tmpWidget = ege::widget::Scene::create(m_env);
	if (tmpWidget == nullptr) {
		APPL_ERROR("Can not allocate widget ==> display might be in error");
	} else {
		tmpWidget->setExpand(bvec2(true,true));
		tmpWidget->setFill(bvec2(true,true));
		tmpWidget->setCamera("basic");
		setSubWidget(tmpWidget);
	}
	// Create an external box :
	std::shared_ptr<ege::resource::Mesh> myMesh = ege::resource::Mesh::create("---", "DATA:texturedNoMaterial.prog");
	if (myMesh != nullptr) {
		std::shared_ptr<ege::Material> material = std::make_shared<ege::Material>();
		// set the element material properties :
		material->setAmbientFactor(vec4(1,1,1,1));
		material->setDiffuseFactor(vec4(0,0,0,1));
		material->setSpecularFactor(vec4(0,0,0,1));
		material->setShininess(1);
		material->setTexture0(""); //"
		myMesh->addMaterial("basics", material);
		// 1024  == > 1<<9
		// 2048  == > 1<<10
		// 4096  == > 1<<11
		int32_t size = 1<<11;
		material->setImageSize(ivec2(size,size));
		egami::Image* myImage = material->get();
		if (nullptr == myImage) {
			return;
		}
		myImage->clear(etk::color::black);
		ivec2 tmpPos;
		for (int32_t iii=0; iii<6000; iii++) {
			tmpPos.setValue(etk::tool::frand(0,size), etk::tool::frand(0,size)) ;
			myImage->set(tmpPos, etk::color::white);
		}
		material->flush();
		// basis on cube :
		myMesh->createViewBox("basics", 1000/* distance */);
		// generate the VBO
		myMesh->generateVBO();
		m_env->addStaticMeshToDraw(myMesh);
	}
	myMesh = ege::resource::Mesh::create("---");
	if (myMesh != nullptr) {
		std::shared_ptr<ege::Material> material = std::make_shared<ege::Material>();
		material->setAmbientFactor(vec4(0.112f,0.112f,0.112f,1.0f));
		material->setDiffuseFactor(vec4(0.512f,0.512f,0.512f,1.0f));
		material->setSpecularFactor(vec4(0.5f,0.5f,0.5f,1.0f));
		material->setShininess(96.078431f);
		material->setTexture0("DATA:texture_mars.png");
		myMesh->addMaterial("basics", material);
		myMesh->createIcoSphere("basics", 16, 3);
		myMesh->generateVBO();
		m_env->addStaticMeshToDraw(myMesh);
	}
}
