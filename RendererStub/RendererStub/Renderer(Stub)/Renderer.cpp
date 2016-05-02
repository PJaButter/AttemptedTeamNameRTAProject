#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::Render(RenderSet &set)
{
	RenderNode *pCurrent = set.GetHead();

	while (0 != pCurrent)
	{
		pCurrent->RenderProcess();
		pCurrent = pCurrent->GetNext();
	}
}