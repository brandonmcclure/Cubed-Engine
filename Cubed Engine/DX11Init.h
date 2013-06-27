#ifndef _DX11_INIT_
#define _DX11_INIT_

class DX11Init
{
public:
	HRESULT Initialize();

private:
	//- Functions -//
	bool CreateD3DVertexShader( char* filePath, ID3DBlob* vsBuffer );
	bool CreateD3DPixelShader( char* filePath );
	HRESULT CompileD3DShader( char* filePath, char* entry, char* shaderModel, ID3DBlob** buffer );

	//- Variables -//
	HRESULT myResult;

public:
	D3D_DRIVER_TYPE driverType_;
	D3D_FEATURE_LEVEL featureLevel_;
};

extern DX11Init *_DX11Init;

#endif