#pragma once
#include "RenderNode.h"

// Note: You will need to either extend or inherit from this class to support
// sorting of render batches. Sorting of batches would be needed to properly
// support rendering of transparent objects. Your set of RenderSahpes would need
// to be sorted based on depth. Insertion sort could be used for this, or possibly
// merge support could be a better alternative if the data set will be large enough.


/// Container class defines a render batch.
class RenderSet
{
	friend class Renderer;
protected:
	/// The first item in the render batch.
	RenderNode *headPtr;
	/// The last item in the render batch.
	RenderNode *tailPtr;
public:
	/// Initializes the head and tail pointers to a default state.
	RenderSet(void);
	virtual ~RenderSet(void) {}

	/// Adds a RenderNode to the render batch.
	/// \param nodePtr - The RenderNode to add to the render batch
	virtual void AddRenderNode(RenderNode *nodePtr);

	/// Sets the head and tail pointers to a default state.
	void ClearRenderSet(void);

	/// \return Returns pointer to the first item in the render batch.
	RenderNode *GetHead() { return headPtr; }

	void AddRenderNodeSolution(RenderNode *nodePtr);
	void ClearRenderSetSolution(void);
};
