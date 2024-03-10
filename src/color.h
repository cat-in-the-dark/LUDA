#ifndef luda_color_h
#define luda_color_h

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <raylib.h>
#include <stdio.h>
#include <string.h>

#define META_COLOR "color"

Color *to_color(lua_State *L, int index) {
  Color *vec = (Color *) lua_touserdata(L, index);
  if (vec == NULL) luaL_typeerror(L, index, META_COLOR);
  return vec;
}

Color *check_color(lua_State *L, int index) {
  luaL_checktype(L, index, LUA_TUSERDATA);
  Color *color = (Color *) luaL_checkudata(L, index, META_COLOR);
  if (color == NULL) luaL_typeerror(L, index, META_COLOR);
  return color;
}

Color *push_color(lua_State *L, Color value) {
  Color *color = (Color *) lua_newuserdata(L, sizeof(Color));
  luaL_getmetatable(L, META_COLOR);
  lua_setmetatable(L, -2);

  memcpy(color, &value, sizeof(Color));

  return color;
}

unsigned char to_byte(double v) {
  return (char) v % 255;
}

int color_new(lua_State *L) {
  double r = luaL_optnumber(L, 1, 0);
  double g = luaL_optnumber(L, 2, 0);
  double b = luaL_optnumber(L, 3, 0);
  double a = luaL_optnumber(L, 4, 255);
  Color c = {to_byte(r), to_byte(g), to_byte(b), to_byte(a)};
  push_color(L, c);
  return 1;
}

int color_tostring(lua_State *L) {
  Color *color = check_color(L, 1);
  const char *text = TextFormat("color{%d, %d, %d, %d}", color->r, color->g, color->b, color->a);
  // TODO: do i need strlen?
  lua_pushlstring(L, text, strlen(text));
  return 1;
}

const luaL_Reg color_functions[] = {{"new", color_new}, {NULL, NULL}};

const luaL_Reg color_methods[] = {{0, 0}};

const luaL_Reg color_meta[] = {{"__tostring", color_tostring}, {0, 0}};

void color_register(lua_State *L) {
  lua_newtable(L);

  luaL_setfuncs(L, color_functions, 0);

  luaL_newmetatable(L, META_COLOR);
  luaL_setfuncs(L, color_meta, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, color_methods, 0);

  lua_pop(L, 1);

  push_color(L, RED);
  lua_setfield(L, -2, "RED");

  push_color(L, RAYWHITE);
  lua_setfield(L, -2, "RAYWHITE");

  lua_setglobal(L, META_COLOR);
}

#endif
