#pragma once


class RenderNode;
/// The signature of render functions to be used by RenderNode instances.
typedef void(*EDRenderFunc)(RenderNode &node);

/// The base render object.
/// RenderNode is the base class for all renderable objects that the Renderer can deal with.
class RenderNode
{
protected:
	/// The nextPtr is used to create a linked list of renderable objects
	RenderNode *nextPtr;

	/// The parent of this node
	RenderNode *parentPtr;

public:
	/// Default constructor
	RenderNode() : nextPtr(nullptr), parentPtr(nullptr) { }

	/// A pointer to the function to be used for rendering this renderable object.
	/// Each renderable component must set this function pointer to a function 
	/// that should be used to render it.
	EDRenderFunc RenderFunc;

	/// RenderProcess will be called for all batches of RenderNodes passed to the Renderer.
	/// This method invokes the function pointer RenderFunc.  It is also the only spot to 
	/// add code that will be run on all RenderNodes to be rendered, such as debug tracking.
	void RenderProcess()
	{
		RenderFunc(*this);
	}

	/// Returns the linked list next pointer
	inline RenderNode *GetNext(void) { return nextPtr; }
	/// Sets the linked list next pointer
	inline void SetNext(RenderNode *nodePtr) { nextPtr = nodePtr; }

	/// Returns the parent pointer
	inline RenderNode *GetParent(void) { return parentPtr; }
	/// Sets the parent pointer
	inline void SetParent(RenderNode *nodePtr) { parentPtr = nodePtr; }
};
