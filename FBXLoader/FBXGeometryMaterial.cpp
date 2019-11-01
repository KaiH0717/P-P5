#include "FBXGeometryMaterial.h"
#include "FBXUtility.h"
#include <string>

void FBXGeometryMaterial::Load(FbxGeometry* pGeometry)
{
	int materialCount = 0;
	FbxNode* node = nullptr;
	node = pGeometry->GetNode();
	if (node)
	{
		materialCount = node->GetMaterialCount();
	}

	FBXUtility::DisplayInt("Material Count: ", materialCount);
	if (materialCount > 0)
	{
		FbxPropertyT<FbxDouble3> fbxDouble3;
		FbxPropertyT<FbxDouble> fbxDouble;
		FbxColor color;
		FbxFileTexture* fileTexture = nullptr;
		std::string fileName;

		for (int materialIndex = 0; materialIndex < materialCount; ++materialIndex)
		{
			FbxString string = "Material[";
			string += materialIndex;
			string += ']';
			FBXUtility::DisplayString(string);

			FbxSurfaceMaterial* material = node->GetMaterial(materialIndex);
			if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				FbxSurfacePhong* phong = (FbxSurfacePhong*)material;
				FileMaterial fbxMaterial;
				// Display the Ambient Color
				fbxDouble3 = ((FbxSurfacePhong*)material)->Ambient;
				color.Set(fbxDouble3.Get()[0], fbxDouble3.Get()[1], fbxDouble3.Get()[2], phong->AmbientFactor.Get());
				FBXUtility::DisplayMaterialComponent("\tAmbient: \t", color);
				fileTexture = phong->Ambient.GetSrcObject<FbxFileTexture>();
				if (fileTexture)
				{
					fileName = fileTexture->GetRelativeFileName();
					fbxMaterial[Phong::Ambient].color = XMFLOAT3((float)fbxDouble3.Get()[0], (float)fbxDouble3.Get()[1], (float)fbxDouble3.Get()[2]);
					fbxMaterial[Phong::Ambient].factor = (float)phong->AmbientFactor.Get();
					strcpy_s(fbxMaterial[Phong::Ambient].filePath.data(), sizeof(FilePath), fileName.data());
					fbxMaterial[Phong::Ambient].fileSize = fileName.size();
				}

				// Display the Diffuse Color
				fbxDouble3 = ((FbxSurfacePhong*)material)->Diffuse;
				color.Set(fbxDouble3.Get()[0], fbxDouble3.Get()[1], fbxDouble3.Get()[2], phong->DiffuseFactor.Get());
				FBXUtility::DisplayMaterialComponent("\tDiffuse: \t", color);
				fileTexture = phong->Diffuse.GetSrcObject<FbxFileTexture>();
				if (fileTexture)
				{
					fileName = fileTexture->GetRelativeFileName();
					fbxMaterial[Phong::Diffuse].color = XMFLOAT3((float)fbxDouble3.Get()[0], (float)fbxDouble3.Get()[1], (float)fbxDouble3.Get()[2]);
					fbxMaterial[Phong::Diffuse].factor = (float)phong->DiffuseFactor.Get();
					strcpy_s(fbxMaterial[Phong::Diffuse].filePath.data(), sizeof(FilePath), fileName.data());
					fbxMaterial[Phong::Diffuse].fileSize = fileName.size();
				}

				// Display the Specular Color (unique to Phong materials)
				fbxDouble3 = ((FbxSurfacePhong*)material)->Specular;
				color.Set(fbxDouble3.Get()[0], fbxDouble3.Get()[1], fbxDouble3.Get()[2], phong->SpecularFactor.Get());
				FBXUtility::DisplayMaterialComponent("\tSpecular: \t", color);
				fileTexture = phong->Specular.GetSrcObject<FbxFileTexture>();
				if (fileTexture)
				{
					fileName = fileTexture->GetRelativeFileName();
					fbxMaterial[Phong::Specular].color = XMFLOAT3((float)fbxDouble3.Get()[0], (float)fbxDouble3.Get()[1], (float)fbxDouble3.Get()[2]);
					fbxMaterial[Phong::Specular].factor = (float)phong->SpecularFactor.Get();
					strcpy_s(fbxMaterial[Phong::Specular].filePath.data(), sizeof(FilePath), fileName.data());
					fbxMaterial[Phong::Specular].fileSize = fileName.size();
				}

				// Display the Emissive Color
				fbxDouble3 = ((FbxSurfacePhong*)material)->Emissive;
				color.Set(fbxDouble3.Get()[0], fbxDouble3.Get()[1], fbxDouble3.Get()[2], phong->EmissiveFactor.Get());
				FBXUtility::DisplayMaterialComponent("\tEmissive: \t", color);
				fileTexture = phong->Emissive.GetSrcObject<FbxFileTexture>();
				if (fileTexture)
				{
					fileName = fileTexture->GetRelativeFileName();
					fbxMaterial[Phong::Emissive].color = XMFLOAT3((float)fbxDouble3.Get()[0], (float)fbxDouble3.Get()[1], (float)fbxDouble3.Get()[2]);
					fbxMaterial[Phong::Emissive].factor = (float)phong->EmissiveFactor.Get();
					strcpy_s(fbxMaterial[Phong::Emissive].filePath.data(), sizeof(FilePath), fileName.data());
					fbxMaterial[Phong::Emissive].fileSize = fileName.size();
				}

				//Opacity is Transparency factor now
				fbxDouble = ((FbxSurfacePhong*)material)->TransparencyFactor;
				FBXUtility::DisplayDouble("\tOpacity: \t", 1.0 - fbxDouble.Get(), " (factor)");
				fbxMaterial[Phong::Opacity].factor = (float)(1.0 - fbxDouble.Get());

				// Display the Shininess
				fbxDouble = ((FbxSurfacePhong*)material)->Shininess;
				FBXUtility::DisplayDouble("\tShininess: \t", fbxDouble.Get(), " (factor)");
				fbxMaterial[Phong::Shininess].factor = (float)fbxDouble.Get();

				// Display the Reflectivity
				fbxDouble = ((FbxSurfacePhong*)material)->ReflectionFactor;
				FBXUtility::DisplayDouble("\tReflectivity: \t", fbxDouble.Get(), " (factor)");
				fbxMaterial[Phong::Reflectivity].factor = (float)fbxDouble.Get();

				// add material
				fbxData.fbxMaterials.push_back(fbxMaterial);
			}
			else if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfaceLambert* lambert = (FbxSurfaceLambert*)material;
				// Display the Ambient Color
				fbxDouble3 = ((FbxSurfaceLambert*)material)->Ambient;
				color.Set(fbxDouble3.Get()[0], fbxDouble3.Get()[1], fbxDouble3.Get()[2], lambert->AmbientFactor.Get());
				FBXUtility::DisplayMaterialComponent("\tAmbient: \t", color);

				// Display the Diffuse Color
				fbxDouble3 = ((FbxSurfaceLambert*)material)->Diffuse;
				color.Set(fbxDouble3.Get()[0], fbxDouble3.Get()[1], fbxDouble3.Get()[2], lambert->DiffuseFactor.Get());
				FBXUtility::DisplayMaterialComponent("\tDiffuse: \t", color);

				// Display the Emissive
				fbxDouble3 = ((FbxSurfaceLambert*)material)->Emissive;
				color.Set(fbxDouble3.Get()[0], fbxDouble3.Get()[1], fbxDouble3.Get()[2], lambert->EmissiveFactor.Get());
				FBXUtility::DisplayMaterialComponent("\tEmissive: \t", color);

				// Display the Opacity
				fbxDouble = ((FbxSurfaceLambert*)material)->TransparencyFactor;
				FBXUtility::DisplayDouble("\tOpacity: \t", 1.0 - fbxDouble.Get(), " (factor)");
			}
			else
				FBXUtility::DisplayString("Unknown type of Material");
		}
	}
}
