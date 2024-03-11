#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <raylib.h>
#include <stdlib.h>

#include "color.h"
#include "rect.h"
#include "texture2d.h"
#include "vec.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define PACKAGE "luda"

void call_update(lua_State *L) {
  lua_getglobal(L, PACKAGE);
  lua_getfield(L, -1, "update");
  if (lua_isfunction(L, -1)) {
    lua_pushnumber(L, GetFrameTime());
    if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
      const char *msg = lua_tostring(L, -1);
      TraceLog(LOG_ERROR, "lua: %s", msg);
    }
  } else {
    TraceLog(LOG_ERROR, "rl.update is not a function");
  }
}

void call_draw(lua_State *L) {
  lua_getglobal(L, PACKAGE);
  lua_getfield(L, -1, "draw");
  if (lua_isfunction(L, -1)) {
    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
      const char *msg = lua_tostring(L, -1);
      TraceLog(LOG_ERROR, "lua: %s", msg);
    }
  } else {
    TraceLog(LOG_ERROR, "rl.draw is not a function");
  }
}

int lua_draw_texture(lua_State *L) {
  int n = lua_gettop(L);
  if (n == 3) {
    Texture *tex = to_texture(L, 1);
    Vector2 *pos = to_vec2(L, 2);
    Color *color = to_color(L, 3);
    DrawTexture(*tex, pos->x, pos->y, *color);
  } else {
    luaL_error(L, "rl.texture expects 1 arg, got: %d", n);
  }

  return 0;
}

int lua_draw_rectangle(lua_State *L) {
  int n = lua_gettop(L);
  if (n == 5) {
    int pos_x = lua_tonumber(L, 1);
    int pos_y = lua_tonumber(L, 2);
    int width = lua_tonumber(L, 3);
    int height = lua_tonumber(L, 4);
    Color *color = to_color(L, 5);

    DrawRectangle(pos_x, pos_y, width, height, *color);
  } else {
    luaL_error(L, "rl.rect expects 5 args, got: %d", n);
  }
  return 0;
}

int lua_clear(lua_State *L) {
  int n = lua_gettop(L);
  if (n == 0) {
    ClearBackground(BLACK);
  } else if (n == 1) {
    Color *color = to_color(L, 1);
    ClearBackground(*color);
  } else {
    luaL_error(L, "rl.clear expects 0 or 1 args, got: %d", n);
  }
  return 0;
}

void update(void *arg) {
  lua_State *L = (lua_State *) arg;

  lua_getglobal(L, PACKAGE);
  // TODO: is it better than passing a function to lua?
  lua_pushinteger(L, GetScreenWidth());
  lua_setfield(L, -2, "screen_width");
  lua_pushinteger(L, GetScreenHeight());
  lua_setfield(L, -2, "screen_height");

  call_update(L);
  BeginDrawing();
  call_draw(L);
  DrawFPS(5, 5);
  EndDrawing();
}

int main(void) {
  lua_State *L;

  L = luaL_newstate();
  luaL_openlibs(L);

  color_register(L);
  texture_register(L);
  vec2_register(L);
  rect_register(L);

  lua_newtable(L);

  lua_pushinteger(L, 0);
  lua_setfield(L, -2, "screen_width");
  lua_pushinteger(L, 0);
  lua_setfield(L, -2, "screen_height");

  lua_pushcfunction(L, lua_draw_rectangle);
  lua_setfield(L, -2, "rect");

  lua_pushcfunction(L, lua_draw_texture);
  lua_setfield(L, -2, "texture");

  lua_pushcfunction(L, lua_clear);
  lua_setfield(L, -2, "clear");

  lua_setglobal(L, PACKAGE);

  InitWindow(800, 600, "LOL");

  int err = luaL_dofile(L, "examples/main.lua");
  if (err != LUA_OK) {
    const char *msg = lua_tostring(L, -1);
    TraceLog(LOG_ERROR, "lua: %s", msg);
    return 0;
  }

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop_arg(update, L, 0, 1);
#else
  // SetTargetFPS(60);
  while (!WindowShouldClose()) {
    update(L);
  }
#endif

  CloseWindow();

  lua_close(L);

  return 0;
}
