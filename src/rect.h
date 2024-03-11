#ifndef luda_rect_h
#define luda_rect_h

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <raylib.h>
#include <stdio.h>
#include <string.h>

#define META_RECT "rect"

Rectangle *to_rect(lua_State *L, int index) {
  Rectangle *rect = (Rectangle *) lua_touserdata(L, index);
  if (rect == NULL) luaL_typeerror(L, index, META_RECT);
  return rect;
}

Rectangle *check_rect(lua_State *L, int index) {
  Rectangle *rect;
  luaL_checktype(L, index, LUA_TUSERDATA);
  rect = (Rectangle *) luaL_checkudata(L, index, META_RECT);
  if (rect == NULL) luaL_typeerror(L, index, META_RECT);
  return rect;
}

Rectangle *push_rect(lua_State *L, Rectangle value) {
  Rectangle *rect = (Rectangle *) lua_newuserdata(L, sizeof(Rectangle));
  luaL_getmetatable(L, META_RECT);
  lua_setmetatable(L, -2);

  memcpy(rect, &value, sizeof(Rectangle));

  return rect;
}

int rect_new(lua_State *L) {
  // TODO: overload with rect.new(vec2, vec2)
  float x = luaL_optnumber(L, 1, 0);
  float y = luaL_optnumber(L, 2, 0);
  float w = luaL_optnumber(L, 3, 0);
  float h = luaL_optnumber(L, 4, 0);
  Rectangle value = {x, y, w, h};
  push_rect(L, value);
  return 1;
}

const luaL_Reg rect_functions[] = {{"new", rect_new}, {NULL, NULL}};

int rect_tostring(lua_State *L) {
  Rectangle *rect = check_rect(L, 1);
  const char *text =
      TextFormat("rect{x: %02.02f, y: %02.02f, w: %02.02f, h: %02.02f}", rect->x, rect->y, rect->width, rect->height);
  // TODO: do i need strlen?
  lua_pushlstring(L, text, strlen(text));
  return 1;
}

int rect_gc(lua_State *L) {
  Rectangle *rect = check_rect(L, 1);
  TraceLog(LOG_DEBUG, "gc %p rect(%f, %f, %f, %f)", rect, rect->x, rect->y, rect->width, rect->height);
  return 0;
}

const luaL_Reg rect_methods[] = {{0, 0}};

const luaL_Reg rect_meta[] = {{"__gc", rect_gc}, {"__tostring", rect_tostring}, {0, 0}};

void rect_register(lua_State *L) {
  lua_newtable(L);
  luaL_setfuncs(L, rect_functions, 0);

  luaL_newmetatable(L, META_RECT);
  luaL_setfuncs(L, rect_meta, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, rect_methods, 0);

  lua_pop(L, 1);

  lua_setglobal(L, "rect");
}

#endif
