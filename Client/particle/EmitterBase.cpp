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

#include <SFML/Graphics.hpp>
#include "EmitterBase.h"
#include <GL/gl.h>


namespace part
{
	//Set default values
	IParticleEmitter::IParticleEmitter() :
		mEmissionCounter(0.0f) ,
		mEmittNew(true) ,
		mPos(0,0) ,
		mPositionOffset(0,0) ,
		mStartColor(255, 255, 255, 255),
		mEndColor(255, 255, 255, 255),
		mIsGrayScale(false),
		mIsSingle(true),
		mMaterial(NULL) ,
		mMaxParticles(100) ,
		mWidth(8) ,
		mHeight(8) ,
		mEmissionRate(0.1f) ,
		mEmissionNum (1) ,
		mLiveTime (1.f) ,
		mLiveTimeVariation ( 0.f) ,
		mRotationSpeed(0.f),
		mBlendMode(sf::Blend::Alpha)
	{
		this->mAffectors.clear();
		this->mFreeParticles.clear();
		this->mParticles.clear();
	}

	IParticleEmitter::~IParticleEmitter()
	{
		Clean();
	}

	void IParticleEmitter::Render(sf::RenderTarget &rt) const
	{
		// Setup alpha-blending
		if (mBlendMode == sf::Blend::None)
		{
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
		}
		else
		{
			glEnable(GL_BLEND);
			glEnable(GL_ALPHA_TEST);
			switch (mBlendMode)
			{
			case sf::Blend::Alpha :    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
			case sf::Blend::Add :      glBlendFunc(GL_ONE, GL_ONE);                       break;
			case sf::Blend::Multiply : glBlendFunc(GL_DST_COLOR, GL_ZERO);                break;
			default : break;
			}
		}
		// Get the sprite size
		float halfWidth = this->mWidth/2;
		float halfHeight = this->mHeight/2;
		// Calculate the texture coordinates
		sf::FloatRect texCoords(0,0,1,1);

		if(this->mMaterial)
			this->mMaterial->Bind();

		//Iterate through all emitted particles
		ParticleConstIter it = this->mParticles.begin();
		ParticleConstIter eIt = this->mParticles.end();
		for(; it != eIt; ++it)
		{
			glPushMatrix();
			glTranslatef((*it)->GetPos().x, (*it)->GetPos().y, 0);
			glRotatef((*it)->GetRotation(),0,0,1);
			glScalef((*it)->GetScale(), (*it)->GetScale(), (*it)->GetScale());
			glColor4ub((*it)->GetColor().r, (*it)->GetColor().g, (*it)->GetColor().b, (*it)->GetColor().a);
			glBegin(GL_QUADS);
				glTexCoord2f(texCoords.Left,  texCoords.Top);	glVertex2f(-halfWidth, -halfHeight);
				glTexCoord2f(texCoords.Left,  texCoords.Top + texCoords.Height);	glVertex2f(-halfWidth, halfHeight);
				glTexCoord2f(texCoords.Left + texCoords.Width, texCoords.Top + texCoords.Height);	glVertex2f(halfWidth,	halfHeight);
				glTexCoord2f(texCoords.Left + texCoords.Width, texCoords.Top);	glVertex2f(halfWidth,	-halfHeight);
			glEnd();
			glColor4ub(255, 255, 255, 255);
			glPopMatrix();
		}
	}

	void IParticleEmitter::AddAffector(IParticleAffector::AffectorPtr affector)
	{
		affector->SetParentEmitter(this);
		mAffectors.push_back(affector);
	}

	void IParticleEmitter::Clean()
	{
		ParticleConstIter it = this->mParticles.begin();
		ParticleConstIter eIt = this->mParticles.end();
		for(; it != eIt; ++it)
		{
			CParticle *part = (*it);
			delete part;
		}
		mParticles.clear();

		it = this->mFreeParticles.begin();
		eIt = this->mFreeParticles.end();
		for(; it != eIt; ++it)
		{
			CParticle *part = (*it);
			delete part;
		}
		mFreeParticles.clear();
	}

	//Function for sorting particles
	bool LiveTimeSorter(const IParticleEmitter::ParticlePtr &lhs, const IParticleEmitter::ParticlePtr &rhs)
	{
		return lhs->GetLiveTime() > rhs->GetLiveTime();
	}

	void IParticleEmitter::Update(float dt)
	{
		//Emit new particles
		mEmissionCounter -= dt;
		if(mEmissionCounter < 0 && mEmittNew)
		{
			for(sf::Int32 i = 0; i < this->mEmissionNum; ++i)
			{
				if(mFreeParticles.empty())
					break;
				ParticlePtr part = mFreeParticles.back();

				this->InitParticle(part);

				this->mParticles.push_back(part);
				this->mFreeParticles.pop_back();
				mEmissionCounter = this->mEmissionRate;
			}
			//Sort the particles after how long the have left to live
			std::sort(mParticles.begin(), mParticles.end(), LiveTimeSorter);
		}

		//No need to update if ther are no spawned particles :)
		if(mParticles.empty())
			return;

		ParticleIter it = mParticles.begin();
		ParticleIter eIt = mParticles.end();
		ParticleIter delIt = mParticles.end();

		//Update particles
		AffectorIter affectIt;
		AffectorIter affectEndIt = mAffectors.end();
		for(; it != eIt; )
		{
			if((*it)->GetLiveTime() <= 0)
			{
				delIt = it;
				break;
			}
			//Apply affectors
			for(affectIt = mAffectors.begin(); affectIt != affectEndIt; ++affectIt)
			{
				(*affectIt)->Update((*it), dt);
			}
			//Rotate and move particles
			(*it)->Rotate(this->mRotationSpeed*dt);
			(*it)->Move(dt);

			++it;
		}

		//Erasing
		if(delIt != eIt)
		{
			mFreeParticles.insert(mFreeParticles.end(), delIt, eIt);
			mParticles.erase(delIt, eIt);
		}
	}

	void IParticleEmitter::Init()
	{
		this->mEmittNew = true;
		mParticles.clear();
		this->mFreeParticles.clear();

		mParticles.reserve(mMaxParticles);
		mFreeParticles.reserve(mMaxParticles);
		//Allocate our particle pool
		for(sf::Int32 i = 0; i < this->mMaxParticles; ++i)
		{
			ParticlePtr part(new CParticle(mPos, sf::Vector2f(0,1), 0, 0.f));
			this->mFreeParticles.push_back(part);
		}
		this->mEmissionCounter = 0;
	}


	sf::Color IParticleEmitter::RandomiseColor() const
	{
		if(mIsSingle)
			return mStartColor;

		sf::Color retVal;
		if(mIsGrayScale)
		{
			retVal.r = sf::Randomizer::Random(std::min(mStartColor.r, mEndColor.r), std::max(mStartColor.r, mEndColor.r));
			retVal.g = retVal.b = retVal.r;
		}
		else
		{
			retVal.r = sf::Randomizer::Random(std::min(mStartColor.r, mEndColor.r), std::max(mStartColor.r, mEndColor.r));
			retVal.g = sf::Randomizer::Random(std::min(mStartColor.g, mEndColor.g), std::max(mStartColor.g, mEndColor.g));
			retVal.b = sf::Randomizer::Random(std::min(mStartColor.b, mEndColor.b), std::max(mStartColor.b, mEndColor.b));
		}
		retVal.a = sf::Randomizer::Random(std::min(mStartColor.a, mEndColor.a), std::max(mStartColor.a, mEndColor.a));

		return retVal;
	}
}
