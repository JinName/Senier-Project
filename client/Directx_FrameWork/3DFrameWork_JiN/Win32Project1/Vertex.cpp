#include "Vertex.h"


CVertex::CVertex()
	:m_pVB(NULL)
{
}


CVertex::~CVertex()
{
}


HRESULT CVertex::InitVB(LPDIRECT3DDEVICE9 _d3dDevice)
{
	// Initialize three Vertices for rendering a triangle
	CUSTOMVERTEX Vertices[] =
	{
		{ -1.0f,-1.0f, 0.0f, 0xffff0000, },
		{ 1.0f,-1.0f, 0.0f, 0xff0000ff, },
		{ 0.0f, 1.0f, 0.0f, 0xffffffff, },
	};

	// Create the vertex buffer. Here we are allocating enough memory
	// (from the default pool) to hold all our 3 custom Vertices. We also
	// specify the FVF, so the vertex buffer knows what data it contains.
	if (FAILED(_d3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return E_FAIL;
	}

	// Now we fill the vertex buffer. To do this, we need to Lock() the VB to
	// gain access to the Vertices. This mechanism is required becuase vertex
	// buffers may be in device memory.
	VOID* pVertices;
	if (FAILED(m_pVB->Lock(0, sizeof(Vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, Vertices, sizeof(Vertices));
	m_pVB->Unlock();

	return S_OK;
}

void CVertex::Release()
{
	if (m_pVB != NULL)
		m_pVB->Release();
}