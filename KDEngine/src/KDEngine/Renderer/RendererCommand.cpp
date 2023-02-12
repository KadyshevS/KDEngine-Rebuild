#include <kdpch.h>
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace KDE
{
	KDE::RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI;

}