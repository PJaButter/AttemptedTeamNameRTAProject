#pragma once
#include "RenderNode.h"
#include "RenderSet.h"

// Use this object to create and give access to API specific 
// resources and for rendering a RenderSet

// You will likely need access to API resources thoughout the
// rest of the renderer library code. Take this into consideration when 
// designing this object. You could make this a singleton, pass the one and only 
// instance of it to each fo the other objects that will need it or make
// all of the functionality and resources accessible globally. The FSGDEngine
// does the last option by making all members of the class static, in this example
// the class could and likely should have been just a namespace
class Renderer
{
public:
	Renderer();
	~Renderer();

	/// Renders a batch of RenderNode instances contained in the RenderSet passed in.
	/// \param set - The batch to be rendered.
	static void Render(RenderSet &set);
};

