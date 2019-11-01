#pragma once
// Include DirectX11 and DirectXMath for interface and math operation access
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <vector>

class VertexLayout
{
public:
	typedef enum class LayoutElementType
	{
		Position,
		Texture,
		Normal,
		Tangent,
		Bitangent,
		Color,
		Indices,
		Weights
	};
	template<LayoutElementType> struct Layout;
	template<> struct Layout<LayoutElementType::Position>
	{
		using LayoutDataType = XMFLOAT4;
		static constexpr const char* hlslSemantic = "POSITION";
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	};
	template<> struct Layout<LayoutElementType::Texture>
	{
		using LayoutDataType = XMFLOAT2;
		static constexpr const char* hlslSemantic = "TEXCOORD";
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	};
	template<> struct Layout<LayoutElementType::Normal>
	{
		using LayoutDataType = XMFLOAT3;
		static constexpr const char* hlslSemantic = "NORMAL";
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	};
	template<> struct Layout<LayoutElementType::Tangent>
	{
		using LayoutDataType = XMFLOAT4;
		static constexpr const char* hlslSemantic = "TANGENT";
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	};
	template<> struct Layout<LayoutElementType::Bitangent>
	{
		using LayoutDataType = XMFLOAT4;
		static constexpr const char* hlslSemantic = "BITANGENT";
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	};
	template<> struct Layout<LayoutElementType::Color>
	{
		using LayoutDataType = XMFLOAT4;
		static constexpr const char* hlslSemantic = "COLOR";
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	};
	template<> struct Layout<LayoutElementType::Indices>
	{
		using LayoutDataType = XMINT4;
		static constexpr const char* hlslSemantic = "BLENDINDICES";
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_UINT;
	};
	template<> struct Layout<LayoutElementType::Weights>
	{
		using LayoutDataType = XMFLOAT4;
		static constexpr const char* hlslSemantic = "BLENDWEIGHTS";
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	};

	class LayoutElement
	{
	private:
		LayoutElementType type;
		unsigned int offset;
	private:
		template<LayoutElementType type>
		static constexpr D3D11_INPUT_ELEMENT_DESC GenerateInputElementDescription(unsigned int offset)
		{
			return { Layout<type>::hlslSemantic, 0, Layout<type>::dxgiFormat, 0, offset, D3D11_INPUT_PER_VERTEX_DATA,0 };
		}
		static constexpr unsigned int SizeOf(LayoutElementType type);
	public:
		LayoutElement(LayoutElementType type, unsigned int offset) : type(type), offset(offset) {};
		LayoutElementType GetType() const;
		unsigned int GetOffset() const;
		unsigned int GetLayoutDataTypeSize() const;
	};
private:
	std::vector<LayoutElement> elements;
public:

	void Append(LayoutElementType type, unsigned int offset);
};

class Vertex
{
private:
	VertexLayout vertexLayout;
public:
	template<VertexLayout::LayoutElementType type>
	auto& Attribute()
	{

	}
};

