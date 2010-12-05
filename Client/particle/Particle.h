////////////////////////////////////////////////////////////
//
// sfml-particle - a particle systemn for the SFML library
// Copyright (C) 2007-2008 Jonas Erixon (jonas.erixon@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef _PARTICLE_PARTICLE_H_
#define _PARTICLE_PARTICLE_H_

#include "SFML/System.hpp"

namespace part
{
	//A single particle
	class CParticle
	{
	public:
		CParticle() {}
		CParticle(const sf::Vector2f &pos, const sf::Vector2f &dir, float speed, float liveTime, const sf::Color &color = sf::Color(255, 255, 255, 255), const float angle = 0, const float scale = 1) 
			: mPos(pos),
			mColor(color),
			mStartColor(color),
			mAngle(angle),
			mSpeed(speed),
			mDir(dir),
			mScale(scale),
			mLiveTime(liveTime),
			mStartLiveTime(liveTime)
		{  }
		virtual ~CParticle() {}
		
		void Init(const sf::Vector2f &pos, const sf::Vector2f &dir, float speed, float liveTime, const sf::Color &color = sf::Color(255, 255, 255, 255), const float angle = 0, const float scale = 1) 
		{
			mDir = dir;
			mSpeed = speed;
			mPos = pos;
			mColor = color;
			mAngle = angle;
			mScale = scale;
			mLiveTime = liveTime;
			mStartLiveTime = liveTime;
			mStartColor = color;
		}
		
		void SetPos(const sf::Vector2f &pos)	{ mPos = pos; }
		const sf::Vector2f &GetPos() const		{ return mPos; }

		void Rotate(float angle)				{ mAngle += angle; }
		void SetRotation(float angle)			{ mAngle = angle; }
		float GetRotation() const				{ return mAngle; }

		void SetScale(float scale)				{ mScale = scale; }
		float GetScale() const					{ return mScale; }

		void SetColor(const sf::Color &color)	{ mColor = color; }
		const sf::Color GetColor() const		{ return mColor; }
		const sf::Color &GetStartColor() const	{ return mStartColor; }
		void SetAlpha(sf::Uint8 alpha)			{ mColor.a = alpha; }
		const float GetAlpha() const			{ return mColor.a; }

		void SetSpeed(float speed)				{ mSpeed = speed; }
		float GetSpeed() const					{ return mSpeed; }

		void SetDir(const sf::Vector2f &dir)	{ mDir = dir; }
		sf::Vector2f GetDir() const				{ return mDir; }

		void SetLiveTime(float time)			{ mLiveTime = time; mStartLiveTime = time; }
		float GetLiveTime() const				{ return mLiveTime; }
		float GetStartLiveTime() const			{ return mStartLiveTime; }

		void Move(float dt)
		{ 
			this->mLiveTime -= dt;
			this->mPos += (mDir * mSpeed) * dt; 
		}

	protected:

		sf::Vector2f mPos;
		sf::Color mColor;
		sf::Color mStartColor;
		float mAngle;
		float mSpeed;
		sf::Vector2f mDir;
		float mScale;
		float mLiveTime;
		float mStartLiveTime;

	};
}
#endif //_PARTICLE_PARTICLE_H_
