local flip_x = vec2.new(-1, 1)
local flip_y = vec2.new(1, -1)

Bunny = {}
Bunny.__index = Bunny
function Bunny:new(pos, spd, col)
  local bunny = {}
  setmetatable(bunny, Bunny)
  bunny.position = pos
  bunny.speed = spd
  bunny.color = col
  return bunny
end

function Bunny:update(texture)
  local w, h = texture:shape()
  self.position:add(self.speed)
  local x, y = self.position:get()

  if ((x + w / 2) > rl.screen_width) or ((x + w / 2) < 0) then
    self.speed:mul(flip_x)
  end
  if ((y + h / 2) > rl.screen_height) or ((y + h / 2 - 40) < 0) then
    self.speed:mul(flip_y)
  end
end

return Bunny
