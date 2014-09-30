/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/resource/tools/icoSphere.h>

// return index of point in the middle of p1 and p2
static int32_t getMiddlePoint(std::vector<vec3>& _listVertex, int32_t _p1, int32_t _p2) {
	vec3 middle = (_listVertex[_p1] + _listVertex[_p2])*0.5f;
	// check if this point exist:
	int32_t id=0;
	for (auto &point : _listVertex) {
		if (middle == point) {
			return id;
		}
		id++;
	}
	_listVertex.push_back(middle.normalize());
	return _listVertex.size()-1;
}

void ege::icoSphere::create(etk::Hash<ege::Material*>& _materials, etk::Hash<FaceIndexing>& _listFaces, std::vector<vec3>& _listVertex, std::vector<vec2>& _listUV,
                            const std::string& _materialName, int32_t _recursionLevel) {
	/*
	                                     5                                                  
	                                    o                                                   
	                                   /|                                                   
	                                  / |                                                   
	                                 /  |                                                   
	                                /   |                                                   
	                   11          /    |       9                                           
	      Z              o--------/...---------0   1                                        
	      |    y         |      4o o....|......|--o                                         
	      |   /          |       |. 0   |      | /                                          
	      |  /           |      .|      .      |/                                           
	      | /            |     . |      .      |                                            
	      |/             |    /  |      .     .|                                            
	      o-------x      |   /   |      .    / |                                            
	                     o--/....|........../--o                                            
	                   10  /     |     7o  /    8                                           
	                      /      .     .  /                                                 
	                     /       .    .  /                                                  
	                    o---------------o                                                   
	                   2         |  /    3                                                  
	                             | /                                                        
	                             |/                                                         
	                             o                                                          
	                            6                                                           
	*/
	// create 12 vertices of a icosahedron
	double size = 1.0;
	double ttt = (1.0 + sqrt(5.0)) / 2.0*size;
	EGE_ERROR("podition : " << ttt);
	_listVertex.push_back(vec3(-size,  ttt,  0.0).normalize()); // 0
	_listVertex.push_back(vec3( size,  ttt,  0.0).normalize()); // 1
	_listVertex.push_back(vec3(-size, -ttt,  0.0).normalize()); // 2
	_listVertex.push_back(vec3( size, -ttt,  0.0).normalize()); // 3

	_listVertex.push_back(vec3( 0.0, -1,  ttt).normalize()); // 4
	_listVertex.push_back(vec3( 0.0,  1,  ttt).normalize()); // 5
	_listVertex.push_back(vec3( 0.0, -1, -ttt).normalize()); // 6
	_listVertex.push_back(vec3( 0.0,  1, -ttt).normalize()); // 7

	_listVertex.push_back(vec3( ttt,  0.0, -size).normalize()); // 8
	_listVertex.push_back(vec3( ttt,  0.0,  size).normalize()); // 9
	_listVertex.push_back(vec3(-ttt,  0.0, -size).normalize()); // 10
	_listVertex.push_back(vec3(-ttt,  0.0,  size).normalize()); // 11

	// _listUV ==> TODO : very bad code ... get from viewBox ....
	_listUV.push_back(vec2(0.0, 0.0    )); // 0
	_listUV.push_back(vec2(0.0, 1.0    )); // 1
	_listUV.push_back(vec2(1.0, 0.0    )); // 2
	
	for (auto &elem : _listVertex) {
		EGE_INFO("plop : " << etk::to_string(elem));
	}
	
	if (_listFaces.exist(_materialName) == false) {
		FaceIndexing empty;
		_listFaces.add(_materialName, empty);
	}
	{
		FaceIndexing& tmpElement = _listFaces[_materialName];
		// 5 faces around point 0
		tmpElement.m_faces.push_back(Face(0,0, 11,1,  5,2));
		tmpElement.m_faces.push_back(Face(0,0,  5,1,  1,2));
		tmpElement.m_faces.push_back(Face(0,0,  1,1,  7,2));
		tmpElement.m_faces.push_back(Face(0,0,  7,1, 10,2));
		tmpElement.m_faces.push_back(Face(0,0, 10,1, 11,2));
		
		// 5 adjacent faces
		tmpElement.m_faces.push_back(Face( 1,0,  5,1, 9,2));
		tmpElement.m_faces.push_back(Face( 5,0, 11,1, 4,2));
		tmpElement.m_faces.push_back(Face(11,0, 10,1, 2,2));
		tmpElement.m_faces.push_back(Face(10,0,  7,1, 6,2));
		tmpElement.m_faces.push_back(Face( 7,0,  1,1, 8,2));
		
		// 5 faces around point 3
		tmpElement.m_faces.push_back(Face(3,0, 9,1, 4,2));
		tmpElement.m_faces.push_back(Face(3,0, 4,1, 2,2));
		tmpElement.m_faces.push_back(Face(3,0, 2,1, 6,2));
		tmpElement.m_faces.push_back(Face(3,0, 6,1, 8,2));
		tmpElement.m_faces.push_back(Face(3,0, 8,1, 9,2));
		
		// 5 adjacent faces 
		tmpElement.m_faces.push_back(Face(4,0, 9,1, 5,2));
		tmpElement.m_faces.push_back(Face(2,0, 4,1, 11,2));
		tmpElement.m_faces.push_back(Face(6,0, 2,1, 10,2));
		tmpElement.m_faces.push_back(Face(8,0, 6,1, 7,2));
		tmpElement.m_faces.push_back(Face(9,0, 8,1, 1,2));
		// refine triangles
		for (int i = 0; i < _recursionLevel; i++) {
			std::vector<Face> listFaces;
			for (auto &tri : tmpElement.m_faces) {
				// replace triangle by 4 triangles
				int32_t a = getMiddlePoint(_listVertex, tri.m_vertex[0], tri.m_vertex[1]);
				int32_t b = getMiddlePoint(_listVertex, tri.m_vertex[1], tri.m_vertex[2]);
				int32_t c = getMiddlePoint(_listVertex, tri.m_vertex[2], tri.m_vertex[0]);
				listFaces.push_back(Face(tri.m_vertex[0],0, a,1, c,2));
				listFaces.push_back(Face(tri.m_vertex[1],0, b,1, a,2));
				listFaces.push_back(Face(tri.m_vertex[2],0, c,1, b,2));
				listFaces.push_back(Face(a,0,               b,1, c,2));
			}
			tmpElement.m_faces = listFaces;
		}
	}
}

