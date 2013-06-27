//-----------------------------------------------------------------------------
// Scripting.h implementation.
// Refer to the Scripting.h interface for more details.
//
// Programming a Multiplayer First Person Shooter in DirectX
// Copyright (c) 2004 Vaughan Young
//-----------------------------------------------------------------------------
#include "FPSEngine.h"

LuaScript::LuaScript( char *name, char *path ) : Resource< LuaScript >( name, path )
{
	L = lua_open();
	luaL_openlibs( L );
}

LuaScript::~LuaScript()
{
	lua_close(L);
}

bool *LuaScript::GetBoolData( char *variable )
{
	return NULL;
}

D3DXCOLOR  LuaScript::GetColourData( char *variable )
{
	D3DXCOLOR  temp;

	luaL_dofile( L, this->GetPath() );
	lua_getglobal(L, variable);	

	if ( lua_istable(L, -1) )
	{
			temp.r = GetField("r");
			temp.g = GetField("g");
			temp.b = GetField("b");
			temp.a = GetField("a");
	}
	return temp;
}

float LuaScript::GetFloatData( char *variable )
{
	luaL_dofile( L, this->GetPath() );
	lua_getglobal(L, variable);
	float temp = lua_tonumber(L,-1);
	return temp;
	
}

long LuaScript::GetNumberData( char *variable )
{
	luaL_dofile( L, this->GetPath() );
	lua_getglobal(L, variable);
	long temp = lua_tonumber(L,-1);
	return temp;
}

char *LuaScript::GetStringData( char *variable )
{
	return NULL;
}

XMFLOAT3  LuaScript::GetVectorData( char *variable )
{
	XMFLOAT3 temp;

	luaL_dofile(L, this->GetPath() );
	lua_getglobal(L, variable);

	if ( lua_istable(L, -1) )
	{
		temp.x = GetField("x");
		temp.y = GetField("y");
		temp.z = GetField("z");
	}
	return temp;
}

VertexPos LuaScript::GetVertxPosData( char *variable )
{
	VertexPos temp;

	luaL_dofile(L, this->GetPath() );
	lua_getglobal(L, variable);

	return temp;
}

void *LuaScript::GetUnknownData( char *variable )
{
	return NULL;
}

float LuaScript::GetField (const char *key) 
{
      float result;

      lua_pushstring(L, key);
      lua_gettable(L, -2);  /* get background[key] */

      result = (float)lua_tonumber(L, -1);
      lua_pop(L, 1);  /* remove number */
      return result;
   }
