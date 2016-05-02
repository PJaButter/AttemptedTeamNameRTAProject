#include "stdafx.h"
#include "RenderMaterial.h"
#include "Renderer.h"

RenderMaterial::RenderMaterial()
{
}


RenderMaterial::~RenderMaterial()
{
}

void RenderMaterial::MaterialStubRenderFunc(RenderNode &node)
{
	RenderMaterial &material = (RenderMaterial &)node;
	//1.) Apply any textures or other material states. Use your "parentPtr"
	// if you need to access the RenderContext

	//2.) Pass your RenderSet to the renderer
	Renderer::Render(material.renderSet);

	//3.) revert any needed changes. If the same textures are being used as 
	// shader resources and render targets you will likely need to disable 
	// the resources here.
}