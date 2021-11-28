/************ (C) Copyright 2003 Valve, L.L.C. All rights reserved. ***********
**
** The copyright to the contents herein is the property of Valve, L.L.C.
** The contents may be used and/or copied only with the written permission of
** Valve, L.L.C., or in accordance with the terms and conditions stipulated in
** the agreement/contract under which the contents have been supplied.
**
*******************************************************************************
**
** Contents:
**
**	      interpolation.h: Bezier inpolation classes
**
******************************************************************************/

#pragma once

//  interpolation class
class CInterpolation
{
public:
	CInterpolation();
	virtual ~CInterpolation();

	void SetWaypoints(Vector* prev, Vector start, Vector end, Vector* next);
	void SetViewAngles(Vector start, Vector end);
	void SetFOVs(float start, float end);
	void SetSmoothing(bool start, bool end);

	// get interpolated point 0 =< t =< 1, 0 = start, 1 = end
	void Interpolate(float t, Vector& point, Vector& angle, float* fov);

protected:
	void BezierInterpolatePoint(float t, Vector& point);
	void InterpolateAngle(float t, Vector& angle);

	Vector m_StartPoint;
	Vector m_EndPoint;
	Vector m_StartAngle;
	Vector m_EndAngle;
	Vector m_Center;
	float m_StartFov;
	float m_EndFov;

	bool m_SmoothStart;
	bool m_SmoothEnd;
};
