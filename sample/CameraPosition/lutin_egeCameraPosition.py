#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import datetime


def get_type():
	return "BINARY"

def get_sub_type():
	return "SAMPLE"

def get_name():
	return "ege-sample-CameraPosition"

def get_desc():
	return "ege sample : CameraPisition"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def create(target, module_name):
	# module name is 'edn' and type binary.
	my_module = module.Module(__file__, module_name, get_type())
	
	my_module.add_src_file([
		'appl/debug.cpp',
		'appl/main.cpp',
		'appl/Windows.cpp'
		])
	
	my_module.add_module_depend('ege')
	
	my_module.add_path(tools.get_current_path(__file__))
	
	my_module.copy_path("data/*")
	
	# set the package properties :
	my_module.pkg_set("VERSION", "0.0.0")
	my_module.pkg_set("VERSION_CODE", "0")
	my_module.pkg_set("COMPAGNY_TYPE", get_compagny_type())
	my_module.pkg_set("COMPAGNY_NAME", get_compagny_name())
	my_module.pkg_set("MAINTAINER", get_maintainer())
	my_module.pkg_set("SECTION", ["Game"])
	my_module.pkg_set("PRIORITY", "optional")
	my_module.pkg_set("DESCRIPTION", get_desc())
	my_module.pkg_set("NAME", get_name())
	
	# add the currrent module at the 
	return my_module

