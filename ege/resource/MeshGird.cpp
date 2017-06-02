/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/resource/Mesh.hpp>

ememory::SharedPtr<ege::resource::Mesh> ege::resource::Mesh::createGrid(int32_t _lineCount, const vec3& _position, float _size, const std::string& _materialName) {
	ememory::SharedPtr<ege::resource::Mesh> out = ege::resource::Mesh::create("---", "DATA:color3.prog");
	if (out != nullptr) {
		ememory::SharedPtr<ege::Material> material = ememory::makeShared<ege::Material>();
		// set the element material properties :
		material->setAmbientFactor(vec4(1,1,1,1));
		material->setDiffuseFactor(vec4(0,0,0,1));
		material->setSpecularFactor(vec4(0,0,0,1));
		material->setShininess(1);
		material->setRenderMode(gale::openGL::renderMode::line);
		//material->setRenderMode(gale::openGL::renderMode::point);
		out->addMaterial(_materialName, material);
		
		out->addFaceIndexing(_materialName);
		// create X lines
		for (int32_t iii=-_lineCount; iii<=_lineCount; ++iii) {
			if (iii==0) {
				out->addLine(_materialName, _position+vec3(-_lineCount,0,0)*_size, _position+vec3(_lineCount+1,0,0)*_size, etk::color::red);
				// arrow
				out->addLine(_materialName, _position+vec3(_lineCount+1.0f,0,0)*_size, _position+vec3(_lineCount+0.5f,0.5f,0)*_size, etk::color::red);
				out->addLine(_materialName, _position+vec3(_lineCount+1.0f,0,0)*_size, _position+vec3(_lineCount+0.5f,-0.5f,0)*_size, etk::color::red);
				out->addLine(_materialName, _position+vec3(_lineCount+1.0f,0,0)*_size, _position+vec3(_lineCount+0.5f,0,0.5f)*_size, etk::color::red);
				out->addLine(_materialName, _position+vec3(_lineCount+1.0f,0,0)*_size, _position+vec3(_lineCount+0.5f,0,-0.5f)*_size, etk::color::red);
				// X letter
				out->addLine(_materialName, _position+vec3(_lineCount+2.0f,1.0f,-1.0f)*_size, _position+vec3(_lineCount+2.0f,-1.0f,1.0f)*_size, etk::color::red);
				out->addLine(_materialName, _position+vec3(_lineCount+2.0f,-1.0f,-1.0f)*_size, _position+vec3(_lineCount+2.0f,1.0f,1.0f)*_size, etk::color::red);
			} else {
				out->addLine(_materialName, _position+vec3(-_lineCount,iii,0)*_size, _position+vec3(_lineCount,iii,0)*_size, etk::color::gray);
			}
			//out->addPoint(_materialName, vec3(-_lineCount,iii,0), etk::color::white);
			//out->addPoint(_materialName, vec3(_lineCount,iii,0), etk::color::white);
		}
		// create Y lines
		for (int32_t iii=-_lineCount; iii<=_lineCount; ++iii) {
			if (iii==0) {
				out->addLine(_materialName, _position+vec3(0,-_lineCount,0)*_size, _position+vec3(0,_lineCount+1,0)*_size, etk::color::green);
				// arrow
				out->addLine(_materialName, _position+vec3(0,_lineCount+1.0f,0)*_size, _position+vec3(0.5f,_lineCount+0.5f,0)*_size, etk::color::green);
				out->addLine(_materialName, _position+vec3(0,_lineCount+1.0f,0)*_size, _position+vec3(-0.5f,_lineCount+0.5f,0)*_size, etk::color::green);
				out->addLine(_materialName, _position+vec3(0,_lineCount+1.0f,0)*_size, _position+vec3(0,_lineCount+0.5f,0.5f)*_size, etk::color::green);
				out->addLine(_materialName, _position+vec3(0,_lineCount+1.0f,0)*_size, _position+vec3(0,_lineCount+0.5f,-0.5f)*_size, etk::color::green);
				// Y letter
				out->addLine(_materialName, _position+vec3(0,_lineCount+2.0f,0)*_size, _position+vec3(0.7f,_lineCount+2.0f,1.0f)*_size, etk::color::green);
				out->addLine(_materialName, _position+vec3(0,_lineCount+2.0f,0)*_size, _position+vec3(-0.7f,_lineCount+2.0f,1.0f)*_size, etk::color::green);
				out->addLine(_materialName, _position+vec3(0,_lineCount+2.0f,0)*_size, _position+vec3(0,_lineCount+2.0f,-1.0f)*_size, etk::color::green);
			} else {
				out->addLine(_materialName, _position+vec3(iii,-_lineCount,0)*_size, _position+vec3(iii,_lineCount,0)*_size, etk::color::gray);
			}
			//out->addPoint(_materialName, vec3(iii,-_lineCount,0), etk::color::white);
			//out->addPoint(_materialName, vec3(iii,_lineCount,0), etk::color::white);
		}
		// create Z lines
		for (int32_t iii=-_lineCount; iii<=_lineCount; ++iii) {
			if (iii==0) {
				out->addLine(_materialName, _position+vec3(0,0,-_lineCount)*_size, _position+vec3(0,0,_lineCount+1)*_size, etk::color::blue);
				// arrow
				out->addLine(_materialName, _position+vec3(0,0,_lineCount+1)*_size, _position+vec3(0.5f,0,_lineCount+0.5f)*_size, etk::color::blue);
				out->addLine(_materialName, _position+vec3(0,0,_lineCount+1)*_size, _position+vec3(-0.5f,0,_lineCount+0.5f)*_size, etk::color::blue);
				out->addLine(_materialName, _position+vec3(0,0,_lineCount+1)*_size, _position+vec3(0,0.5f,_lineCount+0.5f)*_size, etk::color::blue);
				out->addLine(_materialName, _position+vec3(0,0,_lineCount+1)*_size, _position+vec3(0,-0.5f,_lineCount+0.5f)*_size, etk::color::blue);
				// Z letter
				out->addLine(_materialName, _position+vec3( 1,-1,_lineCount+2.0f)*_size, _position+vec3( 1, 1,_lineCount+2.0f)*_size, etk::color::blue);
				out->addLine(_materialName, _position+vec3( 1, 1,_lineCount+2.0f)*_size, _position+vec3(-1,-1,_lineCount+2.0f)*_size, etk::color::blue);
				out->addLine(_materialName, _position+vec3(-1,-1,_lineCount+2.0f)*_size, _position+vec3(-1, 1,_lineCount+2.0f)*_size, etk::color::blue);
				
			} else {
				std::vector<vec3> list;
				list.push_back(_position+vec3(-1,-1,iii)*_size);
				list.push_back(_position+vec3(1,-1,iii)*_size);
				list.push_back(_position+vec3(1,1,iii)*_size);
				list.push_back(_position+vec3(-1,1,iii)*_size);
				list.push_back(_position+vec3(-1,-1,iii)*_size);
				out->addLines(_materialName, list, etk::color::gray);
			}
			//out->addPoint(_materialName, vec3(iii,-_lineCount,0), etk::color::white);
			//out->addPoint(_materialName, vec3(iii,_lineCount,0), etk::color::white);
		}
		
		// generate the VBO
		out->generateVBO();
	}
	return out;
}
