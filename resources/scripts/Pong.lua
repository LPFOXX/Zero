
POSITION_Left = 0
POSITION_Right = 1

DIRECTION_Stopped = 0
DIRECTION_Up = 1
DIRECTION_Down = 2
DIRECTION_Left = 3
DIRECTION_Right = 4

windowWidth = 0
windowHeight = 0

function createPaddle(side)
	return {
		speed = 6.5,
		position = {x=0.0, y=0.0},
		size = {x=15.0, y=65.0},
		score = 0,
		direction = DIRECTION_Stopped,
		side = side
	}
end

function createBall(newSpeed)
	return {
		position = {x = 0.0, y = 0.0},
		size = {x = 15, y = 15},
		speed = newSpeed,
		directionX = DIRECTION_Stopped,
		directionY = DIRECTION_Down,
		target = 0
	}
end

function resetBall(ball, whoScored, whoLost)
	whoLost.score = whoLost.score + 1
	newBallSpeed = ball.speed + 0.2
	mBall = createBall(newBallSpeed)
	mBall.target = whoLost.side
end

function movePlayerPaddle(windowHeight, keyCode)
	if keyCode == KEYBOARD_S or keyCode == KEYBOARD_Down then
		mPlayer.position.y = mPlayer.position.y + mPlayer.speed
	elseif keyCode == KEYBOARD_W or keyCode == KEYBOARD_Up then
		mPlayer.position.y = mPlayer.position.y - mPlayer.speed
	end

	if mPlayer.position.y < 0 then
		mPlayer.position.y = 0
	elseif mPlayer.position.y >= windowHeight - player.size.y then
		mPlayer.position.y = windowHeight - mPlayer.size.y
	end
end

function moveBall(player, aiPlayer)
	if mBall.target == player.side then
		mBall.directionX = DIRECTION_Left
	else
		mBall.directionX = DIRECTION_Right
	end

	if mBall.directionX == DIRECTION_Left then
		mBall.position.x = mBall.position.x - mBall.speed
	elseif mBall.directionX == DIRECTION_Right then
		mBall.position.x = mBall.position.x + mBall.speed
	end

	if mBall.directionY == DIRECTION_Up then
		mBall.position.y = mBall.position.y - mBall.speed
	elseif mBall.directionY == DIRECTION_Down then
		mBall.position.y = mBall.position.y + mBall.speed
	end
end


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
	windowWidth = width
	windowHeight = height
end

function onMouseMove(x, y, offsetX, offsetY)
	---print("[LUA] [MouseMoveEvent] x=" .. x .. " y=" .. y .. ", offsetX=" .. offsetX .. " offsetY=" .. offsetY)
end

function onMouseScroll(offsetX, offsetY)
	---print("[LUA] [MouseScrollEvent] offsetX=" .. offsetX .. " offsetY=" .. offsetY)
end

HelloWorld()
HelloWorld1(30)
local r = HelloWorld2(30, 4.5)
print("Result = " .. r)
StringTest("Passing a String")
DefaultValueTest(55, 42, 30, 25.0)
DefaultValueTest(55, 42, 30)

function printSprite(sprite)
	sprite:move(5, 5)
	local positionX, positionY = sprite:getPosition()
	print("Position X: " .. positionX .. ", Y: " .. positionY)
	local width, height = sprite:getSize()
	print("Width: " .. width .. ", Height: " .. height)
end
 
---[[
local sprite = Sprite()
sprite:setSize(25, 25)
sprite:setPosition(30, 4.5)
printSprite(sprite)

sprite:move(10, 10)
printSprite(sprite)

print("Sprite.PublicX = " .. sprite.PublicX)
sprite.PublicX = 600
print("Sprite.PublicX = " .. sprite.PublicX)

z = sprite.z
--[[
if z == nil then
	print("z is nil")
else
	print("z = " .. z)
end
--]]

sprite.z = 40
z = sprite.z

function intercepts(sprite1, sprite2)
	print("Intercepts:")
end

sprite.intercepts = function (self, other)
	print("Calling Sprite.intercepts:")
	printSprite(self)
	printSprite(other)
	return false
end

sprite.table = {
	name = "Lidneu Pires",
	"LP",
	"email@domain.com",
	25,
	42.3,
	"Melodia",
	"Outra"
--[[
	name1: "LP",
	email: "email@domain.com",
	other: 25,
	[4]: 42.3
--]]
}

print("Name: " .. sprite.table.name)
--[[
print("Email: " .. sprite.table.email)
print("Other: " .. sprite.table["other"])
print("4: " .. sprite.table[4])
print("Size: " .. #sprite.table)
--]]
--print("5: " .. sprite.table[6])
--print("clash: " .. sprite.table["clash"])

for i = 1, #sprite.table do
	print("sprite.table["..i.."] = " .. sprite.table[i])
end

sprite2 = Sprite()

if sprite:intercepts(sprite2) then
	print("The two sprites intercept.")
else
	print("The two sprites do *not* intercept.")
end

mPlayer = createPaddle(POSITION_Left)
mAIPlayer = createPaddle(POSITION_Right)
mBall = createBall(7.0)
mBall.target = mPlayer.side

isGameOver = false
--[[
player = Paddle.new(POSITION_Left)
aiPlayer = Paddle.new(POSITION_Right)

player.x = player.y
print("player.x " .. player.x)
print("player.y " .. player.y)

Paddle.setSpeed(player, 50)
Paddle.incrementScore(player)
Paddle.getSpeed(player);
Paddle.getMovementDirection(player)
Paddle.resetPosition(player)

aiPlayer:setSpeed(6.5)
aiPlayer:incrementScore()
print("aiPlayer speed: " .. aiPlayer:getSpeed())
print("aiPlayer getMovementDirection: " .. aiPlayer:getMovementDirection())
aiPlayer:resetPosition()
--]]

--[[
	Initializing code: c++ function onAttach -> lua function init
--]]
function onAttach()
	print("[LUA] onAttach")

	local sprt = Sprite()

---[[
	SpriteTable = {}

	for i = 1, 5 do
		SpriteTable[i] = Sprite()
	end
--]]
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

	if Input.IsKeyPressed(Keyboard.Up) then
		sprite:move(0, deltaTime)
	end

	if Input.IsKeyPressed(Keyboard.Down) then
		sprite:move(0, -deltaTime)
	end

	if Input.IsKeyPressed(Keyboard.Left) then
		sprite:move(-deltaTime, 0)
	end
	
	if Input.IsKeyPressed(Keyboard.Right) then
		sprite:move(deltaTime, 0)
	end

	if Input.IsKeyPressed(Keyboard.D) then
		sprite:draw()

		for i = 1, #SpriteTable do
			SpriteTable[i]:draw()
		end
	end

	end

	windowQuarter = windowWidth * .25

	if mPlayer.score == 10 or mAIPlayer.score == 10 then
		isGameOver = true

		if(mPlayer.score == 10) then
			-- print("[LUA] You Won. Press space")
		elseif (mAIPlayer.score == 10) then
			-- print("[LUA] AI Won. Press space")
		end
	end

	if not isGameOver then
		if mBall.position.x <= 0 then
			resetBall(mBall, mAIPlayer, mPlayer)
		end

		if mBall.position.x >= windowWidth - mBall.size.x then
			resetBall(mBall, mPlayer, mAIPlayer)
		end

		if mBall.position.y <= 0 then
			mBall.directionY = DIRECTION_Down
		end

		if mBall.position.y >= windowHeight - mBall.size.y then
			mBall.directionY = DIRECTION_Up
		end

		moveBall(mPlayer, mAIPlayer)
--[[
	//			if (mAIPlayer->getPosition().y > mBall->getPosition().y - (mAIPlayer->getSize().y / 2.f)) {
	//				if (mBall->getMovementDirectionX() == Direction::Right) {
	//					mAIPlayer->move(0.f, -mAIPlayer->getSpeed());
	//				}
	//			}

	//			if (mAIPlayer->getPosition().y < mBall->getPosition().y - (mAIPlayer->getSize().y) / 2.f) {
	//				if (mBall->getMovementDirectionX() == Direction::Right) {
	//					mAIPlayer->move(0.f, mAIPlayer->getSpeed());
	//				}
	//			}

	//			if (mAIPlayer->getPosition().y < 0) {
	//				mAIPlayer->setPosition(mAIPlayer->getPosition().x, 0.f);
	//			}
	//			else if (mAIPlayer->getPosition().y >= windowHeight - mAIPlayer->getSize().y) {
	//				mAIPlayer->setPosition(mAIPlayer->getPosition().x, windowHeight - mAIPlayer->getSize().y);
	//			}

	//			if (mBall->getPosition().x - mBall->getSize().x <= mPlayer->getPosition().x && mBall->getPosition().x >= mPlayer->getPosition().x - mPlayer->getSize().x) {
	//				if (mBall->getPosition().y <= mPlayer->getPosition().y + mPlayer->getSize().y && mBall->getPosition().y + mBall->getSize().y >= mPlayer->getPosition().y) {
	//					mBall->setDirectionX(Direction::Right);
	//					mBall->setTarget(mAIPlayer);
	//					// play sound
	//				}
	//			}

	//			if (mBall->getPosition().x - mBall->getSize().x <= mAIPlayer->getPosition().x && mBall->getPosition().x >= mAIPlayer->getPosition().x - mAIPlayer->getSize().x) {
	//				if (mBall->getPosition().y <= mAIPlayer->getPosition().y + mAIPlayer->getSize().y && mBall->getPosition().y + mBall->getSize().y >= mAIPlayer->getPosition().y) {
	//					mBall->setDirectionX(Direction::Left);
	//					mBall->setTarget(mPlayer);
	//					// play sound
	//				}
	//			}
	//		}
	//		else {
	//			mGameOverText->setPosition(2.f * windowQuarter - mGameOverText->getSize().x * .5f, windowHeight * .5f - mGameOverText->getSize().y * .5f);
	//		}
--]]	
end

--[[
	Handle events code: c++ function onEvent
--]]
function onEvent(event)
	if event.type == EventType.KeyPressed then
		onKeyPressed(event.keyCode, event.repeatCount)
	elseif event.type == EventType.KeyReleased then
		onKeyReleased(event.keyCode)
	elseif event.type == EventType.MouseButtonPressed then
		onMousePressed(event.button)
	elseif event.type == EventType.MouseButtonReleased then
		onMouseReleased(event.button)
	elseif event.type == EventType.WindowResize then
		onWindowResize(event.width, event.height)
	elseif event.type == EventType.MouseMove then
		onMouseMove(event.x, event.y, event.offsetX, event.offsetY)
	elseif event.type == EventType.MouseScroll then
		onMouseScroll(event.offsetX, event.offsetY)
	end
end

--[[
function testFunction(a, b, c)
	print("[LUA] a: " .. a .. " b: " .. b .. " c: " .. c)
end
]]

function testFunction(a, b, c)
	print("[LUA] testFunction")
	return a
end
