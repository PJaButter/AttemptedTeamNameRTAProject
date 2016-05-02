#include "stdafx.h"
#include "RenderContext.h"
#include "Renderer.h"


RenderContext::RenderContext()
{
}


RenderContext::~RenderContext()
{
}

void RenderContext::ContextStubRenderFunc(RenderNode &node)
{
	RenderContext &context = (RenderContext &)node;
	//1.) Apply any rendering state changes such as rasterization, blending, etc as well
	// as activating the correct shader

	//2.) Pass your RenderSet to the renderer
	Renderer::Render(*context.renderSetPtr);

	//3.) revert any render changes you made from the default state 
	// such as rasterization, blending, etc.
}
