#ifndef luda_texture2d_h
#define luda_texture2d_h

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <raylib.h>
#include <stdio.h>
#include <string.h>

#define META_TEXTURE "texture"

Texture *to_texture(lua_State *L, int index) {
  Texture *vec = (Texture *) lua_touserdata(L, index);
  if (vec == NULL) luaL_typeerror(L, index, META_TEXTURE);
  return vec;
}

Texture *check_texture(lua_State *L, int index) {
  luaL_checktype(L, index, LUA_TUSERDATA);
  Texture *texture = (Texture *) luaL_checkudata(L, index, META_TEXTURE);
  if (texture == NULL) luaL_typeerror(L, index, META_TEXTURE);
  return texture;
}

Texture *push_texture(lua_State *L, Texture value) {
  Texture *texture = (Texture *) lua_newuserdata(L, sizeof(Texture));
  luaL_getmetatable(L, META_TEXTURE);
  lua_setmetatable(L, -2);

  memcpy(texture, &value, sizeof(Texture));

  return texture;
}

int texture_new(lua_State *L) {
  int n = lua_gettop(L);
  luaL_argcheck(L, n == 1, 1, "expected fileName");

  const char *fileName = lua_tostring(L, 1);
  Texture texture = LoadTexture(fileName);
  push_texture(L, texture);

  return 1;
}

int texture_tostring(lua_State *L) {
  Texture *texture = check_texture(L, 1);
  const char *text = TextFormat("texture{id: %d, width: %d, height: %d}", texture->id, texture->width, texture->height);
  // TODO: do i need strlen?
  lua_pushlstring(L, text, strlen(text));
  return 1;
}

int texture_shape(lua_State *L) {
  Texture *texture = check_texture(L, 1);
  lua_pushnumber(L, texture->width);
  lua_pushnumber(L, texture->height);
  return 2;
}

const luaL_Reg texture_functions[] = {{"new", texture_new}, {NULL, NULL}};

const luaL_Reg texture_methods[] = {{"shape", texture_shape}, {0, 0}};

const luaL_Reg texture_meta[] = {{"__tostring", texture_tostring}, {0, 0}};

void texture_register(lua_State *L) {
  lua_newtable(L);

  luaL_setfuncs(L, texture_functions, 0);

  luaL_newmetatable(L, META_TEXTURE);
  luaL_setfuncs(L, texture_meta, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, texture_methods, 0);

  lua_pop(L, 1);

  lua_setglobal(L, META_TEXTURE);
}

#endif
