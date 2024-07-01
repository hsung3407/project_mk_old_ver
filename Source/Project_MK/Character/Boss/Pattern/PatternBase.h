#pragma once

#include "CoreMinimal.h"

#include "PatternBase.generated.h"

UCLASS(Blueprintable)
class UPatternBase : public UObject
{
	GENERATED_BODY()

	UPatternBase(): Montage(nullptr), Priority(0), MinAnimTime(0), DelayTime(0), MinDistance(0), MaxDistance(100000),
	                MinHPRatio(0),
	                MaxHPRatio(100), bIgnoreGravity(false), bAnimLoop(false), LoopQuantum(1)
	{
	}

public:
	UPROPERTY(EditAnywhere, Category="Default")
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	int Priority;

	UPROPERTY(EditAnywhere)
	float MinAnimTime;
	
	UPROPERTY(EditAnywhere)
	float DelayTime;
	
	UPROPERTY(EditAnywhere)
	float MinDistance;
	
	UPROPERTY(EditAnywhere)
	float MaxDistance;

	UPROPERTY(EditAnywhere)
	float MinHPRatio;

	UPROPERTY(EditAnywhere)
	float MaxHPRatio;
	
	UPROPERTY(EditAnywhere)
	bool bIgnoreGravity;

	UPROPERTY(EditAnywhere)
	bool bAnimLoop;

	UPROPERTY(EditAnywhere)
	float LoopQuantum;
};
