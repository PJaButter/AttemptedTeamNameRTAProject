#pragma once

#include "RenderContext.h"

/// An object to define and apply a material
/// For the fastest rendering possible RenderNode batches should be sorted 
/// in a manner to have as few RenderMaterials as possible
class RenderMaterial : public RenderNode
{
public:
	RenderMaterial();
	virtual ~RenderMaterial();

	/// Adds a RenderNode to the RenderSet of this context.  This should be called
	/// on all RenderNodes that should be rendered with this context each frame.
	/// \param nodePtr - The RenderNode to be rendered in this batch.
	inline void AddRenderNode(RenderNode *nodePtr)
	{
		// Check if we need to add to our parent...
		if (renderSet.GetHead() == nullptr && parentPtr != nullptr)
		{
			// assuming parent is a RenderContext, this may prove dangerous in some setups
			((RenderContext *)parentPtr)->AddRenderNode(this);
		}
		renderSet.AddRenderNode(nodePtr);
		nodePtr->SetParent(this);
	}

	/// Clears the RenderSet of this context.
	inline void ClearRenderSet(void) { renderSet.ClearRenderSet(); }

	static void MaterialStubRenderFunc(RenderNode &node);


protected:

	/// The linked list of RenderNodes to be rendered using this context.
	/// This is typically filled with RenderShape instances.
	RenderSet renderSet;

};

