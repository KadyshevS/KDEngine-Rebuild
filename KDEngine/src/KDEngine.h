#pragma once

//	For use by KDEngine apps
#include "KDEngine/Log.h"
#include "KDEngine/Layer.h"
#include "KDEngine/Application.h"
#include "KDEngine/ImGui/ImGuiLayer.h"

//	ImGui
#include "ImGui/imgui.h"
#include "ImGuizmo.h"

//	Input
#include "KDEngine/Codes/KeyCodes.h"
#include "KDEngine/Codes/MouseCodes.h"
#include "KDEngine/Input.h"

//	Time
#include "KDEngine/Core/Timestep.h"

//	Random
#include "KDEngine/Core/Random.h"

//	Renderer
#include "KDEngine/Renderer/Renderer.h"
#include "KDEngine/Renderer/RendererCommand.h"
#include "KDEngine/Renderer/RendererAPI.h"
#include "KDengine/Renderer/Renderer2D.h"

//	Scene
#include "KDEngine/Scene/Scene.h"
#include "KDEngine/Scene/SceneSerializer.h"
#include "KDEngine/Scene/Entity.h"
#include "KDEngine/Scene/ScriptableEntity.h"
#include "KDEngine/Scene/Components.h"

//	OpenGL
#include "Platform/OpenGL/OpenGLShader.h"
#include "KDEngine/Renderer/Shader.h"
#include "KDEngine/Renderer/Buffer.h"
#include "KDEngine/Renderer/VertexArray.h"
#include "KDEngine/OrthographicCameraController.h"
#include "KDEngine/Renderer/Texture.h"
#include "KDEngine/Renderer/SubTexture2D.h"
#include "KDEngine/Renderer/Framebuffer.h"

//	Math
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "KDEngine/Math/Math.h"

//	Core
#include "KDEngine/Core/ParticleSystem.h"
#include "KDEngine/Utils/PlatformUtils.h"

//	Entry point
// #include "KDEngine/EntryPoint.h"