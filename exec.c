void ExecuteScript2(DWORD rL, std::string Script) {
	DWORD xD = r_lua_newthread(rL);

	Script = "spawn(function() script = Instance.new('LocalScript') script.Disabled = true script.Parent = nil\r\n" + Script + "\r\nend)";
	lua_getglobal(L, "string");
	lua_getfield(L, -1, "dump");
	lua_pushvalue(L, -3);
	lua_pcall(L, 1, -1, 0);
	lua_getglobal(L, "string");
	lua_getfield(L, -1, "len");
	lua_pushvalue(L, -3);
	lua_pcall(L, 1, -1, 0);
	const char* DumpedFunc = lua_tostring(L, -3);
	int Len = lua_tonumber(L, -1);
	r_lua_getfield(rL, LUA_GLOBALSINDEX, "execute");
	r_lua_pushlstring(rL, DumpedFunc, Len);
	r_lua_pcall(rL, 1, -1, 0);
	if (luaL_loadbuffer(L, Script.c_str(), Script.size(), "@ThunderAPI"))
	{
		const std::string errr = lua_tostring(L, -1);
		lua_close(L);
		throw std::exception(errr.c_str());
	}
	std::string Serialized;
	LClosure* VanillaClosure = (LClosure*)lua_topointer(L, -1);
	Proto* RobloxProto = LuauTranspiler::convert(L, VanillaClosure->p);
	Closure* RobloxClosure = luaF_newLclosure(L, RobloxProto->nups, hvalue(gt(L)));
	RobloxClosure->l.p = RobloxProto;
	for (int i = 0; i < RobloxProto->nups; i++) {
		RobloxClosure->l.upvals[i] = luaF_newupval(L);
	}
	lua_pop(L, 1);
	RL_CLOSURE_OBFUSCATE(&RobloxClosure->l.p, RobloxProto);
	pushlclosure(RobloxClosure->l);
	std::cout << r_lua_type(rL, -1) << std::endl;
	r_lua_deserialize(xD, ExploitName, LuauTranspiler::compile(lua_open(), Script).c_str(), LuauTranspiler::compile(lua_open(), Script).size());
	r_lua_spawn(xD);
}
#define RL_CLOSURE_OBFUSCATE(p, v) *(int*)(int)(p) = (int)(v) - (int)(p)
