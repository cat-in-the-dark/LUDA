#ifndef luda_vec_h
#define luda_vec_h

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <string.h>

#define META_VEC2 "vec2"

Vector2 *to_vec2(lua_State *L, int index) {
  Vector2 *vec = (Vector2 *) lua_touserdata(L, index);
  if (vec == NULL) luaL_typeerror(L, index, META_VEC2);
  return vec;
}

Vector2 *check_vec2(lua_State *L, int index) {
  Vector2 *vec;
  luaL_checktype(L, index, LUA_TUSERDATA);
  vec = (Vector2 *) luaL_checkudata(L, index, META_VEC2);
  if (vec == NULL) luaL_typeerror(L, index, META_VEC2);
  return vec;
}

Vector2 *push_vec2(lua_State *L, Vector2 value) {
  Vector2 *vec = (Vector2 *) lua_newuserdata(L, sizeof(Vector2));
  luaL_getmetatable(L, META_VEC2);
  lua_setmetatable(L, -2);

  memcpy(vec, &value, sizeof(Vector2));

  return vec;
}

void push_named_vec(lua_State *L, Vector2 value, const char *name) {
  push_vec2(L, value);
  lua_setfield(L, -2, name);
}

int vec2_new(lua_State *L) {
  double x = luaL_optnumber(L, 1, 0);
  double y = luaL_optnumber(L, 2, 0);
  Vector2 value = {x, y};
  push_vec2(L, value);
  return 1;
}

int vec2_add(lua_State *L) {
  Vector2 *lhs = check_vec2(L, 1);
  Vector2 *rhs = check_vec2(L, 2);
  push_vec2(L, Vector2Add(*lhs, *rhs));
  return 1;
}

int vec2_add_inplace(lua_State *L) {
  Vector2 *lhs = check_vec2(L, 1);
  Vector2 *rhs = check_vec2(L, 2);
  lhs->x += rhs->x;
  lhs->y += rhs->y;
  return 0;
}

int vec2_mul(lua_State *L) {
  Vector2 *lhs = check_vec2(L, 1);
  Vector2 *rhs = check_vec2(L, 2);
  push_vec2(L, Vector2Multiply(*lhs, *rhs));
  return 1;
}

int vec2_mul_inplace(lua_State *L) {
  Vector2 *lhs = check_vec2(L, 1);
  Vector2 *rhs = check_vec2(L, 2);
  lhs->x *= rhs->x;
  lhs->y *= rhs->y;
  return 0;
}

int vec2_index(lua_State *L) {
  Vector2 *vec = check_vec2(L, 1);
  const char *name = luaL_checkstring(L, 2);

  if (strcmp("x", name) == 0) {
    lua_pushnumber(L, vec->x);
    return 1;
  }

  if (strcmp("y", name) == 0) {
    lua_pushnumber(L, vec->y);
    return 1;
  }

  luaL_error(L, "'vec2' object has no field '%s'", name);
  return 0;
}

int vec2_get(lua_State *L) {
  Vector2 *vec = check_vec2(L, 1);
  lua_pushnumber(L, vec->x);
  lua_pushnumber(L, vec->y);
  return 2;
}

const luaL_Reg vec2_functions[] = {{"new", vec2_new}, {NULL, NULL}};

int vec2_tostring(lua_State *L) {
  Vector2 *vec = check_vec2(L, 1);
  const char *text = TextFormat("vec2{x: %02.02f, y: %02.02f}", vec->x, vec->y);
  // TODO: do i need strlen?
  lua_pushlstring(L, text, strlen(text));
  return 1;
}

int vec2_gc(lua_State *L) {
  Vector2 *vec = check_vec2(L, 1);
  TraceLog(LOG_DEBUG, "gc %p vec2(%f, %f)", vec, vec->x, vec->y);
  return 0;
}

const luaL_Reg vec2_methods[] = {{"add", vec2_add_inplace}, {"mul", vec2_mul_inplace}, {"get", vec2_get}, {0, 0}};

const luaL_Reg vec2_meta[] = {{"__gc", vec2_gc},   {"__tostring", vec2_tostring}, {"__add", vec2_add},
                              {"__mul", vec2_mul}, {"__index", vec2_index},       {0, 0}};

void vec2_register(lua_State *L) {
  lua_newtable(L);
  luaL_setfuncs(L, vec2_functions, 0);

  luaL_newmetatable(L, META_VEC2);
  luaL_setfuncs(L, vec2_meta, 0);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, vec2_methods, 0);

  lua_pop(L, 1);

  Vector2 up = {0, 1};
  Vector2 down = {0, -1};
  Vector2 left = {-1, 0};
  Vector2 right = {1, 0};
  Vector2 one = {1, 1};
  Vector2 zero = {0, 0};

  push_named_vec(L, up, "UP");
  push_named_vec(L, down, "DOWN");
  push_named_vec(L, right, "RIGHT");
  push_named_vec(L, left, "LEFT");
  push_named_vec(L, zero, "ZERO");
  push_named_vec(L, one, "ONE");

  lua_setfield(L, -2, "vec2");
}

#endif
