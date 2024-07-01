#pragma once
#include "RuneData.generated.h"

enum ERuneType : uint8;

USTRUCT()
struct FRuneData : public FTableRowBase
{
	GENERATED_BODY()

	FRuneData(): RuneType()
	{
	}

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ERuneType> RuneType;
};
