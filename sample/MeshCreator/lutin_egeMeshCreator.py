#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import datetime


def get_desc():
	return "ege sample : MeshCreator"

def create(target):
	# module name is 'edn' and type binary.
	my_module = module.Module(__file__, 'egeMeshCreator', 'PACKAGE')
	
	my_module.add_src_file([
		'appl/debug.cpp',
		'appl/main.cpp',
		'appl/Windows.cpp'
		])
	
	my_module.add_module_depend('ege')
	
	my_module.add_path(tools.get_current_path(__file__))
	
	my_module.copy_folder("data/*")
	
	# set the package properties :
	my_module.pkg_set("VERSION", "0.0.0")
	my_module.pkg_set("VERSION_CODE", "0")
	my_module.pkg_set("COMPAGNY_TYPE", "org")
	my_module.pkg_set("COMPAGNY_NAME", "ege")
	my_module.pkg_set("MAINTAINER", ["noOne <no.one@noreplay.com>"])
	my_module.pkg_set("SECTION", ["Game"])
	my_module.pkg_set("PRIORITY", "optional")
	my_module.pkg_set("DESCRIPTION", "ege sample : MeshCreator")
	my_module.pkg_set("NAME", "egeMeshCreator")
	
	# add the currrent module at the 
	return my_module

