#pragma once

//	For use by KDEngine apps
#include "KDEngine/Log.h"
#include "KDEngine/Layer.h"
#include "KDEngine/Application.h"
#include "KDEngine/ImGui/ImGuiLayer.h"

//	Input
#include "KDEngine/Codes/KeyCodes.h"
#include "KDEngine/Codes/MouseCodes.h"
#include "KDEngine/Input.h"

//	Time
#include "KDEngine/Core/Timestep.h"

//	Renderer
#include "KDEngine/Renderer/Renderer.h"
#include "KDEngine/Renderer/RendererCommand.h"
#include "KDEngine/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "KDEngine/Renderer/Shader.h"
#include "KDEngine/Renderer/Buffer.h"
#include "KDEngine/Renderer/VertexArray.h"
#include "KDEngine/Renderer/OrthographicCamera.h"

//	Glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//	Entry point
#include "KDEngine/EntryPoint.h"