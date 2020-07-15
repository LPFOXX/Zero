#include <Zero/Zero.h>
#include <Zero/Zero/Platform/RTTR/TestSprite.h>

namespace lp
{
	enum class Direction
	{
		Stopped,
		Up,
		Down,
		Left,
		Right
	};

	enum class Side
	{
		Left,
		Right
	};

	class Paddle : public zr::Sprite
	{
	public:
		Paddle(Side side) :
			zr::Sprite(),
			mDirection(Direction::Stopped),
			mPlayerSide(side),
			mScore(0),
			mSpeed(11.f)
		{
			resetPosition();
		}

		~Paddle()
		{

		}

		void setSpeed(float newSpeed)
		{
			if (newSpeed != mSpeed) {
				mSpeed = newSpeed;
			}
		}

		inline float getSpeed()
		{
			return mSpeed;
		}

		void incrementScore()
		{
			mScore++;
		}

		inline Direction getMovementDirection()
		{
			return mDirection;
		}

		void setMovementDirection(Direction direction)
		{
			mDirection = direction == Direction::Up ? Direction::Up : Direction::Down;
		}

		inline int getScore() const
		{
			return mScore;
		}

		virtual void onViewportUpdate(const glm::vec2& viewportSize) override
		{
			Sprite::onViewportUpdate(viewportSize);

			setPosition(mPlayerSide == Side::Left ? 150.f : zr::ViewportDependable::GetViewportSize().x - 150.f - mSize.x, mPosition.y);
		}

		void resetPosition()
		{
			const zr::Window& appWindow = zr::Application::GetWindow();
			setSize(15.f, 65.f);
			setPosition(mPlayerSide == Side::Left ? 150.f : appWindow.getWidth() - 150.f - mSize.x, appWindow.getHeight() / 2.f - mSize.y / 2.f);

			mScore = 0;
		}

	private:
		Direction mDirection;
		Side mPlayerSide;
		int mScore;
		float mSpeed;
	};

	class Ball : public zr::Sprite
	{
	public:
		Ball(float newSpeed) :
			zr::Sprite(),
			mDirectionX(Direction::Stopped),
			mDirectionY(Direction::Down),
			mSpeed(newSpeed),
			mTarget(nullptr)
		{
			mSize = glm::vec2(15.f, 15.f);
			mPosition = glm::vec2(1280.f / 2.f - mSize.x / 2.f, 600.f / 2.f - mSize.y / 2.f);
		}

		~Ball()
		{

		}

		void setDirectionX(Direction direction)
		{
			mDirectionX = direction;
		}

		void setDirectionY(Direction direction)
		{
			mDirectionY = direction;
		}

		inline Direction getMovementDirectionX()
		{
			return mDirectionX;
		}

		inline Direction getMovementDirectionY()
		{
			return mDirectionY;
		}

		void setTarget(const std::shared_ptr<Paddle>& newTarget)
		{
			mTarget = newTarget;
		}

		inline float getSpeed()
		{
			return mSpeed;
		}

		void move(const std::shared_ptr<Paddle>& player, const std::shared_ptr<Paddle> aiPlayer)
		{
			if (mTarget == player) {
				mDirectionX = Direction::Left;
			}
			else {
				mDirectionX = Direction::Right;
			}

			if (mDirectionX == Direction::Left) {
				zr::Movable2D::move(-mSpeed, 0.f);
			}
			else if (mDirectionX == Direction::Right) {
				zr::Movable2D::move(mSpeed, 0.f);
			}

			if (mDirectionY == Direction::Up) {
				zr::Movable2D::move(0.f, -mSpeed);
			}
			else if (mDirectionY == Direction::Down) {
				zr::Movable2D::move(0.f, mSpeed);
			}
		}

		void reset(float newBallSpeed)
		{
			mDirectionY = Direction::Up;
			setPosition(zr::ViewportDependable::GetViewportSize().x / 2.f, zr::ViewportDependable::GetViewportSize().y / 2.f);
			mSpeed = newBallSpeed;
			mTarget = nullptr;
		}

	private:
		Direction mDirectionX;
		Direction mDirectionY;
		float mSpeed;
		std::shared_ptr<Paddle> mTarget;
	};

	class LuaLayer
	{
	protected:
		zr::LuaVM::LuaFunction luaOnAttach;
		zr::LuaVM::LuaFunction onDetach;
		zr::LuaVM::LuaFunction onUpdate;
		zr::LuaVM::LuaFunction onEvent;
	};

	class ZeroLuaLayer : public zr::Layer, public LuaLayer
	{
	public:
		ZeroLuaLayer();
		virtual ~ZeroLuaLayer();

		// Inherited via Layer
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(const zr::Time& elapsedTime) override;
		virtual void onEvent(zr::Event& e) override;
		virtual void onImGuiRender() override;

		int testFunction(unsigned a, const char* b, double c);

	private:
		void drawEditorWindow();

	private:
		zr::LuaVM mLuaVM;
	};
}
