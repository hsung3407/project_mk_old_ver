#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

UENUM()
enum class EItemType : uint8
{
	Rune,
	Equipment,
	Consumable,
	Ingredient
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere)
	FText Description;
	
	UPROPERTY(EditAnywhere)
	EItemType ItemType;
};
