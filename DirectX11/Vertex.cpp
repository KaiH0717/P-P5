#include "Vertex.h"

constexpr unsigned int VertexLayout::LayoutElement::SizeOf(LayoutElementType type)
{
	switch (type)
	{
	case LayoutElementType::Position:
		return sizeof(Layout<LayoutElementType::Position>::LayoutDataType);
	case LayoutElementType::Texture:
		return sizeof(Layout<LayoutElementType::Texture>::LayoutDataType);
	case LayoutElementType::Normal:
		return sizeof(Layout<LayoutElementType::Normal>::LayoutDataType);
	case LayoutElementType::Tangent:
		return sizeof(Layout<LayoutElementType::Tangent>::LayoutDataType);
	case LayoutElementType::Bitangent:
		return sizeof(Layout<LayoutElementType::Bitangent>::LayoutDataType);
	case LayoutElementType::Color:
		return sizeof(Layout<LayoutElementType::Color>::LayoutDataType);
	}
}

VertexLayout::LayoutElementType VertexLayout::LayoutElement::GetType() const { return type; }
unsigned int VertexLayout::LayoutElement::GetOffset() const { return offset; }
unsigned int VertexLayout::LayoutElement::GetLayoutDataTypeSize() const { return SizeOf(type); }

void VertexLayout::Append(LayoutElementType type, unsigned int offset)
{
	LayoutElement element(type, offset);

}
