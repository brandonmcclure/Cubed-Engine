//-----------------------------------------------------------------------------
// DeviceEnum.h implementation.
// Refer to the DeviceEnum.h interface for more details.
//
// Programming a Multiplayer First Person Shooter in DirectX
// Copyright (c) 2004 Vaughan Young
//-----------------------------------------------------------------------------
#include "FPSEngine.h"

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
DeviceEnum *g_deviceEnumeration = NULL;


std::vector <IDXGIAdapter*> DeviceEnum::Enumerate( )
{
	IDXGIAdapter * pAdapter; 
    std::vector <IDXGIAdapter*> vAdapters; 
    IDXGIFactory* pFactory = NULL; 
    

    // Create a DXGIFactory object.
    if(FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory) ,(void**)&pFactory)))
    {
        return vAdapters;
    }


    for ( UINT i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i )
    {
        vAdapters.push_back(pAdapter); 
    } 


    if(pFactory)
    {
        pFactory->Release();
    }

    return vAdapters;
}

