#pragma once

#include <glm/ext.hpp>
#include "Shader.hpp"
#include "VertexTypes.hpp"
#include <map>

namespace Osiris{

	// Base GizmoObject, Inherit from this to create your own
	template <typename TYPE>
	struct GizmoObject{
		~GizmoObject(){
			delete[] Verticies;
			delete[] Indicies;
		}
		unsigned int IndexCount,VertexCount;
		unsigned int *Indicies;
		TYPE *Verticies;
	};

	// VertexBatch
	// Basic Single draw call batch of elements.
	template <typename TYPE>
	class VertexBatch{
	public:

		// Instantiate the Gizmos.
		// Must setup all VertexAtribArrays
		VertexBatch(){
			_NextObjectID = _VertexCount = _IndexCount = _VAO = _VBO = _IBO = 0;
			_sGizmoShader = nullptr;
			_Verticies  = nullptr;
			_Indices = nullptr;

			// Create opengl buffers
			glGenVertexArrays(1, &_VAO);
			glBindVertexArray(_VAO);

			glGenBuffers(1, &_VBO);
			glGenBuffers(1, &_IBO);

			glBindBuffer(GL_ARRAY_BUFFER, _VBO);
			glBufferData(GL_ARRAY_BUFFER, 1048576 * sizeof(TYPE), nullptr, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1048576 * sizeof(unsigned int), nullptr, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		// SetupVertexAttribs
		// ui_ID			= Array Attrib to enable.
		// ui_ElementCount	= Amount of elements to add.
		// gle_Type			= Type of elements being added.
		// glb_Normalized	= To normalize, or not to normalize, that is the question.
		// ui_Offset		= Amount the data is offseted by.
		void EnableVAA(unsigned int ui_ID,unsigned int ui_ElementCount,GLenum gle_Type,GLboolean glb_Normalized,unsigned int ui_Offset){
			glBindVertexArray(_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, _VBO);
			glEnableVertexAttribArray(ui_ID);
			glVertexAttribPointer(ui_ID, ui_ElementCount, gle_Type, glb_Normalized, sizeof(TYPE), (void*) + ui_Offset);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		// Destroy the Gizmos.
		// !WARNING! does NOT clean up the given shader, be sure to delete it.
		// !WARNING! DOES clean up all given GizmoObjects.
		~VertexBatch(){
			glDeleteVertexArrays(1,&_VAO);
			glDeleteBuffers(1, &_VBO);
			glDeleteBuffers(1, &_IBO);

			delete[] _Indices;
			delete[] _Verticies;
			_GizmoObjects.clear();
			_IndexCount = _VertexCount = _NextObjectID = _VAO = _VBO = _IBO = 0;
		}

		// Let the Gizmos know what shader to use.
		void SetShader(Shader* s_GizmoShader){
			_sGizmoShader = s_GizmoShader;
		}

		// Create the given Gizmo.
		// go_ObjectToAdd a pointer to a GizmoObject.
		// return is the ID for this GizmoObject.
		unsigned int Add(GizmoObject<TYPE> *go_ObjectToAdd){
			_GizmoObjects[_NextObjectID] = go_ObjectToAdd;
			_VertexCount	+= _GizmoObjects[_NextObjectID]->VertexCount;
			_IndexCount		+= _GizmoObjects[_NextObjectID]->IndexCount;
			_NextObjectID++;
			return _NextObjectID - 1;
		}

		// Delete the given Gizmo.
		// ui_GizmoID = The ID of the gizmo to remove.
		// !WARNING! This calls a delete on the GizmoObject.
		// !WARNING! This batch will draw weird until Update() is called.
		void Remove(unsigned int ui_GizmoID){
			if (_GizmoObjects.find(ui_GizmoID) != _GizmoObjects.end()){
				_VertexCount	-= _GizmoObjects[ui_GizmoID]->VertexCount;
				_IndexCount		-= _GizmoObjects[ui_GizmoID]->IndexCount;
				delete _GizmoObjects[ui_GizmoID];
				_GizmoObjects.erase(ui_GizmoID);
			}
			if (_GizmoObjects.size() == 0){_NextObjectID = 0;}
		}

		// Get a Gizmo from the batch.
		// ui_GizmoID = The ID of the gizmo to return.
		// GizmoType  = The Type of the gizmo to return;
		// !WARNING! any changes will need to be updated.
		template <typename GizmoType>
		GizmoObject<TYPE>* Get(unsigned int ui_GizmoID){
			if (_GizmoObjects.find(ui_GizmoID) != _GizmoObjects.end()){
				return (GizmoType<TYPE>)_GizmoObjects[ui_GizmoID];
			}
		}

		// Update the Gizmos.
		// Call ONCE after all changes have been made.
		// If this hasn't been called nothing added or removed will be updated.
		void Update(){

			_Verticies = new TYPE[_VertexCount];
			_Indices = new unsigned int[_IndexCount];

			_VertexCount = 0;
			_IndexCount = 0;

			for (std::map<unsigned int,GizmoObject<TYPE>*>::iterator it = _GizmoObjects.begin(); it != _GizmoObjects.end(); it++){
				for(unsigned int i = 0; i < it->second->VertexCount; i++){
					_Verticies[_VertexCount + i] = it->second->Verticies[i];
				}

				for(unsigned int i = 0; i < it->second->IndexCount; i++){
					_Indices[_IndexCount + i] = _VertexCount + it->second->Indicies[i];
				}

				_VertexCount += it->second->VertexCount;
				_IndexCount += it->second->IndexCount;
			}

			glBindBuffer(GL_ARRAY_BUFFER, _VBO);
			glBufferSubData(GL_ARRAY_BUFFER,0, _VertexCount * sizeof(TYPE), _Verticies);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0, _IndexCount * sizeof(unsigned int), _Indices);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// Draw the Gizmos with the given Shader.
		// Will only draw if it has been given a Shader.
		void Draw(){
				if (_sGizmoShader != nullptr && _IndexCount >= 0 && _VAO != 0){
				glBindVertexArray( _VAO );
				glDrawElements(GL_TRIANGLES, _IndexCount , GL_UNSIGNED_INT, _Indices);
				glBindVertexArray(0);
			}
		}

	protected:
		unsigned int _VAO,_VBO,_IBO;
		Shader* _sGizmoShader;

		unsigned int _VertexCount;
		TYPE *_Verticies;

		unsigned int _IndexCount;
		unsigned int *_Indices;

		std::map<unsigned int,GizmoObject<TYPE>*> _GizmoObjects;   
		unsigned int _NextObjectID;
	};

	namespace Gizmo{

		// Gizmo Plane
		// X Verticies, Y Indicies
		template <typename TYPE>
		struct Plane : public GizmoObject<TYPE>{
			Plane(glm::vec3 Location,glm::vec3 Extents,glm::vec4 Colour){

				VertexCount = 4;
				Verticies = new TYPE[VertexCount];

				Verticies[0].position = glm::vec4(Location,0.0f) + glm::vec4(  Extents.x,  0, -Extents.z,1.0f);
				Verticies[1].position = glm::vec4(Location,0.0f) + glm::vec4(  Extents.x,  0,  Extents.z,1.0f);
				Verticies[2].position = glm::vec4(Location,0.0f) + glm::vec4( -Extents.x,  0,  Extents.z,1.0f);
				Verticies[3].position = glm::vec4(Location,0.0f) + glm::vec4( -Extents.x,  0, -Extents.z,1.0f);
				
				Verticies[0].normal = glm::vec4( 0.5f, 1.0f,-0.5f, 1.0f);
				Verticies[1].normal = glm::vec4( 0.5f, 1.0f, 0.5f, 1.0f);
				Verticies[2].normal = glm::vec4(-0.5f, 1.0f, 0.5f, 1.0f);
				Verticies[3].normal = glm::vec4(-0.5f, 1.0f,-0.5f, 1.0f);
				
				Verticies[0].uv = glm::vec2(1,0);
				Verticies[1].uv = glm::vec2(1,1);
				Verticies[2].uv = glm::vec2(0,1);
				Verticies[3].uv = glm::vec2(0,0);
				
				Verticies[0].colour = Colour;
				Verticies[1].colour = Colour;
				Verticies[2].colour = Colour;
				Verticies[3].colour = Colour;

				IndexCount = 6;
				Indicies = new unsigned int[IndexCount];
				Indicies[0] = Indicies[3] = 0; 
				if (Extents.y >= 0){
					Indicies[1] = 2; Indicies[2] = 1;
					Indicies[4] = 3; Indicies[5] = 2;
				}else{
					Indicies[1] = 1; Indicies[2] = 2;
					Indicies[4] = 2; Indicies[5] = 3;
				}

			}
		};

		// Gizmo Point
		// 1 Vertex, 1 Index
		template <typename TYPE>
		struct Point_1v_1i : public GizmoObject<TYPE>{
			Point_1v_1i(glm::vec3 Location,glm::vec4 Colour){

				VertexCount = 1;
				Verticies = new TYPE[VertexCount];
				Verticies[0].position = glm::vec4(Location,0.0f); 
				Verticies[0].normal = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				Verticies[0].uv = glm::vec2(0,0);
				Verticies[0].colour = Colour;

				IndexCount = 1;
				Indicies = new unsigned int[IndexCount];
				Indicies[0] = 0;

			}
		};

		// Gizmo Sphere
		// X Verticies, Y Indicies
		template <typename TYPE>
		struct Sphere : public GizmoObject<TYPE>{
			Sphere(glm::vec3 Location,float Radius, glm::vec2 v2_SubDivisions,glm::vec4 Colour){
			
				VertexCount = 1;
				Verticies = new TYPE[VertexCount];
				Verticies[0].position = glm::vec4(Location,0.0f); 
				Verticies[0].normal = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				Verticies[0].uv = glm::vec2(0,0);
				Verticies[0].colour = Colour;

				IndexCount = 1;
				Indicies = new unsigned int[IndexCount];
				Indicies[0] = 0;
			}
		};

		// Gizmo Box 
		// 8 Verticies, 36 Incicies
		template <typename TYPE>
		struct Box_v8_i36 : public GizmoObject<TYPE>{
			Box_v8_i36(glm::vec3 Location,glm::vec3 Extents,glm::vec4 Colour){
				IndexCount = 36;
				VertexCount = 8;

				Verticies = new TYPE[VertexCount];
				Verticies[0].position = glm::vec4(Location,0.0f) + glm::vec4( -Extents.x, -Extents.y, -Extents.z,1.0f); Verticies[0].normal = glm::vec4(-0.5f,-0.5f,-0.5f, 1.0f); Verticies[0].uv = glm::vec2(1,0);
				Verticies[1].position = glm::vec4(Location,0.0f) + glm::vec4( -Extents.x, -Extents.y,  Extents.z,1.0f); Verticies[1].normal = glm::vec4(-0.5f,-0.5f, 0.5f, 1.0f); Verticies[1].uv = glm::vec2(1,1);
				Verticies[2].position = glm::vec4(Location,0.0f) + glm::vec4(  Extents.x, -Extents.y,  Extents.z,1.0f); Verticies[2].normal = glm::vec4( 0.5f,-0.5f, 0.5f, 1.0f); Verticies[2].uv = glm::vec2(0,1);
				Verticies[3].position = glm::vec4(Location,0.0f) + glm::vec4(  Extents.x, -Extents.y, -Extents.z,1.0f); Verticies[3].normal = glm::vec4( 0.5f,-0.5f,-0.5f, 1.0f); Verticies[3].uv = glm::vec2(0,0);
																																											  
				Verticies[4].position = glm::vec4(Location,0.0f) + glm::vec4(  Extents.x,  Extents.y, -Extents.z,1.0f); Verticies[4].normal = glm::vec4( 0.5f, 0.5f,-0.5f, 1.0f); Verticies[4].uv = glm::vec2(1,0);
				Verticies[5].position = glm::vec4(Location,0.0f) + glm::vec4(  Extents.x,  Extents.y,  Extents.z,1.0f); Verticies[5].normal = glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f); Verticies[5].uv = glm::vec2(1,1);
				Verticies[6].position = glm::vec4(Location,0.0f) + glm::vec4( -Extents.x,  Extents.y,  Extents.z,1.0f); Verticies[6].normal = glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f); Verticies[6].uv = glm::vec2(0,1);
				Verticies[7].position = glm::vec4(Location,0.0f) + glm::vec4( -Extents.x,  Extents.y, -Extents.z,1.0f); Verticies[7].normal = glm::vec4(-0.5f, 0.5f,-0.5f, 1.0f); Verticies[7].uv = glm::vec2(0,0);

				for (unsigned int i = 0; i < VertexCount; i++){
					Verticies[i].colour = Colour;
				}

				unsigned int Ind[36] = {
					0,2,1 ,0,3,2,
					2,6,1 ,2,5,6,
					3,5,2 ,3,4,5,
					7,6,5 ,7,5,4,
					0,4,3 ,0,7,4,
					1,7,0 ,1,6,7,
				};

				Indicies = new unsigned int[IndexCount];
				for (unsigned int i = 0; i < IndexCount; i++){
					Indicies[i] = Ind[i];
				}
			}
		};
	}
}

