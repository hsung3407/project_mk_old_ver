#pragma once
#include "ItemObject.h"
#include "RuneItemObject.generated.h"

enum ERuneType : uint8;

UCLASS()
class URuneItemObject : public UItemObject
{
	GENERATED_BODY()

	URuneItemObject(): RuneType()
	{
	};

public:
	ERuneType RuneType;
};
