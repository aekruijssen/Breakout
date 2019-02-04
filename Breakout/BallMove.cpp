#include "BallMove.h"
#include "Actor.h"
#include "Ball.h"
#include "Paddle.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Block.h"
#include "Math.h"

// MoveComponent constructor - initialize order to 50 and speeds to 0
BallMove::BallMove(class Ball* owner)
	:MoveComponent(owner)
	, mAngularSpeed(0.0f)
	, mForwardSpeed(0.0f)
{
	mBall = owner;
	mBallVel.x = 250;
	mBallVel.y = -250;
}


// Update - updates position/rotation of Actor
void BallMove::Update(float deltaTime)
{
	// if angular speed is not near zero -> increment rotation by angular speed * deltaTime
	if (!Math::NearZero(mAngularSpeed)) {
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}
	// if forward speed is not near zero -> increment position by forward vector * forward speed * delataTime
	if (!Math::NearZero(mForwardSpeed)) {
		Vector2 pos = mOwner->GetPosition();
		//pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
		pos.x += (mBallVel).x * deltaTime;
		pos.y += (mBallVel).y * deltaTime;
		mOwner->SetPosition(pos);
	}

	//top wall -> bounce
	if (mBall->GetPosition().y <= 40 && (mBallVel).y < 0.0f) {
		(mBallVel).y *= -1;
	}

	//right wall -> bounce
	if (mBall->GetPosition().x >= 1024 - 40 && (mBallVel).x > 0.0f) {
		(mBallVel).x *= -1;
	}

	//left wall -> bounce
	if (mBall->GetPosition().x <= 0 + 40 && (mBallVel).x < 0.0f) {
		(mBallVel).x *= -1;
	}

	//bottom wall -> reset
	if (mBall->GetPosition().y > 768.0f) {
		mBallVel.x = 250;
		mBallVel.y = -250;

		Vector2 resetPos;
		resetPos.x = 512;
		resetPos.y = 384;
		mBall->SetPosition(resetPos);
	}

	Paddle* pad = (mBall->GetGame())->GetPaddle();

	if ((mBall->cc)->Intersect(((mBall->GetGame())->GetPaddle())->cc)) {
		Vector2 n;
		Vector2 v = mBallVel;

		float paddleWidth = (((mBall->GetGame())->GetPaddle())->cc)->GetWidth();
		float thirdlength = paddleWidth/3.0f;
		int third;
		if (mBall->GetPosition().x <= (pad->cc->GetMin()).x + thirdlength) {
			third = 1;
		}
		else if (mBall->GetPosition().x <= (pad->cc->GetMin()).x + (thirdlength * 2)) {
			third = 2;
		}
		else {
			third = 3;
		}

		//If bouncing off the left third, normal vector rotated to the left
		if (third == 1) {
			n.x = -2;
			n.y = -10;
			n.Normalize();
		}
		//If bouncing off the middle, normal vector pointing straight up
		else if (third == 2) {
			n.x = 0;
			n.y = -10;
			n.Normalize();
		}
		//If bouncing off the right third, normal vector rotated to the right.
		else if (third == 3){
			n.x = 2;
			n.y = -10;
			n.Normalize();
		}
		Vector2 rot = Vector2::Reflect(v, n);
		mBallVel.x = rot.x;
		if (mBallVel.y > 0) {
			mBallVel.y = rot.y;
		}
	}
	
	//Block Collision
	for (auto block : (mBall->GetGame())->mBlocks) {
		float blockX = block->GetPosition().x;
		float blockY = block->GetPosition().y;
		Vector2* blockPos = new Vector2(blockX, blockY);
		float ballX = mBall->GetPosition().x;
		float ballY = mBall->GetPosition().y;
		Vector2* ballPos = new Vector2(ballX, ballY);

		CollSide cs = (mBall->cc)->GetMinOverlap(block->cc, *ballPos);
		if (cs != CollSide::None) {
			if (cs == CollSide::Left) {
				mBallVel.x *= -1.0f;
			}
			else if (cs == CollSide::Right) {
				mBallVel.x *= -1.0f;
			}
			else if (cs == CollSide::Top) {
				mBallVel.y *= -1.0f;
			}
			else if (cs == CollSide::Bottom) {
				mBallVel.y *= -1.0f;
			}
			block->SetState(ActorState::Destroy);
			break;
		}
	}
}
