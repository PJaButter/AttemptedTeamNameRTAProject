#include "stdafx.h"
#include "RenderShape.h"


RenderShape::RenderShape()
{
}


RenderShape::~RenderShape()
{
}

void RenderShape::ShapeStubRenderFunc(RenderNode &node)
{
	RenderShape &shape = (RenderShape &)node;

	// Use your "parentPtr"
	// if you need to access the RenderContext

	// Apply your world tranform

	// Make your draw calls here
}
