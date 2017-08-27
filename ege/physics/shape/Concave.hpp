/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once


#include <etk/types.hpp>
#include <ege/physics/shape/Shape.hpp>


namespace ege {
	namespace physics {
		namespace shape {
			class Concave : public ege::physics::Shape {
				public:
					Concave() {};
					virtual ~Concave() {};
				public:
					virtual bool parse(const char* _line);
					virtual void display() {};
				public:
					virtual enum ege::physics::Shape::type getType() const {
						return ege::physics::Shape::type::concave;
					}
				public:
					virtual const ege::physics::shape::Concave* toConcave() const {
						return this;
					}
					virtual ege::physics::shape::Concave* toConcave() {
						return this;
					}
				private:
					etk::Vector<vec3> m_listVertex;
					etk::Vector<uint32_t> m_indices;
				public:
					void clear() {
						m_listVertex.clear();
						m_indices.clear();
					}
					void setListOfVertex(const etk::Vector<vec3>& _listVertex) {
						m_listVertex = _listVertex;
					}
					void addTriangle(const etk::Vector<uint32_t>& _index) {
						/*
						if (m_indices.size() == 0) {
							m_indices = _index;
							return;
						}
						*/
						if (_index.size()%3 != 0 ) {
							EGE_ERROR("wrong number of faces : " << _index.size() << " ==> not a multiple of 3");
							return;
						}
						for (auto &it: _index) {
							m_indices.pushBack(it);
						}
					}
					const etk::Vector<vec3>& getVertex() const {
						return m_listVertex;
					}
					const etk::Vector<uint32_t>& getIndices() const {
						return m_indices;
					}
			};
		}
	}
}

