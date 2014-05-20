#include "States/GizmoTest.hpp"

#include <glm/ext.hpp>

#include "Game.hpp"
#include "Logger.hpp"
#include "States/AllStates.hpp"

#include "Movement.hpp"

#include "Skybox.hpp"
#include "Textures.hpp"

namespace Osiris{

	// Passing a pointer to a class and using it
	void UpdateGizmo(std::string* returnstring,void* vp_This){
		VertexBatch <sPosColNormUV>*vbGizmos = (VertexBatch<sPosColNormUV>*)vp_This;
		vbGizmos->Update();
		returnstring->append("finished");
	}

	// Passing a pointer to a class and using it with dynamic values
	void AddBox(std::string* returnstring,void* vp_This){
		VertexBatch <sPosColNormUV>*vbGizmos = (VertexBatch<sPosColNormUV>*)vp_This;

		std::string rs = "box";
		unsigned int index = returnstring->find(" ")+1;

		unsigned int subindex = 0;
		float Values[6];
		unsigned int index2 = index+1;
		char buffer[64];

		while (index != -1 && index < returnstring->size()){
			index2 = returnstring->find(" ",index)-1;
			Values[subindex] = (float)atof((char*)returnstring->substr(index,index2).c_str());
			index = index2+2;
			subindex++;
		}

		Gizmo::Box_v8_i36<sPosColNormUV> *Box = new Gizmo::Box_v8_i36<sPosColNormUV>(glm::vec3(Values[0],Values[1],Values[2]),glm::vec3(Values[3],Values[4],Values[5]),glm::vec4(1));
		unsigned int ID = vbGizmos->Add(Box);

		sprintf(buffer," %i (%i,%i,%i)(%i,%i,%i)",ID,(int)Values[0],(int)Values[1],(int)Values[2],(int)Values[3],(int)Values[4],(int)Values[5]);
		rs.append(buffer);

		(*returnstring) = rs;
	}

	// Passing a pointer to a class and using it with dynamic values
	void AddPlane(std::string* returnstring,void* vp_This){
		VertexBatch <sPosColNormUV>*vbGizmos = (VertexBatch<sPosColNormUV>*)vp_This;

		std::string rs = "plane";
		unsigned int index = returnstring->find(" ")+1;

		unsigned int subindex = 0;
		float Values[6];
		unsigned int index2 = index+1;
		char buffer[64];

		while (index != -1 && index < returnstring->size()){
			index2 = returnstring->find(" ",index)-1;
			Values[subindex] = (float)atof((char*)returnstring->substr(index,index2).c_str());
			index = index2+2;
			subindex++;
		}

		Gizmo::Plane<sPosColNormUV> *Plane = new Gizmo::Plane<sPosColNormUV>(glm::vec3(Values[0],Values[1],Values[2]),glm::vec3(Values[3],Values[4],Values[5]),glm::vec4(1));
		unsigned int ID = vbGizmos->Add(Plane);

		sprintf(buffer," %i (%i,%i,%i)(%i,%i,%i)",ID,(int)Values[0],(int)Values[1],(int)Values[2],(int)Values[3],(int)Values[4],(int)Values[5]);
		rs.append(buffer);

		(*returnstring) = rs;
	}

	// Passing a pointer to a class and using it with dynamic values
	void Remove(std::string* returnstring,void* vp_This){
		VertexBatch <sPosColNormUV>*vbGizmos = (VertexBatch<sPosColNormUV>*)vp_This;

		std::string rs = "Element";
		int index = returnstring->find(" ");

		int Values;

		glm::vec3 cPos = (*App::GetCamera())[3].xyz;

		while (index != -1){
			int index2 = returnstring->find(" ",index);
			rs.append(":");
			Values = atoi((char*)returnstring->substr(index,index2).c_str());
			rs.append((char*)returnstring->substr(index,index2).c_str());
			index = returnstring->find(" ",index2+1);
			vbGizmos->Remove(Values);
		}

		rs.append("Removed");
		(*returnstring) = rs;
	}

	GizmoTest::GizmoTest(void){}

	GizmoTest::~GizmoTest(void){
		Unload();
	}

	void GizmoTest::Load(){
		
		GizmoShader = new Shader("./Shaders/Gizmo.vert","./Shaders/Gizmo.frag",0,0,0,0);
		GizmoShader->SetUniformMatrix("Projection", "m4fv", 1, false, true, glm::value_ptr(App::Perspective()));

		vbGizmos = new VertexBatch<sPosColNormUV>();

		Game::GlobalContainer.ElementGet<Console>("Console")->Add("box PX PY PZ EX EY EZ",AddBox,vbGizmos);
		Game::GlobalContainer.ElementGet<Console>("Console")->Add("plane PX PY PZ EX UP EZ",AddPlane,vbGizmos);
		Game::GlobalContainer.ElementGet<Console>("Console")->Add("update",UpdateGizmo,vbGizmos);
		Game::GlobalContainer.ElementGet<Console>("Console")->Add("remove ID",Remove,vbGizmos);

		vbGizmos->EnableVAA(0,4,GL_FLOAT,GL_FALSE,sPosColNormUV::PositionOffset);
		vbGizmos->EnableVAA(1,4,GL_FLOAT,GL_FALSE,sPosColNormUV::ColourOffset);
		vbGizmos->EnableVAA(2,4,GL_FLOAT,GL_FALSE,sPosColNormUV::NormalOffset);
		vbGizmos->EnableVAA(3,2,GL_FLOAT,GL_FALSE,sPosColNormUV::UVOffset);
		vbGizmos->SetShader(GizmoShader);

		Gizmo::Box_v8_i36<sPosColNormUV> *Box = new Gizmo::Box_v8_i36<sPosColNormUV>(glm::vec3(0),glm::vec3(1),glm::vec4(1));
		vbGizmos->Add(Box);
		vbGizmos->Update();

		// Something behind the menu to make it look better
		Shader *sSkyboxShader = new Shader("./Shaders/Skybox.vert","./Shaders/Skybox.frag",0,0,0,0);
		TextureCube *sDayBox = new TextureCube("./Images/Cubemaps/Lagoon/Lagoon",".jpg");
		sSky = new Skybox(100.0f,sDayBox,sSkyboxShader);

	}

	void GizmoTest::Unload(){
		delete sSky;
		delete GizmoShader;
		delete vbGizmos;
	}

	void GizmoTest::Update(){

		if (Game::KeyPressed(Key::ESC)){
			Game::Manager.Pop();
		}

		if (!Game::GlobalContainer.HasKeyboard()){
			Movement::Movement3D(10);
		}
	}

	void GizmoTest::Draw(){
		sSky->Draw(App::GetView(),App::GetProjection());
		GizmoShader->SetUniformMatrix("View", "m4fv", 1, false, true, glm::value_ptr(App::GetView()));
		vbGizmos->Draw();
	}
}
