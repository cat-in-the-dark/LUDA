local texBunny = rl.load_texture("examples/resources/wabbit_alpha.png")

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
  self.position.x = self.position.x + self.speed.x
  self.position.y = self.position.y + self.speed.y
  if ((self.position.x + texture.width/2) > rl.screen_width) or ((self.position.x + texture.width/2) < 0) then
    self.speed.x = self.speed.x * -1
  end
  if ((self.position.y + texture.height/2) > rl.screen_height) or ((self.position.y + texture.height/2 - 40) < 0) then
    self.speed.y = self.speed.y * -1
  end
end

local bunnies = {}
for i = 1, 10000 do
  local speed = rl.vec2(math.random(-250, 250) / 60, math.random(-250, 250) / 60)
  local color = rl.color(math.random(50, 240), math.random(80, 240), math.random(100, 240))
  table.insert(bunnies, Bunny:new(rl.vec2(400, 300), speed, color))
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
    rl.texture(texBunny, b.position.x, b.position.y, b.color);
  end
end