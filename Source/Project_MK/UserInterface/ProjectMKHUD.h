// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InventoryWidget.h"
#include "ProjectMKHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MK_API AProjectMKHUD : public AHUD
{
	GENERATED_BODY()

	AProjectMKHUD(){};

	virtual void BeginPlay() override;
public:
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;

	void ToggleMenu() const;
private:
	
};
