local Bunny = require('examples.bunny')
local tex_bunny = luda.texture.new("examples/resources/wabbit_alpha.png")

local bunnies = {}
for i = 1, 10000 do
  local speed = luda.vec2.new(math.random(-250, 250) / 60, math.random(-250, 250) / 60)
  local color = luda.color.new(math.random(50, 240), math.random(80, 240), math.random(100, 240))
  table.insert(bunnies, Bunny:new(luda.vec2.new(400, 300), speed, color))
end

function luda.update(dt)
  for i = 1, #bunnies do
    bunnies[i]:update(tex_bunny)
  end
end

function luda.draw()
  luda.clear(luda.color.RAYWHITE)

  for i = 1, #bunnies do
    local b = bunnies[i]
    luda.draw_texture(tex_bunny, b.position, b.color);
  end
end
