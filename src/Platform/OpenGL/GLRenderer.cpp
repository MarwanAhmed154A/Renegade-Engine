#include "GLRenderer.h"
#include "Log.h"
#include "Renderer/Shader.h"
#include "Events/Event.h"
#include "Scene/Entity.h"
#include "Time/Time.h"
#include "Model.h"
#include "Input/Input.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>

unsigned int VBO, VAO, EBO;
RG::Shader *shader, *cubemapShader, *SSAAShader;
RG::Framebuffer* buf;
RG::Vec2 curViewSizeVec;

float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
float camMovSpeed = 100.0f;
bool firstMouse = true;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void UpdateCamera(double xposIn, double yposIn)
{
	float xMov = (Input::GetAxis(AxisCodes::Vertical));
	float yMov = (Input::GetAxis(AxisCodes::Horizontal));

	if(xMov || yMov)
		cameraPos += glm::normalize((cameraFront * xMov + glm::cross(cameraFront, cameraUp) * yMov)) * (float)(camMovSpeed * Time::GetDeltaTime());

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	pitch = std::clamp(pitch, -89.0f, 89.0f);

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

namespace RG
{
	GLRenderer::GLRenderer()
	{
		EventManager::AddCallback<GLRenderer>(EventType::OnWindowResize, this, &GLRenderer::OnWindowResizeCallback); 
		EventManager::AddCallback<GLRenderer>(EventType::OnSceneInit, this, &GLRenderer::OnSceneInitCallback);
	}

	void GLRenderer::InitImpl()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		RG_CORE_INFO("GLFW INIT SUCCESFUL")
	}

	void GLRenderer::ViewportInitImpl()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			RG_CORE_ERROR("FAILED TO INIT GLAD")
		}
		else
			RG_CORE_INFO("GLAD INIT SUCCESFUL")

		float vertices[] = {
			// positions         // texture coords
			 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,   // top right
			 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,   // bottom right
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,   // bottom left
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f    // top left 
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		projection = glm::perspective(glm::radians(90.0f), (float)(1920 / 1080), 0.1f, 1000.0f);

		cubemapShader = Shader::CreateShader("Shaders/cubemapVertex.shader", "Shaders/cubemapFragment.shader");
		cubemapShader->Bind();
		cubemapShader->SetMat4("projection", glm::value_ptr(projection));

		//Setup default shader
		shader = Shader::CreateShader("Shaders/vertex.shader", "Shaders/fragment.shader");
		shader->Bind();

		shader->SetMat4("projection", glm::value_ptr(projection));

		m_nativeOutput = Framebuffer::Create(Vec2(800, 600));
		buf            = Framebuffer::Create(Vec2(800, 600));


		SSAAShader = Shader::CreateShader("Shaders/SSAAVertex.shader", "Shaders/SSAAFragment.shader");

		glViewport(0, 0, 800, 600);

		glEnable(GL_DEPTH_TEST);

		//glPolygonMode(GL_FRONT, GL_FILL);
		//glDepthMask(GL_TRUE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glfwWindowHint(GLFW_DEPTH_BITS, 24);
	}

	void GLRenderer::RenderImpl()
	{

	}

	void GLRenderer::RenderEntitiesImpl()
	{
		if (Input::GetMouseState() == MouseState::HIDDEN)
			UpdateCamera(Input::GetCursorPos().x, Input::GetCursorPos().y);
		else
			firstMouse = true;
		//setup view matrix
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shader->SetMat4("view", glm::value_ptr(view));

		buf->Bind();
		glViewport(0, 0, curViewSizeVec.x * 4, curViewSizeVec.y * 4);
		//clear framebuffer
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Transform* curTransform = nullptr;

		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);
		glDisable(GL_CULL_FACE);

		glActiveTexture(GL_TEXTURE0);

		CubemapComponent* mapComp = nullptr;
		for (int i = 0; i < ents->GetLength(); i++)
		{
			CubemapComponent* curMapComp = ents[0][i]->GetComponent<CubemapComponent>();
			if (curMapComp)
			{
				cubemapShader->Bind();
				cubemapShader->SetMat4("projection", glm::value_ptr(projection));

				glm::mat4 viewMat = glm::mat4(glm::mat3(view));
				cubemapShader->SetMat4("view", glm::value_ptr(viewMat));
				cubemapShader->SetInt("skybox", 0);

				curMapComp->map->Draw();
				mapComp = curMapComp;
			}
		}

		//Render cycle for all entities in scene
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		shader->Bind();
		shader->SetMat4("projection", glm::value_ptr(projection));
		shader->SetMat4("view", glm::value_ptr(view));
		shader->SetInt("skybox", 4);
		shader->SetVec3("cameraPos", glm::value_ptr(cameraPos));
		if (mapComp)
		{
			glActiveTexture(GL_TEXTURE4);
			mapComp->map->BindTex();
		}
		for (int i = 0; i < ents->GetLength(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);

			curTransform = (*ents)[i]->transform;
			

			//check if entity has texComp and bind if so
			//TextureComponent* t = ents[0][i]->GetComponent<TextureComponent>();
			ModelComponent* m = (*ents)[i]->GetComponent<ModelComponent>();
			if (m != nullptr && m->m_model.get() && m->m_model.get()->GetPath().length() > 1)
			{
				model = glm::translate(model, glm::vec3(curTransform->Position.x, curTransform->Position.y, curTransform->Position.z));

				Model* mModel = m->m_model.get();
				model = glm::rotate(model, glm::radians(curTransform->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(curTransform->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(curTransform->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

				model = glm::scale(model, glm::vec3(curTransform->LocalScale.x, curTransform->LocalScale.y, curTransform->LocalScale.z));

				shader->SetMat4("model", glm::value_ptr(model));
				//shader->SetVec4("solidBaseColor", m->m_model->)
				m->Draw(shader);
			}

			TextureComponent* t = ents[0][i]->GetComponent<TextureComponent>();
			if (t != nullptr && t->m_texture.get() && t->m_texture.get()->GetPath().length() > 1)
			{
				t->Bind();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}

		glDisable(GL_CULL_FACE);

		m_nativeOutput->Bind();
		SSAAShader->Bind();
		SSAAShader->SetInt("tex", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, buf->GetTextureID());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, curViewSizeVec.x, curViewSizeVec.y);
		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glEnable(GL_DEPTH_TEST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLRenderer::OnSceneInitCallback(struct Event* e)
	{
		ents = e->SafeCastTo<OnSceneInitEvent>()->vec;
	}

	void GLRenderer::OnWindowResizeCallback(Event* event)
	{
		static double lastResizeTime = Time::GetTime();
		//Convert to needed event type
		OnWindowResizeEvent* e = event->SafeCastTo<OnWindowResizeEvent>();

		//resize only once a second
		if (e->size.x != 0.0f && e->size.y != 0.0f)
		{
			buf->Resize(Vec2(e->size.x * 4, e->size.y * 4));

			//Resize framebuffer
			m_nativeOutput->Resize(Vec2(e->size.x, e->size.y));
			glViewport(0, 0, e->size.x, e->size.y);

			//Get current size of window
			float x = e->size.x, y = e->size.y;

			projection = glm::perspective(glm::radians(fov), (float)(x / y), 0.1f, 100000.0f);
			curViewSizeVec = e->size;
		}
	}

}