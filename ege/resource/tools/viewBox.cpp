/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/resource/tools/viewBox.hpp>



void ege::viewBox::create(etk::Hash<ememory::SharedPtr<ege::Material>>& _materials, etk::Hash<FaceIndexing>& _listFaces, std::vector<vec3>& _listVertex, std::vector<vec2>& _listUV,
                          const std::string& _materialName, float _size) {
	// This is the direct generation basis on the .obj system
	/*
			           5                       6  
			            o---------------------o   
			           /.                    /|   
			          / .                   / |   
			         /  .                  /  |   
			        /   .                 /   |   
			       /    .                /    |   
			    4 /     .               /     |   
			     o---------------------o      |   
			     |      .              |7     |   
			     |      .              |      |   
			     |      .              |      |   
			     |      .              |      |   
			     |      o..............|......o   
			     |     . 1             |     / 2  
			     |    .                |    /     
			     |   .                 |   /      
			     |  .                  |  /       
			     | .                   | /        
			     |.                    |/         
			     o---------------------o          
			    0                       3         
	*/
	_listVertex.push_back(vec3( _size, -_size, -_size)); // 0
	_listVertex.push_back(vec3( _size, -_size,  _size)); // 1
	_listVertex.push_back(vec3(-_size, -_size,  _size)); // 2
	_listVertex.push_back(vec3(-_size, -_size, -_size)); // 3
	_listVertex.push_back(vec3( _size,  _size, -_size)); // 4
	_listVertex.push_back(vec3( _size,  _size,  _size)); // 5
	_listVertex.push_back(vec3(-_size,  _size,  _size)); // 6
	_listVertex.push_back(vec3(-_size,  _size, -_size)); // 7
	/*
		     o----------o----------o----------o
		     |8         |9         |10        |11
		     |          |          |          |
		     |          |          |          |
		     |    0     |    1     |    2     |
		     |          |          |          |
		     |          |          |          |
		     |          |          |          |
		     |          |          |          |
		     o----------o----------o----------o
		     |4         |5         |6         |7
		     |          |          |          |
		     |          |          |          |
		     |    3     |    4     |    5     |
		     |          |          |          |
		     |          |          |          |
		     |          |          |          |
		     |          |          |          |
		     o----------o----------o----------o
		     0          1          2          3
	*/
	_listUV.push_back(vec2(0.0    , 0.0    )); // 0
	_listUV.push_back(vec2(1.0/3.0, 0.0    )); // 1
	_listUV.push_back(vec2(2.0/3.0, 0.0    )); // 2
	_listUV.push_back(vec2(1.0    , 0.0    )); // 3
	_listUV.push_back(vec2(0.0    , 0.5    )); // 4
	_listUV.push_back(vec2(1.0/3.0, 0.5    )); // 5
	_listUV.push_back(vec2(2.0/3.0, 0.5    )); // 6
	_listUV.push_back(vec2(1.0    , 0.5    )); // 7
	_listUV.push_back(vec2(0.0    , 1.0    )); // 8
	_listUV.push_back(vec2(1.0/3.0, 1.0    )); // 9
	_listUV.push_back(vec2(2.0/3.0, 1.0    )); // 10
	_listUV.push_back(vec2(1.0    , 1.0    )); // 11
	
	if (_listFaces.exist(_materialName) == false) {
		FaceIndexing empty;
		_listFaces.add(_materialName, empty);
	}
	{
		FaceIndexing& tmpElement = _listFaces[_materialName];
		tmpElement.m_faces.push_back(Face(0,1, 1,5,  2,6)); // 4
		tmpElement.m_faces.push_back(Face(0,1, 2,6,  3,2)); // 4
		tmpElement.m_faces.push_back(Face(4,4, 0,0,  3,1)); // 3
		tmpElement.m_faces.push_back(Face(4,4, 3,1,  7,5)); // 3
		tmpElement.m_faces.push_back(Face(2,6, 6,10, 7,11)); // 2
		tmpElement.m_faces.push_back(Face(2,6, 7,11, 3,7)); // 2
		tmpElement.m_faces.push_back(Face(4,2, 7,3,  6,7)); // 5
		tmpElement.m_faces.push_back(Face(4,2, 6,7,  5,6)); // 5
		tmpElement.m_faces.push_back(Face(1,5, 5,9,  6,10)); // 1
		tmpElement.m_faces.push_back(Face(1,5, 6,10, 2,6)); // 1
		tmpElement.m_faces.push_back(Face(0,4, 4,8,  5,9)); // 0
		tmpElement.m_faces.push_back(Face(0,4, 5,9,  1,5)); // 0
	}
}
