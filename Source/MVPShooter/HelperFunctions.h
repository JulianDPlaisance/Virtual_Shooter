// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

namespace HelperFunctions
{
	//----------------------------- Clamp ---------------------------------
	//
	//  Clamps a 2D vector so that its magnitude does not exceed MaxMagnitude
	//------------------------------------------------------------------------
	inline void Clamp3DVector(FVector& V, const float MaxMag)
	{
		if (V.Size() > MaxMag)
		{
			V.Normalize();

			V *= MaxMag;
		}
	}
}
