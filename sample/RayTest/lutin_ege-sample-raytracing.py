#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import datetime


def get_type():
	return "BINARY"

def get_sub_type():
	return "TEST"

def get_name():
	return "ege-test-ray"

def get_desc():
	return "ege sample : RayTest"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def get_version():
	return [0,1]

def create(target, module_name):
	# module name is 'edn' and type binary.
	my_module = module.Module(__file__, module_name, get_type())
	
	my_module.add_src_file([
		'appl/debug.cpp',
		'appl/main.cpp',
		'appl/Windows.cpp'
		])
	
	my_module.add_depend('ege')
	
	my_module.add_path(tools.get_current_path(__file__))
	
	my_module.copy_path("data/*")
	
	my_module.add_flag('c++', [
		"-DPROJECT_NAME=\"\\\"" + my_module.get_name() + "\\\"\"",
		"-DAPPL_VERSION=\"\\\"" + tools.version_to_string(get_version()) + "\\\"\""
		])
	
	# set the package properties:
	my_module.set_pkg("VERSION_CODE", "0")
	my_module.set_pkg("COMPAGNY_TYPE", get_compagny_type())
	my_module.set_pkg("COMPAGNY_NAME", get_compagny_name())
	my_module.set_pkg("MAINTAINER", get_maintainer())
	my_module.set_pkg("SECTION", ["Development"])
	my_module.set_pkg("PRIORITY", "optional")
	my_module.set_pkg("DESCRIPTION", get_desc())
	my_module.set_pkg("NAME", get_name())
	return my_module

