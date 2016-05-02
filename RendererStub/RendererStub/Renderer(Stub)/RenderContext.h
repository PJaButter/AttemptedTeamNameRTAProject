#pragma once

#include "RenderNode.h"
#include "RenderSet.h"

/// An object to define and apply a collection of render states.
/// The RenderContext class is a specialization of RenderNode that is used to set
/// any render states that can be shared.  For the fastest rendering possible
/// RenderNode batches should be sorted in a manner to have as few RenderContexts
/// as possible
class RenderContext : public RenderNode
{
public:
	RenderContext();
	virtual ~RenderContext();

	/// Inlines:
	/// Creates the RenderSet to be used by this context.
	/// \param bSorted  - If true creates a render set with sorting
	/// \param SortFunc - A function pointer to the method to be used for sorting.
	///                    This parameter is ignored if bSorted is false.
	inline void CreateRenderSet(bool bSorted = false,
		bool(*SortFunc)(RenderNode *left, RenderNode *right) = 0)
	{
		// If you support sorting...
		/*if (bSorted)
		{
			renderSetPtr = new RenderSetSorted;
			((RenderSetSorted *)renderSetPtr)->SortFunc = SortFunc;
		}
		else*/
			renderSetPtr = new RenderSet;
	}


	/// Adds a RenderNode to the RenderSet of this context.  This should be called
	/// on all RenderNodes that should be rendered with this context each frame.
	/// \param nodePtr - The RenderNode to be rendered in this batch.
	inline void AddRenderNode(RenderNode *nodePtr)
	{
		renderSetPtr->AddRenderNode(nodePtr);
		nodePtr->SetParent(this);
	}

	/// Clears the RenderSet of this context.
	inline void ClearRenderSet(void) { renderSetPtr->ClearRenderSet(); }

	// Multiple different RenderFunc's will likely be needed to support 
	// different features. For the RenderContext, you will likely need a different 
	// method for each vertex type supported, as well as for other feature support such as
	// transparent rendering.
	static void ContextStubRenderFunc(RenderNode &node);


protected:

	/// The linked list of RenderNodes to be rendered using this context.
	/// This is typically filled just RenderMaterial instances.
	RenderSet *renderSetPtr;

	// storage for any "state" objects that this RenderContext will apply. This will 
	// typically be the shader if nothing else.
};

