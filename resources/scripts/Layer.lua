
function onMouseReleased(button)
	---print("[LUA] [MouseButtonReleasedEvent] button: " .. button)
end

function onMousePressed(button)
	---print("[LUA] [MouseButtonPressedEvent] button: " .. button)
end

function onKeyPressed(keyCode, repeatCount)
	---print("[LUA] [KeyPressedEvent] code: " .. keyCode .. " repeatCount: " .. repeatCount)
	---movePlayerPaddle(windowHeight, keyCode)
end

function onKeyReleased(keyCode)
	---print("[LUA] [KeyReleasedEvent] code: " .. keyCode)
end

function onWindowResize(width, height)
	---print("[LUA] [WindowResizeEvent] (" .. width .. "x" .. height .. ")")
end

function onMouseMove(x, y, offsetX, offsetY)
	---print("[LUA] [MouseMoveEvent] x=" .. x .. " y=" .. y .. ", offsetX=" .. offsetX .. " offsetY=" .. offsetY)
end

function onMouseScroll(offsetX, offsetY)
	---print("[LUA] [MouseScrollEvent] offsetX=" .. offsetX .. " offsetY=" .. offsetY)
end

function onAttach()
	print("[LUA] onAttach")
end

--[[
	Destruction code: c++ function onDetach -> lua function onDetach
--]]
function onDetach()
	print("[LUA] onDetach")
end

--[[
	Updating code: c++ function onUpdate -> lua function onUpdate
--]]
function onUpdate(deltaTime)
end

--[[
	Handle events code: c++ function onEvent
--]]
function onEvent(event)
end
