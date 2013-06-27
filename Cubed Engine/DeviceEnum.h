#ifndef _DEVICE_ENUM_
#define _DEVICE_ENUM_

class DeviceEnum
{
public:
	std::vector <IDXGIAdapter*> Enumerate(  );
};

extern DeviceEnum *g_deviceEnumeration;

#endif