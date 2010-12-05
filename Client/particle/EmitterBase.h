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


#ifndef _PARTICLE_EMITTERBASE_H_
#define _PARTICLE_EMITTERBASE_H_

#include <vector>
#include "SFML/Graphics.hpp"
#include "Particle.h"
#include "AffectorBase.h"

namespace part
{
	class CParticleSystem;

	class IParticleEmitter
	{
	public:
		typedef IParticleEmitter* EmitterPtr;

		typedef CParticle* ParticlePtr;

		typedef std::vector<ParticlePtr> ParticleCont;
		typedef ParticleCont::iterator ParticleIter;
		typedef ParticleCont::const_iterator ParticleConstIter;


		typedef std::vector<IParticleAffector::AffectorPtr> AffectorCont;
		typedef AffectorCont::iterator AffectorIter;
		typedef AffectorCont::const_iterator AffectorConstIter;
	public:

		IParticleEmitter();
		virtual ~IParticleEmitter();

		/// Init
		/// Needs to be called after all properties of the particle emitter is set
		virtual void Init();

		/// Update
		/// Updates all particles the emitter 
		virtual void Update(float dt);

		virtual void Clean();

		virtual void Play() { mEmittNew = true; }
		virtual void Stop() { mEmittNew = false; }

		ParticleIter ParticleBegin() { return mParticles.begin(); }
		ParticleConstIter ParticleBegin() const { return mParticles.begin(); }
		ParticleIter ParticleEnd() { return mParticles.end(); }
		ParticleConstIter ParticleEnd() const { return mParticles.end(); }

		sf::Int32 GetNumActiveParticles() const { return static_cast<sf::Int32>(mParticles.size()); }

		void SetPosition(const sf::Vector2f &vec) { mPos = vec; }
		void SetPosX(const float &x) { mPos.x = x; }
		void SetPosY(const float &y) { mPos.y = y; }
		const sf::Vector2f &GetPos() const { return mPos; }

		//Renders all emitters particles
		virtual void Render(sf::RenderTarget &rt) const;

		void AddAffector(IParticleAffector::AffectorPtr affector);
		AffectorIter AffectorBegin() { return mAffectors.begin(); }
		AffectorConstIter AffectorBegin() const { return mAffectors.begin(); }
		AffectorIter AffectorEnd() { return mAffectors.end(); }
		AffectorConstIter AffectorEnd() const { return mAffectors.end(); }

		/// Particle emission properties

		void SetColorRange(const sf::Color &start, const sf::Color &end)		{ mStartColor = start; mEndColor = end; mIsSingle = false; }
		void SetColorRandom()													{ mStartColor = sf::Color(0, 0, 0, 0); mEndColor = sf::Color(255, 255, 255, 255); mIsSingle = false;}
		void SetColorSingle(const sf::Color &color)								{ mStartColor = color; mEndColor = color; mIsSingle = true; };

		void SetColorAlphaRange(const sf::Uint8 start, const sf::Uint8 end)		{ mStartColor.a = start; mEndColor.a = end; }
		void SetColorAlphaRandom()												{ mStartColor.a = 0; mEndColor.a = 255; }
		void SetColorAlphaSingle(const sf::Uint8 val)							{ mStartColor.a = val; mEndColor.a = val; };
	
		void SetGrayScale(const bool grayscale)		{ mIsGrayScale = grayscale; }
		bool GetGrayScale() const					{ return mIsGrayScale; }

		void SetMaterial(sf::Image *material)	{ mMaterial = material; }
		sf::Image *GetMaterial()				{ return mMaterial; }

		void SetEmissionRate(float rate, sf::Int32 num)			{ mEmissionRate = rate;  mEmissionNum = num; }
		void SetMaxParticles(sf::Int32 num)						{ mMaxParticles = num; }
		void SetLiveTime(float live_time, float variation)		{ mLiveTime = live_time; mLiveTimeVariation = variation; }
		void SetInitialDimensions(float width, float height)	{ mWidth = width; mHeight = height; }
		void SetRotationSpeed(float speed)						{ mRotationSpeed = speed; }
		void SetBlendMode(sf::Blend::Mode mode)					{ mBlendMode = mode; }
		void SetOffset(sf::Vector2f offset)						{ mPositionOffset = offset; }

	protected:

		virtual void InitParticle(ParticlePtr particle) = 0;

		virtual sf::Color RandomiseColor() const;

		float mEmissionCounter;
		bool mEmittNew;

		//Settings
		sf::Vector2f mPos;
		sf::Vector2f mPositionOffset;
		sf::Color mStartColor, mEndColor;
		bool mIsGrayScale;
		bool mIsSingle;
		sf::Image *mMaterial;
		sf::Int32 mMaxParticles;
		float mWidth, mHeight;
		float mEmissionRate;
		sf::Int32 mEmissionNum;
		float mLiveTime;
		float mLiveTimeVariation;
		float mRotationSpeed;
		sf::Blend::Mode mBlendMode;

	protected:
		ParticleCont mFreeParticles;
		ParticleCont mParticles;
		AffectorCont mAffectors;
	};
}

#endif //_PARTICLE_EMITTERBASE_H_
