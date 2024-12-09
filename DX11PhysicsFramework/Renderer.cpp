#include "Renderer.h"

/// <summary>
/// Constructor
/// </summary>
/// <param name="geometry">Object Mesh Information</param>
/// <param name="material">Object Material Information</param>
Renderer::Renderer()
{
	//_geometry = geometry;
	//_material = material;
	_textureRV = nullptr;
}

/// <summary>
/// Destructor
/// </summary>
Renderer::~Renderer()
{
	if (_textureRV != nullptr)
	{
		delete _textureRV;
		_textureRV = nullptr;
	}

	delete _geometry.indexBuffer;
	delete _geometry.vertexBuffer;

	_geometry.indexBuffer = nullptr;
	_geometry.vertexBuffer = nullptr;
}



/// <summary>
/// Renders Game objects. RUNS AFTER MAIN FRAMEWORK DRAW SETUP
/// </summary>
/// <param name="pImmediateContext">Copy of the immediate context to draw the objects to</param>
void Renderer::Draw(ID3D11DeviceContext* pImmediateContext)
{

	// Set vertex and index buffers
	pImmediateContext->IASetVertexBuffers(0, 1, &_geometry.vertexBuffer, &_geometry.vertexBufferStride, &_geometry.vertexBufferOffset);
	pImmediateContext->IASetIndexBuffer(_geometry.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//Draw object
	pImmediateContext->DrawIndexed(_geometry.numberOfIndices, 0, 0);
}