#pragma once
#include "MoveComponent.h"
#include "Math.h"


class BallMove : public MoveComponent
{
public:
	BallMove(class Ball* owner);

	// Update the move component
	void Update(float deltaTime) override;

	// Getters/setters
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

	Ball* mBall;
	Vector2 mBallVel;

private:
	// Angular speed (in radians/second)
	float mAngularSpeed;
	// Forward speed (in pixels/second)
	float mForwardSpeed;
};
