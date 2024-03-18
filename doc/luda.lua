---@meta

-- How to write documentation https://luals.github.io/wiki/annotations/

-- version 0.1
---
---@class luda
---@field screen_width number
---@field screen_height number
luda = {}

---@class luda.texture
luda.texture = {}

---@class luda.vec2
luda.vec2 = {}

---@class luda.color
---@field RAYWHITE luda.color
---@field RED luda.color
luda.color = {}

---
---Callback function used to update the state of the game every frame.
---
---@alias luda.update fun(dt: number)

---
--- Callback function used to draw on the screen every frame.
---
---@alias luda.draw fun()

---
---@param color luda.color
---@overload fun()
function luda.clear(color) end

---
---Creates a 2D vector.
---
---@param x? number
---@param y? number
---@return luda.vec2
function luda.vec2.new(x, y) end

---
---Inplace vector sum
---
---@param rhs luda.vec2
---@return luda.vec2 this
function luda.vec2:add(rhs) end

---
---Inplace vector dot product
---
---@param rhs luda.vec2
---@return luda.vec2 this
function luda.vec2:mul(rhs) end

---
---Get vector components
---@return number x
---@return number y
function luda.vec2:get() end

---
---Creates a color.
---
---@param r? number
---@param g? number
---@param b? number
---@param a? number
---@return luda.color
function luda.color.new(r, g, b, a) end

---
---Loads 2D texture in memory.
---@param file_name string
---@return texture
function luda.texture.new(file_name) end

return luda
