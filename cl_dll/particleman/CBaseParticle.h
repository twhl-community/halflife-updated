#pragma once

#define TRI_COLLIDEWORLD 0x00000020
#define TRI_COLLIDEALL 0x00001000  // will collide with world and slideboxes
#define TRI_COLLIDEKILL 0x00004000 // tent is removed upon collision with anything
#define TRI_SPIRAL 0x00008000
#define TRI_ANIMATEDIE 0x00016000 //animate once and then die
#define TRI_WATERTRACE 0x00032000


#define CULL_FRUSTUM_POINT (1 << 0)
#define CULL_FRUSTUM_SPHERE (1 << 1)
#define CULL_FRUSTUM_PLANE (1 << 2)
#define CULL_PVS (1 << 3)

#define LIGHT_NONE (1 << 4)
#define LIGHT_COLOR (1 << 5)
#define LIGHT_INTENSITY (1 << 6)

#define RENDER_FACEPLAYER (1 << 7)		   // m_vAngles == Player view angles
#define RENDER_FACEPLAYER_ROTATEZ (1 << 8) //Just like above but m_vAngles.z is untouched so the sprite can rotate.


#include "CMiniMem.h"

//pure virtual baseclass
class CBaseParticle
{
private:
	int m_iRenderFlags;
	float m_flNextPVSCheck;
	bool m_bInPVS;

	int m_iCollisionFlags;
	float m_flPlayerDistance; //Used for sorting the particles, DO NOT TOUCH.

public:
	void* operator new(size_t size)
	{
		return CMiniMem::Instance()->Allocate(size);
	}

	void* operator new(size_t size, std::align_val_t alignment)
	{
		return CMiniMem::Instance()->Allocate(size, static_cast<std::size_t>(alignment));
	}

	void operator delete(void* memory, std::size_t size)
	{
		CMiniMem::Instance()->Deallocate(memory, size);
	}

	void operator delete(void* memory, std::size_t size, std::align_val_t alignment)
	{
		CMiniMem::Instance()->Deallocate(memory, size, static_cast<std::size_t>(alignment));
	}

	virtual void Think(float time);
	virtual bool CheckVisibility(void);
	virtual void Draw(void);
	virtual void Animate(float time);
	virtual void AnimateAndDie(float time);
	virtual void Expand(float time);
	virtual void Contract(float time);
	virtual void Fade(float time);
	virtual void Spin(float time);
	virtual void CalculateVelocity(float time);
	virtual void CheckCollision(float time);
	virtual void Touch(Vector pos, Vector normal, int index);
	virtual void Die(void);
	virtual void InitializeSprite(Vector org, Vector normal, model_s* sprite, float size, float brightness);
	virtual void Force(void);

	float m_flSize;			 //scale of object
	float m_flScaleSpeed;	 //speed at which object expands
	float m_flContractSpeed; //speed at which object expands

	float m_flStretchX;
	float m_flStretchY;

	float m_flBrightness; //transparency of object
	float m_flFadeSpeed;  //speed at which object fades

	float m_flTimeCreated; //time object was instanced
	float m_flDieTime;	   //time to remove an object

	float m_flGravity; //how effected by gravity is this object
	float m_flAfterDampGrav;
	float m_flDampingVelocity;
	float m_flDampingTime;

	int m_iFramerate;
	int m_iNumFrames;
	int m_iFrame;
	int m_iRendermode;

	Vector m_vOrigin; //object's position
	Vector m_vAngles; //normal angles of object

	Vector m_vAVelocity;

	Vector m_vVelocity;

	Vector m_vLowLeft;
	Vector m_vLowRight;
	Vector m_vTopLeft;

	Vector m_vColor;
	float m_flMass;

	model_s* m_pTexture;

	float m_flBounceFactor;

	char m_szClassname[32];

	bool m_bInWater;
	bool m_bAffectedByForce;

	int m_iAfterDampFlags;

	void SetLightFlag(int iFlag)
	{
		m_iRenderFlags &= ~(LIGHT_NONE | LIGHT_INTENSITY | LIGHT_COLOR);
		m_iRenderFlags |= iFlag;
	}

	void SetCullFlag(int iFlag)
	{
		m_iRenderFlags &= ~(CULL_PVS | CULL_FRUSTUM_POINT | CULL_FRUSTUM_PLANE | CULL_FRUSTUM_SPHERE);
		m_iRenderFlags |= iFlag;
	}

	int GetRenderFlags(void)
	{
		return m_iRenderFlags;
	}

	bool GetParticlePVS(void)
	{
		return m_bInPVS;
	}

	void SetParticlePVS(bool bPVSStat)
	{
		m_bInPVS = bPVSStat;
	}

	float GetNextPVSCheck(void)
	{
		return m_flNextPVSCheck;
	}

	void SetNextPVSCheck(float flTime)
	{
		m_flNextPVSCheck = flTime;
	}

	void SetCollisionFlags(int iFlag)
	{
		m_iCollisionFlags |= iFlag;
	}

	void ClearCollisionFlags(int iFlag)
	{
		m_iCollisionFlags &= ~iFlag;
	}

	int GetCollisionFlags(void)
	{
		return m_iCollisionFlags;
	}

	void SetRenderFlag(int iFlag)
	{
		m_iRenderFlags |= iFlag;
	}

	float GetPlayerDistance() const { return m_flPlayerDistance; }
	void SetPlayerDistance(float flDistance) { m_flPlayerDistance = flDistance; }

protected:
	float m_flOriginalSize;
	Vector m_vOriginalAngles;
	float m_flOriginalBrightness;
	Vector m_vPrevOrigin;

	float m_flNextCollisionTime;
};
