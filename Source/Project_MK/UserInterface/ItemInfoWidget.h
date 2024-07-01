// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ItemInfoWidget.generated.h"

class UItemObject;
/**
 * 
 */
UCLASS()
class PROJECT_MK_API UItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
		void SetDisplayItemInfo(const UItemObject* Item) const;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> NameT;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DescriptionT;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> BehindInformationT;
};
