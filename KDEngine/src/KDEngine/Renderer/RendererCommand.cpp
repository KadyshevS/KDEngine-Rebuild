#include <kdpch.h>
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace KDE
{
	Scope<RendererAPI> RendererCommand::s_RendererAPI = MakeScope<OpenGLRendererAPI>();
}