#include <fbxsdk.h>
#include <vector>

using namespace std;

struct ExportData
{
	float pos[3];
};

#pragma region Function Prototypes

bool ImportExportData(char* importFilename, char* exportFilename);
void ProcessMesh(FbxNode* inNode);

#pragma endregion

bool ImportExportData(char* importFilename, char* exportFilename)
{
	// Create the FBX SDK manager
	FbxManager* SDKManager = FbxManager::Create();

	// Create an IOSettings object.
	FbxIOSettings * ios = FbxIOSettings::Create(SDKManager, IOSROOT);
	SDKManager->SetIOSettings(ios);

	// ... Configure the FbxIOSettings object ...

	// Create an importer.
	FbxImporter* importer = FbxImporter::Create(SDKManager, "");

	// Initialize the importer.
	bool lImportStatus = importer->Initialize(importFilename, -1, SDKManager->GetIOSettings());

	if (!lImportStatus) 
	{
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", importer->GetStatus().GetErrorString());
		return false;
	}

	// Create a new scene so it can be populated by the imported file.
	FbxScene* scene = FbxScene::Create(SDKManager, "myScene");

	// Import the contents of the file into the scene.
	importer->Import(scene);

	// The file has been imported; we can get rid of the importer.
	importer->Destroy();

	FbxNode* rootNode = scene->GetRootNode();

	vector<ExportData> exportData;

	if (rootNode)
	{
		for (int i = 0; i < rootNode->GetChildCount(); i++)
		{
			FbxNode* childNode = rootNode->GetChild(i);

			if (childNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = childNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			FbxMesh* mesh = (FbxMesh*)childNode->GetNodeAttribute();

			FbxVector4* vertices = mesh->GetControlPoints();

			int numTriangles = mesh->GetPolygonCount();

			exportData.reserve(numTriangles);

			for (int j = 0; j < numTriangles; j++)
			{
				int numVerts = mesh->GetPolygonSize(j);

				if (numVerts == 3)
				{
					for (int k = 0; k < numVerts; k++)
					{
						int iControlPointIndex = mesh->GetPolygonVertex(j, k);

						ExportData vertex;
						vertex.pos[0] = (float)vertices[iControlPointIndex].mData[0];
						vertex.pos[1] = (float)vertices[iControlPointIndex].mData[1];
						vertex.pos[2] = (float)vertices[iControlPointIndex].mData[2];
						exportData.push_back(vertex);
					}
				}
			}
		}
	}
	return true;
}
//
//void ProcessMesh(FbxNode* inNode)
//{
//	FbxMesh* currMesh = inNode->GetMesh();
//	mTriangleCount = currMesh->GetPolygonCount();
//	int vertexCounter = 0;
//	mTriangles.reserve(mTriangleCount);
//
//	for (unsigned int i = 0; i < mTriangleCount; ++i)
//	{
//		XMFLOAT3 normal[3];
//		XMFLOAT3 tangent[3];
//		XMFLOAT3 binormal[3];
//		XMFLOAT2 UV[3][2];
//		Triangle currTriangle;
//		mTriangles.push_back(currTriangle);
//
//		for (unsigned int j = 0; j < 3; ++j)
//		{
//			int ctrlPointIndex = currMesh->GetPolygonVertex(i, j);
//			CtrlPoint* currCtrlPoint = mControlPoints[ctrlPointIndex];
//
//
//			ReadNormal(currMesh, ctrlPointIndex, vertexCounter, normal[j]);
//			// We only have diffuse texture
//			for (int k = 0; k < 1; ++k)
//			{
//				ReadUV(currMesh, ctrlPointIndex, currMesh->GetTextureUVIndex(i, j), k, UV[j][k]);
//			}
//
//
//			PNTIWVertex temp;
//			temp.mPosition = currCtrlPoint->mPosition;
//			temp.mNormal = normal[j];
//			temp.mUV = UV[j][0];
//			// Copy the blending info from each control point
//			for (unsigned int i = 0; i < currCtrlPoint->mBlendingInfo.size(); ++i)
//			{
//				VertexBlendingInfo currBlendingInfo;
//				currBlendingInfo.mBlendingIndex = currCtrlPoint->mBlendingInfo[i].mBlendingIndex;
//				currBlendingInfo.mBlendingWeight = currCtrlPoint->mBlendingInfo[i].mBlendingWeight;
//				temp.mVertexBlendingInfos.push_back(currBlendingInfo);
//			}
//			// Sort the blending info so that later we can remove
//			// duplicated vertices
//			temp.SortBlendingInfoByWeight();
//
//			mVertices.push_back(temp);
//			mTriangles.back().mIndices.push_back(vertexCounter);
//			++vertexCounter;
//		}
//	}
//
//	// Now mControlPoints has served its purpose
//	// We can free its memory
//	for (auto itr = mControlPoints.begin(); itr != mControlPoints.end(); ++itr)
//	{
//		delete itr->second;
//	}
//	mControlPoints.clear();
//}