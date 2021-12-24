#pragma once

#include "interface.h"
#include "pman_triangleffect.h"

#define PARTICLEMAN_INTERFACE "create_particleman"

class CBaseParticle;

class IParticleMan : public IBaseInterface
{

protected:
	virtual ~IParticleMan() {}

public:
	virtual void SetUp(cl_enginefunc_t* pEnginefuncs) = 0;
	virtual void Update(void) = 0;
	virtual void SetVariables(float flGravity, Vector vViewAngles) = 0;
	virtual void ResetParticles(void) = 0;
	virtual void ApplyForce(Vector vOrigin, Vector vDirection, float flRadius, float flStrength, float flDuration) = 0;
	virtual void AddCustomParticleClassSize(unsigned long lSize) = 0;

	//Use this if you want to create a new particle without any overloaded functions, Think, Touch, etc.
	//Just call this function, set the particle's behavior and let it rip.
	virtual CBaseParticle* CreateParticle(Vector org, Vector normal, model_s* sprite, float size, float brightness, const char* classname) = 0;

	//Use this to take a block from the mempool for custom particles ( used in new ).
	virtual char* RequestNewMemBlock(int iSize) = 0;

	virtual void SetRender(int iRender) = 0;
};

extern IParticleMan* g_pParticleMan;

class CBaseParticle : public CCoreTriangleEffect
{
public:
	void* operator new(size_t size) //this asks for a new block of memory from the MiniMem class
	{
		return (g_pParticleMan->RequestNewMemBlock(size));
	}
#ifdef POSIX
	void* operator new(size_t size, const std::nothrow_t&) throw() //this asks for a new block of memory from the MiniMem class
	{
		return (g_pParticleMan->RequestNewMemBlock(size));
	}
#endif
};
