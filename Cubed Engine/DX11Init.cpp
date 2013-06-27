#include "FPSEngine.h"

DX11Init *_DX11Init = NULL;

HRESULT DX11Init::Initialize()
{
	// Incomplete
	g_deviceEnumeration = new DeviceEnum;
	g_deviceEnumeration->Enumerate();
	
	//----- ----- Base DX11 Init ----- -----//
	RECT dimensions;
    GetClientRect( _myEngine->GetWindow(), &dimensions );
	unsigned int width = dimensions.right - dimensions.left;
    unsigned int height = dimensions.bottom - dimensions.top;

	_myEngine->SetSCREEN_HEIGHT( height );
	_myEngine->SetSCREEN_WIDTH( width );

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
    };

    unsigned int totalDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    unsigned int totalFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = _myEngine->GetWindow();
    swapChainDesc.Windowed = true;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    unsigned int creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    unsigned int driver = 0;

    for( driver = 0; driver < totalDriverTypes; ++driver )
    {
		myResult = D3D11CreateDeviceAndSwapChain( 0, driverTypes[ driver ], 0, creationFlags, featureLevels, totalFeatureLevels, 
			D3D11_SDK_VERSION, &swapChainDesc, &_myEngine->swapChain_, &_myEngine->d3dDevice_, &featureLevel_, &_myEngine->d3dContext_ );

        if( SUCCEEDED( myResult ) )
        {
            driverType_ = driverTypes[driver];
            break;
        }
    }

    if( FAILED( myResult ) )
    {
		#ifdef _DEBUG
			MessageBox(0,"Failed to create the Direct3D device!","Compile Error", MB_OK );
		#endif
        
        return false;
    }

	  // create the depth buffer texture
		D3D11_TEXTURE2D_DESC texd;
		ZeroMemory(&texd, sizeof(texd));

		texd.Width = _myEngine->SCREEN_WIDTH();
		texd.Height = _myEngine->SCREEN_HEIGHT();
		texd.ArraySize = 1;
		texd.MipLevels = 1;
		texd.SampleDesc.Count = _myEngine->MUTI_SAMPLE_COUNT();
		texd.Format = DXGI_FORMAT_D32_FLOAT;
		texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		ID3D11Texture2D *pDepthBuffer;
		_myEngine->GetD3DDevice()->CreateTexture2D(&texd, NULL, &pDepthBuffer);

		 // create the depth buffer
		 D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));

		dsvd.Format = DXGI_FORMAT_D32_FLOAT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

		_myEngine->GetD3DDevice()->CreateDepthStencilView(pDepthBuffer, &dsvd, &_myEngine->zBuffer_);
		pDepthBuffer->Release();

    ID3D11Texture2D* backBufferTexture;

    myResult = _myEngine->GetSwapChain()->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBufferTexture );

    if( FAILED( myResult ) )
    {
		#ifdef _DEBUG
			MessageBox(0,"Failed to get the swap chain back buffer!","Compile Error", MB_OK );
		#endif
       
        return false;
    }

	myResult = _myEngine->GetD3DDevice()->CreateRenderTargetView( backBufferTexture, 0, &_myEngine->backBufferTarget_ );

    if( backBufferTexture )
        backBufferTexture->Release( );

    if( FAILED( myResult ) )
    {
		#ifdef _DEBUG
			MessageBox(0,"Failed to create the render target view!","Compile Error", MB_OK );
		#endif
       
        return false;
    }

    _myEngine->GetD3DContext()->OMSetRenderTargets( 1, &_myEngine->backBufferTarget_, _myEngine->GetZBuffer() );

    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

    _myEngine->GetD3DContext()->RSSetViewports( 1, &viewport );

	//----- ----- ------ ----- Start Vertex Shader Init ----- ----- ----- -----//
	ID3DBlob* vsBuffer = 0;
	//Compiles the DX shader
	myResult = CompileD3DShader( "SolidGreenColor.hlsl", "VShader", "vs_4_0", &vsBuffer );

	//Error checking for compiled shader
	if (FAILED( myResult ))
	{
	#ifdef _DEBUG
		MessageBox(0,"Error loading vertex shader.","Compile Error", MB_OK );
	#endif
		return false;
	}

	//Creates DX Vertex Shader
	myResult = _myEngine->GetD3DDevice()->CreateVertexShader( vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, &_myEngine->solidColorVS_ );

	//Error checking for sucsesfull completion of above call
	if ( FAILED( myResult ) )
	{
		if ( &vsBuffer )
		{
			
			#ifdef _DEBUG
				MessageBox(0,"d3dDevice created Vertex Shader Un-Succesfull, &vsbuffer true ","Compile Error", MB_OK );
			#endif
			vsBuffer->Release();
		}
		return false;
	}

	_myEngine->GetD3DContext()->VSSetShader( _myEngine->GetSolidColorVS(), 0, 0 );

	//----- ----- ------ ----- End Vertex Shader Init----- ----- ----- -----//

	//----- ----- ------ ----- End Pixel Shader Init----- ----- ----- -----//
	ID3DBlob* psBuffer = 0;

	myResult = CompileD3DShader( "SolidGreenColor.hlsl", "PShader", "ps_4_0", &psBuffer );

	if( FAILED( myResult ) )
	{
		MessageBox(0,"Error loading pixel shader.","Compile Error", MB_OK );
		return false;
	}

	myResult = _myEngine->GetD3DDevice()->CreatePixelShader( psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), 0, &_myEngine->solidColorPS_ );

	psBuffer->Release();

	if( FAILED( myResult ))
	{
		return false;
	}
	
	_myEngine->GetD3DContext()->PSSetShader( _myEngine->GetSolidColorPS(), 0, 0 );

	//----- ----- ------ ----- End Pixel Shader Init----- ----- ----- -----//

	//----- ----- ------ ----- Start Input Layout Init----- ----- ----- -----//

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	unsigned int totalLayoutElements = ARRAYSIZE( solidColorLayout );

	myResult = _myEngine->GetD3DDevice()->CreateInputLayout( solidColorLayout, totalLayoutElements, vsBuffer->GetBufferPointer(), 
		vsBuffer->GetBufferSize(), &_myEngine->inputLayout_ );
	vsBuffer->Release();

	if( FAILED( myResult ) )
	{
		#ifdef _DEBUG
			MessageBox(0,"d3dDevice_->CreateInputLayout un- Succesfull.","Compile Error", MB_OK );
		#endif

		return false;
	}

	_myEngine->GetD3DContext()->IASetInputLayout( _myEngine->GetInputLayout() );

	//----- ----- ------ ----- Stop Input Layout Init----- ----- ----- -----//
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	bd.ByteWidth = 64;
	_myEngine->GetD3DDevice()->CreateBuffer(&bd, NULL, &_myEngine->constantBuffer_);

	_myEngine->GetD3DContext()->VSSetConstantBuffers( 0, 1, &_myEngine->constantBuffer_);

	//_myEngine->GetScriptManager()->Add( "Meshes", "Meshes.lua" );
	//_myEngine->GetScriptManager()->GetElement( "Meshes", "Meshes.lua" )->GetVertxPosData();

	//Basic Primatives
	//TODO: Add these Primatives via a script
		VertexPos cubeVerticies[] =
		{
			 {D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
			{D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
			{D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)},
			{D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
			{D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
			{D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)},
			{D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
			{D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)}, 
		};

		DWORD cubeIndicies[] =
		{
			0, 1, 2,    // side 1
			2, 1, 3,
			4, 0, 6,    // side 2
			6, 0, 2,
			7, 5, 6,    // side 3
			6, 5, 4,
			3, 1, 7,    // side 4
			7, 1, 5,
			4, 5, 0,    // side 5
			0, 5, 1,
			3, 7, 2,    // side 6
			2, 7, 6,
		};

		_myEngine->GetPrimativeManager()->Add( "Cube", cubeVerticies, 8, cubeIndicies, 36 );

		VertexPos planeVerticies[] =
		{
			{D3DXVECTOR3(-1.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)},
			{D3DXVECTOR3(1.0f, 1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)},
			{D3DXVECTOR3(-1.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)},
			{D3DXVECTOR3(1.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)},
		};

		DWORD planeIndicies[] =
		{
			0, 1, 2,    // side 1
			2, 1, 3,
		};
		_myEngine->GetPrimativeManager()->Add( "Plane", planeVerticies, 4, planeIndicies, 6 );

	return 0;
}

HRESULT DX11Init::CompileD3DShader( char* filePath, char* entry, char* shaderModel, ID3DBlob** buffer )
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	//Does that cool debug checking and alters shaderFlags acordingly
#if defined( DEBUG ) || defined ( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* errorBuffer;

	//Attempt to comile shader
	HRESULT Result = D3DX11CompileFromFile( filePath, 0, 0, entry, shaderModel,
        shaderFlags, 0, 0, buffer, &errorBuffer, 0 );

	//Error checking for succesfull compile
	if( FAILED( myResult ) )
	{
		if( errorBuffer != 0 )
		{
			OutputDebugStringA( ( char* ) errorBuffer->GetBufferPointer() );
			errorBuffer->Release();
		}

		return myResult;
	}

	if( errorBuffer != 0 )
	{
		errorBuffer->Release();
	}

	return S_OK;
}