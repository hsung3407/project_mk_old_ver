// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectMKHUD.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void AProjectMKHUD::BeginPlay()
{
	Super::BeginPlay();

	const FString AssetPath = FString::Printf( TEXT( "/Game/Blueprint/UserInterface/Inventory/InventoryMenuBP.InventoryMenuBP" ));
	UObject* Class = StaticLoadObject(UObject::StaticClass(), nullptr, *AssetPath);
	const UBlueprint* BP_Actor = Cast<UBlueprint>(Class);
	const TSubclassOf<UInventoryWidget> InventoryMenuClass{BP_Actor->GeneratedClass};
	
	InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryMenuClass);
	if(!InventoryWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Load Failed"))
	}
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AProjectMKHUD::ToggleMenu() const
{
	UE_LOG(LogTemp, Warning, TEXT("Toggle Menu"))
	const bool bIsVisible = InventoryWidget->IsVisible();
	
	InventoryWidget->SetInventory(!bIsVisible);
	InventoryWidget->SetFocus();
	auto PlayerController = GetOwningPlayerController();
	PlayerController->SetShowMouseCursor(!bIsVisible);
	if(bIsVisible)
	{
		UE_LOG(LogTemp, Warning, TEXT("Off"));
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("On"));
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController);
	}
}
