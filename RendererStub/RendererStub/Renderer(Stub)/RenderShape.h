#pragma once
#include "RenderMaterial.h"

/// An object to define and render a unique instance of a shape in our scene.
/// We would typically have one RenderShape instance for each time we wish to 
/// render a mesh in our scene. Two or more RenderShape that render the same
/// mesh can instance the same mesh data, but each shape will have a unique matrix
class RenderShape :
	public RenderNode
{
public:
	RenderShape();
	virtual ~RenderShape();

	static void ShapeStubRenderFunc(RenderNode &node);

protected:

	// The current world transform for this RenderShape. Feel free to store
	// the transfomr in a different data structure.
	float worldMatrix[16];

	// This object will also need access to whatever mesh data is needed for 
	// rendering. In an optimized system this mesh data would consist of just
	// the starting index and amount to render from the vertex buffer that
	// would have bound in a previously processed RenderContext. Consider
	// creating a "RenderMesh" object handle loading mesh data from file,
	// as well as stroing the data needed for rendering. The RenderMesh, 
	// IndexBuffer and VertexBuffer classes in the FSGDEngine may provide
	// some ideas on how to handle these elements.
	void *RenderMesh; // <-- replace that with something else

};

