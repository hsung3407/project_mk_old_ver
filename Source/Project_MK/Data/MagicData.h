#pragma once

#include "MagicData.generated.h"

UENUM()
enum ERuneType : uint8
{
	None,
	Fire,
	Compression,
	Emission,
	Enchant
};

USTRUCT()
struct FMagicData : public FTableRowBase
{
	GENERATED_BODY()
	FMagicData():Damage(10), Speed(100), Radius(100){};

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<ERuneType>> TriggerRunes;
	
	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	float Radius;

	UPROPERTY(EditAnywhere)
	TSet<TSubclassOf<class UMagicExecutor>> MagicExecutors;
	
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionTypes;
};
