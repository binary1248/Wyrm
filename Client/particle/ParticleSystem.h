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

#ifndef _PARTICLE_PARTICLESYSTEM_H_
#define _PARTICLE_PARTICLESYSTEM_H_

#include "EmitterBase.h"
#include "AffectorBase.h"

namespace part
{
	class CParticleSystem : public sf::Drawable
	{
	protected:
		typedef std::vector<IParticleEmitter::EmitterPtr> EmitterCont;
		typedef EmitterCont::iterator EmitterIter;
		typedef EmitterCont::const_iterator EmitterConstIter;

	public:
		CParticleSystem();
		virtual ~CParticleSystem();

		//Called on cleanup
		virtual void Clean();
		//Called evry frame
		virtual void Update(float dt);
		//Starts the system
		virtual void Play();
		//Makes all emiters stop emiting and cleans when all particles are dead
		virtual void Stop();

		virtual void AddEmitter(IParticleEmitter::EmitterPtr emitter);

		//Are we showing the system?
		inline bool IsVisible() { return mIsVisible; }
		inline void SetVisibility(bool visible) { mIsVisible = visible; }

		sf::Int32 GetNumActiveParticles();

	protected:
		//Render particle system
		virtual void Render(sf::RenderTarget& rt, sf::Renderer& r) const;

		bool mIsVisible;
		EmitterCont mEmitters;
	};
}

#endif //_PARTICLE_PARTICLESYSTEM_H_
