#ifndef _LUA_SCRIPT_
#define _LUA_SCRIPT_

enum{ VARIABLE_BOOL, VARIABLE_COLOUR, VARIABLE_FLOAT, VARIABLE_NUMBER, VARIABLE_STRING, VARIABLE_VECTOR, VARIABLE_UNKNOWN };

class LuaScript : public Resource< LuaScript >
{
	public:
	LuaScript( char *name, char *path = "./" );
	virtual ~LuaScript();

	bool *GetBoolData( char *variable );
	D3DXCOLOR GetColourData( char *variable );
	float GetFloatData( char *variable );
	long GetNumberData( char *variable );
	char *GetStringData( char *variable );
	XMFLOAT3  GetVectorData( char *variable );
	VertexPos GetVertxPosData( char *variable );
	void *GetUnknownData( char *variable );

	float LuaScript::GetField (const char *key);


private:
	lua_State *L;
};
#endif