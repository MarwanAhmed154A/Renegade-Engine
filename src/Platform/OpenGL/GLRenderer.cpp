#include "GLRenderer.h"
#include "Log.h"
#include "Renderer/Shader.h"
#include "Events/Event.h"
#include "Scene/Entity.h"
#include "Time/Time.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>

unsigned int VBO, VAO, EBO;
RG::Shader* shader;

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
			 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // top right
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // bottom left
			-0.5f,  0.5f, 0.0f,  0.0f, 1.0f    // top left 
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

		//Setup default shader
		shader = Shader::CreateShader("Shaders/vertex.shader", "Shaders/fragment.shader");
		shader->Bind();

		projection = glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, 0.1f, 100.0f);
		shader->SetMat4("projection", glm::value_ptr(projection));

		m_nativeOutput = Framebuffer::Create(Vec2(800, 600));

		glViewport(0, 0, 800, 600);
	}

	void GLRenderer::RenderImpl()
	{

	}

	void GLRenderer::RenderEntitiesImpl()
	{
		//Bind Shader and setup rojection matrix
		shader->Bind();
		shader->SetMat4("projection", glm::value_ptr(projection));

		//setup view matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));
		shader->SetMat4("view", glm::value_ptr(view));

		m_nativeOutput->Bind();

		//clear framebuffer
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		
		Transform* curTransform = nullptr;
		//Render cycle for all entities in scene
		for (int i = 0; i < ents->GetLength(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);

			curTransform = (*ents)[i]->transform;
			model = glm::translate(model, glm::vec3(curTransform->Position.x, curTransform->Position.y, curTransform->Position.z));
			model = glm::scale(model, glm::vec3(curTransform->LocalScale.x, curTransform->LocalScale.y, curTransform->LocalScale.z));

			shader->SetMat4("model", glm::value_ptr(model));

			//check if entity has texComp and bind if so
			TextureComponent* t = ents[0][i]->GetComponent<TextureComponent>();
			if (t != nullptr)
				t->Bind();

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLRenderer::OnSceneInitCallback(class Event* e)
	{
		ents = e->SafeCastTo<OnSceneInitEvent>()->vec;
	}

	void GLRenderer::OnWindowResizeCallback(Event* event)
	{
		static int lastResizeTime = Time::GetTime();

		//resize only once a second
		if (Time::GetTime() - lastResizeTime > 1.0f)
		{
			//Convert to needed event type
			OnWindowResizeEvent* e = event->SafeCastTo<OnWindowResizeEvent>();

			//Resize framebuffer
			m_nativeOutput->Resize(e->size);

			glViewport(0, 0, e->size.x, e->size.y);

			//Get current size of window
			float x = e->size.x, y = e->size.y;
			projection = glm::ortho(0.0f, x, 0.0f, y, 0.1f, 100.0f);

			//prepare resize log meesage
			std::string s = "WINDOW RESIZED: ";
			s += std::to_string((int)x);
			s += ", " + std::to_string((int)y);

			RG_CORE_INFO(s);
		}

		lastResizeTime = Time::GetTime();
	}

}