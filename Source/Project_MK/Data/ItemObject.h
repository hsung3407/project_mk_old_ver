// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "ItemObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MK_API UItemObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName Name;

	UPROPERTY()
	UTexture2D* Icon;
	
	UPROPERTY()
	FText Description;

	UPROPERTY()
	EItemType ItemType;
};

UCLASS()
class UEquippedItemObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UItemObject* ItemObject;
};
