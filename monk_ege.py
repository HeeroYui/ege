#!/usr/bin/python
import monkModule as module
import monkTools as tools

def get_desc():
	return "EGE : Ewol Game engine (based on bullet lib)"

def create():
	# module name is 'edn' and type binary.
	myModule = module.Module(__file__, 'ege', 'LIBRARY')
	# enable doculentation :
	myModule.set_website("http://heeroyui.github.io/ege/")
	myModule.set_path(tools.get_current_path(__file__) + "/ege/")
	# add the currrent module at the 
	return myModule

