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
}
