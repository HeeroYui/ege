#!/usr/bin/python
import lutin.debug as debug
import lutin.tools as tools


def get_type():
	return "BINARY"

def get_sub_type():
	return "SAMPLE"

def get_name():
	return "ege-sample-ia"

def get_desc():
	return "ege sample : Artificial intelligence"

def get_licence():
	return "MPL-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def get_version():
	return [0,1]

def configure(target, my_module):
	my_module.add_extra_flags()
	my_module.add_src_file([
	    'appl/debug.cpp',
	    'appl/main.cpp',
	    'appl/Windows.cpp',
	    ])
	my_module.add_depend('ege')
	my_module.add_path(".")
	my_module.copy_path("data/*.emf")
	my_module.copy_path("data/*.lua")
	my_module.add_flag('c++', [
	    "-DPROJECT_NAME=\"\\\"" + my_module.get_name() + "\\\"\"",
	    "-DAPPL_VERSION=\"\\\"" + tools.version_to_string(get_version()) + "\\\"\"",
	    ])
	# set the package properties:
	my_module.set_pkg("SECTION", ["Game"])
	my_module.set_pkg("PRIORITY", "optional")
	return True

