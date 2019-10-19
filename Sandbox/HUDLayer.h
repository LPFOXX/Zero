#pragma once

#include <Zero/Zero.h>

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
				zr::Movable::move(-mSpeed, 0.f);
			}
			else if (mDirectionX == Direction::Right) {
				zr::Movable::move(mSpeed, 0.f);
			}

			if (mDirectionY == Direction::Up) {
				zr::Movable::move(0.f, -mSpeed);
			}
			else if (mDirectionY == Direction::Down) {
				zr::Movable::move(0.f, mSpeed);
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

	class PongGame : public zr::ViewportDependable, public zr::ViewportResizeListener
	{
	public:
		PongGame()
		{
			init();
		}

		~PongGame()
		{

		}

		void init()
		{
			mPlayer.reset(new Paddle(Side::Left));
			mAIPlayer.reset(new Paddle(Side::Right));

			mAIPlayer->setSpeed(6.5f);

			mBall.reset(new Ball(7.f));
			mBall->setTarget(mPlayer);

			mFont.reset(new zr::Font);
			if (!mFont->loadFromFile("resources/fonts/semplicita_medium.otf")) {
				ZR_ERROR("[PongGame] Can't load game font.");
			}

			mPlayerScoreText.reset(new zr::Text);
			mPlayerScoreText->setFont(mFont);
			mPlayerScoreText->setFontSize(48U);
			mPlayerScoreText->setFillColor({ 1.f, 1.f, 1.f, 1.f });
			mPlayerScoreText->setOutlineThickness(2U);
			mPlayerScoreText->setOutlineColor({ 0.f, 0.f, 0.f, 1.f });

			mAIPlayerScoreText.reset(new zr::Text);
			mAIPlayerScoreText->setFont(mFont);
			mAIPlayerScoreText->setFontSize(48U);
			mAIPlayerScoreText->setFillColor({ 1.f, 1.f, 1.f, 1.f });
			mAIPlayerScoreText->setOutlineThickness(2U);
			mAIPlayerScoreText->setOutlineColor({ 0.f, 0.f, 0.f, 1.f });

			mGameOverText.reset(new zr::Text);
			mGameOverText->setFont(mFont);
			mGameOverText->setFontSize(48U);
			mGameOverText->setFillColor({ 1.f, 1.f, 1.f, 1.f });
			mGameOverText->setOutlineThickness(2U);
			mGameOverText->setOutlineColor({ 0.f, 0.f, 0.f, 1.f });

			subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mPlayer));
			subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mAIPlayer));
			subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mBall));
			subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mPlayerScoreText));
			subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mAIPlayerScoreText));
			subscribe(static_cast<std::shared_ptr<zr::Observer<glm::vec2>>>(mGameOverText));
		}

		void draw(const glm::mat4& viewprojectionMatrix)
		{
			mPlayerScoreText->render(viewprojectionMatrix);
			mAIPlayerScoreText->render(viewprojectionMatrix);

			mPlayer->render(viewprojectionMatrix);
			mAIPlayer->render(viewprojectionMatrix);

			mBall->render(viewprojectionMatrix);

			if (mIsGameOver) {
				mGameOverText->render(viewprojectionMatrix);
			}
		}

		void update()
		{
			float windowWidth = zr::Application::GetWindow().getWidth();
			float windowHeight = zr::Application::GetWindow().getHeight();
			const glm::vec2& ballSize = mBall->getSize();
			const glm::vec2& ballPosition = mBall->getPosition();

			mPlayerScoreText->setString(std::to_string(mPlayer->getScore()));
			mAIPlayerScoreText->setString(std::to_string(mAIPlayer->getScore()));

			float windowQuarter = windowWidth * .25f;
			mPlayerScoreText->setPosition(windowQuarter - mPlayerScoreText->getSize().x * .5f, 4);
			mAIPlayerScoreText->setPosition(3.f * windowQuarter - mAIPlayerScoreText->getSize().x * .5f, 4);

			if (mPlayer->getScore() == 10 || mAIPlayer->getScore() == 10) {
				mIsGameOver = true;

				if (mPlayer->getScore() == 10) {
					mGameOverText->setString("You won! Congrats!\nPress spacebar to restart.");
				}
				else if (mAIPlayer->getScore() == 10) {
					mGameOverText->setString("You let the AI win! Best luck next time!\nPress spacebar to restart.");
				}
			}

			if (!mIsGameOver) {
 				if (mBall->getPosition().x <= 0.f) {
					resetBall(mAIPlayer, mPlayer);
				}

				if (mBall->getPosition().x >= windowWidth - ballSize.x) {
					resetBall(mPlayer, mAIPlayer);
				}

				if (mBall->getPosition().y <= 0.f) {
					mBall->setDirectionY(Direction::Down);
				}

				if (mBall->getPosition().y >= windowHeight - ballSize.y) {
					mBall->setDirectionY(Direction::Up);
				}

				movePlayerPaddle(windowHeight);
				mBall->move(mPlayer, mAIPlayer);

				if (mAIPlayer->getPosition().y > mBall->getPosition().y - (mAIPlayer->getSize().y / 2.f)) {
					if (mBall->getMovementDirectionX() == Direction::Right) {
						mAIPlayer->move(0.f, -mAIPlayer->getSpeed());
					}
				}

				if (mAIPlayer->getPosition().y < mBall->getPosition().y - (mAIPlayer->getSize().y) / 2.f) {
					if (mBall->getMovementDirectionX() == Direction::Right) {
						mAIPlayer->move(0.f, mAIPlayer->getSpeed());
					}
				}

				if (mAIPlayer->getPosition().y < 0) {
					mAIPlayer->setPosition(mAIPlayer->getPosition().x, 0.f);
				}
				else if (mAIPlayer->getPosition().y >= windowHeight - mAIPlayer->getSize().y) {
					mAIPlayer->setPosition(mAIPlayer->getPosition().x, windowHeight - mAIPlayer->getSize().y);
				}

				if (mBall->getPosition().x - mBall->getSize().x <= mPlayer->getPosition().x && mBall->getPosition().x >= mPlayer->getPosition().x - mPlayer->getSize().x) {
					if (mBall->getPosition().y <= mPlayer->getPosition().y + mPlayer->getSize().y && mBall->getPosition().y + mBall->getSize().y >= mPlayer->getPosition().y) {
						mBall->setDirectionX(Direction::Right);
						mBall->setTarget(mAIPlayer);
						// play sound
					}
				}

				if (mBall->getPosition().x - mBall->getSize().x <= mAIPlayer->getPosition().x && mBall->getPosition().x >= mAIPlayer->getPosition().x - mAIPlayer->getSize().x) {
					if (mBall->getPosition().y <= mAIPlayer->getPosition().y + mAIPlayer->getSize().y && mBall->getPosition().y + mBall->getSize().y >= mAIPlayer->getPosition().y) {
						mBall->setDirectionX(Direction::Left);
						mBall->setTarget(mPlayer);
						// play sound
					}
				}
			}
			else {
				mGameOverText->setPosition(2.f * windowQuarter - mGameOverText->getSize().x * .5f, windowHeight * .5f - mGameOverText->getSize().y * .5f);
			}
		}

		void movePlayerPaddle(float windowHeight)
		{
			if (zr::Input::isKeyPressed(zr::Keyboard::S) || zr::Input::isKeyPressed(zr::Keyboard::Down)) {
				mPlayer->move(0.f, mPlayer->getSpeed());
			}
			else if (zr::Input::isKeyPressed(zr::Keyboard::W) || zr::Input::isKeyPressed(zr::Keyboard::Up)) {
				mPlayer->move(0.f, -mPlayer->getSpeed());
			}

			if (mPlayer->getPosition().y < 0) {
				mPlayer->setPosition(mPlayer->getPosition().x, 0.f);
			}
			else if (mPlayer->getPosition().y >= windowHeight - mPlayer->getSize().y) {
				mPlayer->setPosition(mPlayer->getPosition().x, windowHeight - mPlayer->getSize().y);
			}
		}

		void reset()
		{
			if (mIsGameOver) {
				mPlayer->resetPosition();
				mAIPlayer->resetPosition();

				mAIPlayer->setSpeed(6.5f);

				mBall->reset(7.f);
				mBall->setTarget(mPlayer);

				mPlayerScoreText->setString(std::to_string(0));
				mAIPlayerScoreText->setString(std::to_string(0));

				mIsGameOver = false;
			}
		}

		void resetBall(std::shared_ptr<Paddle>& whoScored, std::shared_ptr<Paddle> whoLost)
		{
			whoScored->incrementScore();
			float newBallSpeed = mBall->getSpeed() + .2f;
			mBall->reset(newBallSpeed);
			mBall->setTarget(whoLost);
			mDelayAmount = zr::Application::GetTime().asSeconds();
		}

		void addDelay()
		{

		}

		// Inherited via ViewportResizeListener
		virtual void onViewportUpdate(const glm::vec2& viewportSize) override
		{
			ZR_INFO("[PongGame] Viewport Resize notification. New size {0} x {1}", viewportSize.x, viewportSize.y);
			notify(viewportSize);
		}

	private:
		std::shared_ptr<Paddle> mPlayer;
		std::shared_ptr<Paddle> mAIPlayer;
		std::shared_ptr<Ball> mBall;

		std::shared_ptr<zr::Font> mFont;
		std::shared_ptr<zr::Text> mPlayerScoreText;
		std::shared_ptr<zr::Text> mAIPlayerScoreText;
		std::shared_ptr<zr::Text> mGameOverText;


		float mDelayAmount = zr::Application::GetTime().asSeconds();
		bool mIsGameOver = false;
	};



	class HUDLayer : public zr::Layer, public zr::ViewportDependable, public zr::ViewportResizeListener
	{
	public:
		HUDLayer();
		virtual ~HUDLayer();

		// Inherited via Layer
		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(const zr::Time& elapsedTime) override;
		virtual void OnImGuiRender() override;
		virtual void onEvent(zr::Event& e) override;

	private:
		std::shared_ptr<zr::OrthographicCameraController> mCameraController;
		std::shared_ptr<zr::Font> mFont;
		std::shared_ptr<zr::Text> mText;

		std::shared_ptr<zr::Sprite> mSprite;


		// Game things
		std::shared_ptr<PongGame> mGame;

		// Inherited via ViewportResizeListener
		virtual void onViewportUpdate(const glm::vec2& viewportSize) override;
	};
}
