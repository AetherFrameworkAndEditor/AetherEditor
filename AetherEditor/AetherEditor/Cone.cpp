#include "Cone.h"
#include"MathUtility.h"
#include"VertexType.h"
using namespace aetherClass;
Cone::Cone()
{
	SetName("Cone");
	m_slice = 20;
}


Cone::~Cone()
{
}

Cone::Cone(float slice){
	m_slice = slice;
}

bool Cone::InitializeBuffers(){

	VertexType* vertices = nullptr;
	unsigned long* indices = nullptr;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	if (m_slice < 3){
		m_slice = 3;
	}
	m_vertexCount = ((m_slice + 1) * 2);
	m_indexCount = (m_slice * 4 + 5);


	vertices = new  VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	float downpoly_uvCenter = 1.0 / 4;
	float x, z;
	int cr = 0, vertex = 0;
	int indexoffset = m_slice + 1;



	vertices[vertex]._position = Vector3(0.0f, 1.0f, 0.0f);
	vertices[vertex]._uv = Vector2(downpoly_uvCenter, downpoly_uvCenter);
	vertices[vertex]._normal = Vector3(0, 1, 0);

	vertices[vertex + indexoffset]._position = Vector3(0.0f, -1.0f, 0.0f);
	vertices[vertex + indexoffset]._uv = Vector2(downpoly_uvCenter, downpoly_uvCenter);
	vertices[vertex + indexoffset]._normal = Vector3(0, -1, 0);

	++vertex;
	{
		float onceRadian = 360 / m_slice;
		for (float rad = 0; rad < 360; rad += onceRadian){
			x = sin(kAetherPI*rad / 180);
			z = cos(kAetherPI*rad / 180);

			//ã
			vertices[vertex]._position = Vector3(x, -1, z);
			vertices[vertex]._uv = Vector2(downpoly_uvCenter + (x*downpoly_uvCenter), downpoly_uvCenter + (z*downpoly_uvCenter));
			vertices[vertex]._normal = Vector3(0, -1, 0);


			//‘¤–Ê
			vertices[vertex + indexoffset]._position = Vector3(x, -1, z);
			vertices[vertex + indexoffset]._uv = Vector2(downpoly_uvCenter + (x*downpoly_uvCenter), downpoly_uvCenter + (z*downpoly_uvCenter));
			vertices[vertex + indexoffset]._normal = Vector3(x, 1, z);

			++vertex;
		}

		for (int i = 1; i < m_slice + 1; ++i){
			indices[cr++] = i;
			indices[cr++] = 0;
		}
		indices[cr++] = 1;
		indices[cr++] = 1;
		indices[cr++] = vertex;
		indices[cr++] = vertex;

		for (int i = m_slice; i > 0; --i){
			indices[cr++] = i + indexoffset;
			indices[cr++] = indexoffset;

		}
		indices[cr++] = indexoffset + m_slice;

	}

	// Set up the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)*m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer
	result = GetDirect3DManager()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)*m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer
	result = GetDirect3DManager()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Relese the arrays now that the vertex and index buffers have been created and loaded
	delete[] vertices;
	delete[] indices;
	return true;
}

void Cone::FinalizeBuffers(){

}

void Cone::RenderBuffers(){
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered
	GetDirect3DManager()->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered
	GetDirect3DManager()->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer,in this case triangle
	GetDirect3DManager()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}