#include "Application.h"

#include <iostream>

#include <gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/type_ptr.hpp>

static Application* s_Instance = nullptr;

Application::Application(const ApplicationSpecification& specification)
	: m_Specification(specification), m_Camera(45.0f, 0.01f, 100.0f)
{
    s_Instance = this;

    m_WindowData.Title = m_Specification.Name;
    m_WindowData.Width = m_Specification.Width;
    m_WindowData.Height = m_Specification.Height;

	Init();
}

Application::~Application()
{
    s_Instance = nullptr;

	Shutdown();
}

void Application::Init()
{
    if (!glfwInit())
        printf("GLFW not initialised.\n");

    m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowData.Title.c_str(), NULL, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        printf("Window does not exist.\n");
    }

    glfwMakeContextCurrent(m_Window);

    if (glewInit() != GLEW_OK)
        printf("GLEW not ok.\n");

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST); 
    //glEnable(GL_CULL_FACE);

    glfwSwapInterval(1);

    glfwSetWindowUserPointer(m_Window, &m_WindowData);

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;
            data.Resize = true;
        });

    m_ImGuiLayer->Init();

    float cubeVerts[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
    };

    uint32_t cubeIndecies[] = {
        0, 1, 2,
        1, 2, 3,
        2, 6, 4,
        4, 0, 2,
        2, 3, 6,
        7, 6, 3,
        6, 7, 4,
        7, 4, 5,
        7, 3, 1,
        1, 5, 7,
        0, 1, 4,
        4, 1, 5,
    };

    m_VertexArray = VertexArray::Create();

    m_VertexBuffer = VertexBuffer::Create(cubeVerts, sizeof(cubeVerts));
    m_VertexBuffer->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float3, "a_Colour" },
    });
    m_IndexBuffer = IndexBuffer::Create(cubeIndecies, sizeof(cubeIndecies) / sizeof(uint32_t));

    m_VertexArray->AddVertexBuffer(m_VertexBuffer);
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);

    m_Shader = Shader::Create("res/shaders/vVoxel.glsl", "res/shaders/fVoxel.glsl");
    //m_Shader = Shader::Create("res/shaders/VoxelVert.glsl", "res/shaders/VoxelFrag.glsl");
    m_Shader->Bind();

    m_Camera.SetPosition({ 0.0f, 0.0f, 2.0f });
    m_Camera.OnResize(m_WindowData.Width, m_WindowData.Height);

    m_Crate = VoxelModel::Create("res/assets/Crate.vox");
    m_Crate->Bind();
}

void Application::OnResize()
{
    m_WindowData.Resize = false;

    if (m_WindowData.Width == 0 || m_WindowData.Height == 0)
    {
        m_Minimised = true;
        return;
    }

    m_Minimised = false;
    glViewport(0, 0, m_WindowData.Width, m_WindowData.Height);

    m_Camera.OnResize(m_WindowData.Width, m_WindowData.Height);
}

void Application::Run()
{
	m_Running = true;

    while (!glfwWindowShouldClose(m_Window) && m_Running)
    {
        if (m_WindowData.Resize)
            OnResize();

        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - m_LastTime;
        m_LastTime = currentTime;

        glfwPollEvents();
        if (!m_Minimised)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.4f, 0.4f, 0.4f, 0.0f);

            m_Camera.OnUpdate(deltaTime);

            //m_ModelMat = glm::rotate(m_ModelMat, glm::radians(30.0f) * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));

            // Vertex
            m_Shader->UploadUniformMat4("u_Model", m_ModelMat);
            m_Shader->UploadUniformMat4("u_View", m_Camera.GetViewMatrix());
            m_Shader->UploadUniformMat4("u_Projection", m_Camera.GetProjectionMatrix());
            
            // Frag
            m_Shader->UploadUniformFloat3("u_CamPos", m_Camera.GetPosition());
            m_Shader->UploadUniformFloat3("u_CamDir", m_Camera.GetDirection());
            m_Shader->UploadUniformMat4("u_InverseProjection", m_Camera.GetInverseProjectionMatrix());
            m_Shader->UploadUniformMat4("u_InverseView", m_Camera.GetInverseViewMatrix());
            m_Shader->UploadUniformInt2("u_Resolution", { GetWidth(), GetHeight() });
            m_Shader->UploadUniformFloat3("u_ObjectSize", m_Crate->GetSize());

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

            m_ImGuiLayer->Begin();

            ImGui::Begin("Debug");
            ImGui::Text("Rendering Stats");
            ImGui::Separator();
            ImGui::Text("Frame Time: %f", deltaTime);
            ImGui::Text("FPS: %d", (int)(1.0f / deltaTime)); 
            ImGui::End();

            m_ImGuiLayer->End();

            glfwSwapBuffers(m_Window);
        }
    }
}

void Application::Close()
{
	m_Running = false;
}

Application& Application::Get()
{
    return *s_Instance;
}

void Application::Shutdown()
{
    m_ImGuiLayer->Shutdown();
    glfwTerminate();
}