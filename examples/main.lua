local texBunny = rl.load_texture("examples/resources/wabbit_alpha.png")

local flip_x = vec2.new(-1, 1)
local flip_y = vec2.new(1, -1)

Bunny = {}
Bunny.__index = Bunny
function Bunny:new(pos, spd, col)
   local bunny = {}
   setmetatable(bunny,Bunny)
   bunny.position = pos
   bunny.speed = spd
   bunny.color = col
   return bunny
end

function Bunny:update(texture)
  self.position:add(self.speed)
  local x,y = self.position:get()

  if ((x + texture.width/2) > rl.screen_width) or ((x + texture.width/2) < 0) then
    self.speed:mul(flip_x)
  end
  if ((y + texture.height/2) > rl.screen_height) or ((y + texture.height/2 - 40) < 0) then
    self.speed:mul(flip_y)
  end
end

local bunnies = {}
for i = 1, 10000 do
  local speed = vec2.new(math.random(-250, 250) / 60, math.random(-250, 250) / 60)
  local color = rl.color(math.random(50, 240), math.random(80, 240), math.random(100, 240))
  table.insert(bunnies, Bunny:new(vec2.new(400, 300), speed, color))
end

function rl.update(dt)
  for i = 1, #bunnies do
    bunnies[i]:update(texBunny)
  end
end

function rl.draw()
  rl.clear(rl.RAYWHITE)

  for i = 1, #bunnies do
    local b = bunnies[i]
    rl.texture(texBunny, b.position, b.color);
  end
end