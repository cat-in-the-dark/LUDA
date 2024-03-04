#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <raylib.h>
#include <stdlib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define PACKAGE "rl"

void call_update(lua_State *L) {
  lua_getglobal(L, PACKAGE);
  lua_getfield(L, -1, "update");
  if (lua_isfunction(L, -1)) {
    lua_pushnumber(L, GetFrameTime());
    if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
      const char* msg = lua_tostring(L, -1);
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
    if (lua_pcall(L, 0,0,0) != LUA_OK) {
      const char* msg = lua_tostring(L, -1);
      TraceLog(LOG_ERROR, "lua: %s", msg);
    }
  } else {
    TraceLog(LOG_ERROR, "rl.draw is not a function");
  }
}

void lua_pushcolor(lua_State *L, Color color) {
  // TODO: make it userdata to speed up
  lua_newtable(L);
  lua_pushnumber(L, color.r);
  lua_setfield(L, -2, "r");
  lua_pushnumber(L, color.g);
  lua_setfield(L, -2, "g");
  lua_pushnumber(L, color.b);
  lua_setfield(L, -2, "b");
  lua_pushnumber(L, color.a);
  lua_setfield(L, -2, "a");
}

void lua_pushvector2(lua_State *L, Vector2 vec) {
  // TODO: make it userdata to speed up
  lua_newtable(L);
  lua_pushnumber(L, vec.x);
  lua_setfield(L, -2, "x");
  lua_pushnumber(L, vec.y);
  lua_setfield(L, -2, "y");
}

Color lua_tocolor(lua_State *L, int idx) {
  lua_getfield(L, idx, "r");
  int red = lua_tonumber(L, -1);
  lua_getfield(L, idx, "g");
  int green = lua_tonumber(L, -1);
  lua_getfield(L, idx, "b");
  int blue = lua_tonumber(L, -1);
  lua_getfield(L, idx, "a");
  int alpha = lua_tonumber(L, -1);

  return (Color){ red, green, blue, alpha };
}

int lua_new_color(lua_State *L) {
  int n = lua_gettop(L);
  if (n == 3) {
    int red = lua_tonumber(L, 1);
    int green = lua_tonumber(L, 2);
    int blue = lua_tonumber(L, 3);
    lua_pushcolor(L, (Color){red, green, blue, 255});
  } else {
    lua_pushcolor(L, BLACK);
  }

  return 1;
}

int lua_new_vector2(lua_State *L) {
  int n = lua_gettop(L);
  if (n == 2) {
    double x = lua_tonumber(L, 1);
    double y = lua_tonumber(L, 2);
    lua_pushvector2(L, (Vector2){x, y});
  } else {
    lua_pushvector2(L, (Vector2){0, 0});
  }

  return 1;
}

void lua_pushtexture(lua_State *L, Texture2D tex) {
  lua_newtable(L);
  lua_pushinteger(L, tex.id);
  lua_setfield(L, -2, "id");
  lua_pushinteger(L, tex.width);
  lua_setfield(L, -2, "width");
  lua_pushinteger(L, tex.height);
  lua_setfield(L, -2, "height");
  lua_pushinteger(L, tex.mipmaps);
  lua_setfield(L, -2, "mipmaps");
  lua_pushinteger(L, tex.format);
  lua_setfield(L, -2, "format");
}

Texture lua_totexture2d(lua_State *L, int idx) {
  lua_getfield(L, idx, "id");
  unsigned int id = lua_tointeger(L, -1);  
  lua_getfield(L, idx, "width");
  int width = lua_tointeger(L, -1);
  lua_getfield(L, idx, "height");
  int height = lua_tointeger(L, -1);
  lua_getfield(L, idx, "mipmaps");
  int mipmaps = lua_tointeger(L, -1);
  lua_getfield(L, idx, "format");
  int format = lua_tointeger(L, -1);

  return (Texture){ id, width, height, mipmaps, format };
}

int lua_load_texture(lua_State *L) {
  int n = lua_gettop(L);
  if (n == 1) {
    const char* fileName = lua_tostring(L, 1);
    Texture tex = LoadTexture(fileName);
    lua_pushtexture(L, tex);
    return 1;
  } else {
    luaL_error(L, "rl.load_texture expects 1 arg, got: %d", n);
  }
  return 0;
}

int lua_draw_texture(lua_State *L) {
  int n = lua_gettop(L);
  if (n == 4) {
    Texture tex = lua_totexture2d(L, 1);
    int pos_x = lua_tonumber(L, 2);
    int pos_y = lua_tonumber(L, 3);
    Color color = lua_tocolor(L, 4);
    DrawTexture(tex, pos_x, pos_y, color);
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
    Color color = lua_tocolor(L, 5);
    
    DrawRectangle(pos_x, pos_y, width, height, color);
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
    Color color = lua_tocolor(L, 1);
    ClearBackground(color);
  } else {
    luaL_error(L, "rl.clear expects 0 or 1 args, got: %d", n);
  }
  return 0;
}

void update(void *arg) {
  lua_State* L = (lua_State*)arg;

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
  lua_State* L;

  L = luaL_newstate();
  luaL_openlibs(L);

  lua_newtable(L);

  lua_pushinteger(L, 0);
  lua_setfield(L, -2, "screen_width");
  lua_pushinteger(L, 0);
  lua_setfield(L, -2, "screen_height");

  lua_pushcfunction(L, lua_new_color);
  lua_setfield(L, -2, "color");

  lua_pushcfunction(L, lua_new_vector2);
  lua_setfield(L, -2, "vec2");

  lua_pushcolor(L, RED);
  lua_setfield(L, -2, "RED");

  lua_pushcolor(L, RAYWHITE);
  lua_setfield(L, -2, "RAYWHITE");

  lua_pushcfunction(L, lua_draw_rectangle);
  lua_setfield(L, -2, "rect");

  lua_pushcfunction(L, lua_load_texture);
  lua_setfield(L, -2, "load_texture");

  lua_pushcfunction(L, lua_draw_texture);
  lua_setfield(L, -2, "texture");

  lua_pushcfunction(L, lua_clear);
  lua_setfield(L, -2, "clear");

  lua_setglobal(L, PACKAGE);

  InitWindow(800, 600, "LOL");

  int err = luaL_dofile(L, "examples/main.lua");
  if (err != LUA_OK) {
    const char* msg = lua_tostring(L, -1);
    TraceLog(LOG_ERROR, "lua: %s", msg);
    return 0;
  }
  
#if defined(PLATFORM_WEB)
  emscripten_set_main_loop_arg(update, L, 0, 1);
#else
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    update(L);
  }
#endif

  CloseWindow();

  lua_close(L);
  
  return 0;
}
