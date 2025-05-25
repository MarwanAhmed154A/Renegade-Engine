#include "Framebuffer.h"
#include "Platform/OpenGL/GLFramebuffer.h"

namespace RG
{
    Framebuffer* RG::Framebuffer::Create(Vec2 size)
    {
        return new GLFramebuffer(size);
    }
}