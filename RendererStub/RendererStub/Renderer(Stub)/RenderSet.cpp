#include "stdafx.h"
#include "RenderSet.h"

RenderSet::RenderSet(void)
{
	ClearRenderSet();
}

void RenderSet::ClearRenderSet(void)
{
	headPtr = tailPtr = 0;
}

void RenderSet::AddRenderNode(RenderNode *nodePtr)
{
	nodePtr->SetNext(0);
	// Empty list
	if (0 == headPtr)
	{
		headPtr = nodePtr;
	}
	else
	{
		tailPtr->SetNext(nodePtr);
	}
	tailPtr = nodePtr;
}