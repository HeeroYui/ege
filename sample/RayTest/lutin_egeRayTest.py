#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import datetime


def get_desc():
	return "ege sample : RayTest"

def create(target):
	# module name is 'edn' and type binary.
	myModule = module.Module(__file__, 'egeRayTest', 'PACKAGE')
	
	myModule.add_src_file([
		'appl/debug.cpp',
		'appl/main.cpp',
		'appl/Windows.cpp'
		])
	
	myModule.add_module_depend('ege')
	
	myModule.add_path(tools.get_current_path(__file__))
	
	myModule.copy_folder("data/*")
	
	# set the package properties :
	myModule.pkg_set("VERSION", "0.0.0")
	myModule.pkg_set("VERSION_CODE", "0")
	myModule.pkg_set("COMPAGNY_TYPE", "org")
	myModule.pkg_set("COMPAGNY_NAME", "ege")
	myModule.pkg_set("MAINTAINER", ["noOne <no.one@noreplay.com>"])
	myModule.pkg_set("SECTION", ["Game"])
	myModule.pkg_set("PRIORITY", "optional")
	myModule.pkg_set("DESCRIPTION", "ege sample : Ray test")
	myModule.pkg_set("NAME", "egeRayTest")
	
	# add the currrent module at the 
	return myModule

