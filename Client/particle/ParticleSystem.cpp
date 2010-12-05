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

#include "ParticleSystem.h"

namespace part
{
	CParticleSystem::CParticleSystem() :
		Drawable(),	mIsVisible(true)
	{
		Clean();
	}

	CParticleSystem::~CParticleSystem()
	{
		Clean();
	}

	void CParticleSystem::Clean()
	{
		this->mEmitters.clear();
	}

	void CParticleSystem::Update(float dt)
	{
		EmitterIter it = this->mEmitters.begin();
		EmitterIter eIt = this->mEmitters.end();
		for(; it != eIt; ++it)
		{
			(*it)->Update(dt);
		}
	}

	void CParticleSystem::Play()
	{
		EmitterIter it = this->mEmitters.begin();
		EmitterIter eIt = this->mEmitters.end();
		for(; it != eIt; ++it)
		{
			(*it)->Play();
		}
	}

	void CParticleSystem::Stop()
	{
		EmitterIter it = this->mEmitters.begin();
		EmitterIter eIt = this->mEmitters.end();
		for(; it != eIt; ++it)
		{
			(*it)->Stop();
		}
	}

	void CParticleSystem::AddEmitter( IParticleEmitter::EmitterPtr emitter)
	{
		this->mEmitters.push_back(emitter);
	}

	void CParticleSystem::Render(sf::RenderTarget& rt, sf::Renderer& r) const
	{
		if(this->mIsVisible)
		{
			EmitterConstIter it = this->mEmitters.begin();
			EmitterConstIter eIt = this->mEmitters.end();
			for(; it != eIt; ++it)
			{
				(*it)->Render(rt);
			}
		}
	}

	sf::Int32 CParticleSystem::GetNumActiveParticles()
	{
		EmitterIter it = this->mEmitters.begin();
		EmitterIter eIt = this->mEmitters.end();
		sf::Int32 num = 0;
		for(; it != eIt; ++it)
			num += (*it)->GetNumActiveParticles();
		return num;
	}
}
