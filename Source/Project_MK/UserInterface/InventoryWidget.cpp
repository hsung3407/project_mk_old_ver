// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWidget.h"
#include "ItemInfoWidget.h"
#include "ProjectMKHUD.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/TileView.h"
#include "Project_MK/Character/Player/InventoryComponent.h"
#include "Project_MK/Character/Player/PlayerCharacter.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InventoryComponent = Cast<UInventoryComponent>(
		GetWorld()->GetFirstPlayerController()->GetCharacter()->GetComponentByClass(
			UInventoryComponent::StaticClass()));

	auto SlotBorderArray = Cast<UCanvasPanel>(WidgetTree->FindWidget(FName("EquipSlotCanvas")))->GetAllChildren();
	for (UWidget* SlotWidget : SlotBorderArray)
		if (auto InventoryItem = Cast<UItemWidget>(Cast<UBorder>(SlotWidget)->GetChildAt(0)))
			EquipSlotArray.Emplace(InventoryItem);

	OnFocusDelegate.BindUObject(this, &UInventoryWidget::ItemWidgetOnFocus);
	OnInteractDelegate.BindUObject(this, &UInventoryWidget::ItemWidgetOnInteract);
	UE_LOG(LogTemp, Warning, TEXT("Delegate Set %hs"), OnFocusDelegate.IsBound() ? "True" : "False")
	UE_LOG(LogTemp, Warning, TEXT("Delegate Set %hs"), OnInteractDelegate.IsBound() ? "True" : "False")

	SetInventory(false);
}

void UInventoryWidget::SetInventory(const bool Active)
{
	if (Active)
	{
		SelectedEquipSlot = EquipSlotArray[0];
		RefreshAll();
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
		for (const auto EquipSlot : EquipSlotArray)
		{
			EquipSlot->SetItem(nullptr);
			EquipSlot->SetSelect(false);
		}
		SelectedEquipSlot = nullptr;
		ItemInfoView->SetDisplayItemInfo(nullptr);
		ItemView->ClearListItems();
	}
}

void UInventoryWidget::RefreshAll()
{
	for (const auto EquipSlot : EquipSlotArray)
	{
		EquipSlot->SetItem(nullptr);
		EquipSlot->SetSelect(false);
	}
	for (const auto [ItemObject, EquipSlot] : InventoryComponent->GetEquipList())
	{
		EquipSlot->SetItem(ItemObject);
		// if (const auto ItemWidget = Cast<UItemWidget>(ItemView->GetEntryWidgetFromItem(ItemObject))) ItemWidget->SetEquip(true);
	}
	if (!SelectedEquipSlot) return;

	auto SetItemType = SelectedEquipSlot->EquipItemType;
	TArray<UObject*> Items = static_cast<TArray<UObject*>>(InventoryComponent->GetItemList().FilterByPredicate(
		[SetItemType](const UItemObject* ItemObject) {
			return ItemObject->ItemType == SetItemType;
		}));
	TArray<UObject*> EquippedItems;
	for (auto [ItemObject, EquipSlot] : InventoryComponent->GetEquipList()) if(ItemObject->ItemType == SetItemType) EquippedItems.Add(ItemObject);

	const int Num = Items.Num();
	for (int i = 0; i < Num; ++i)
	{
		if(EquippedItems.Contains(Items[i]))
		{
			const auto EquippedItemObject =  NewObject<UEquippedItemObject>(GetTransientPackage(), UEquippedItemObject::StaticClass());
			EquippedItemObject->ItemObject = Cast<UItemObject>(Items[i]);
			Items[i] = EquippedItemObject;
		}
	}

	ItemView->SetListItems(Items);
}

void UInventoryWidget::ItemWidgetOnFocus(UItemWidget* ItemWidget) const
{
	ItemInfoView->SetDisplayItemInfo(ItemWidget->ItemObject);
}

void UInventoryWidget::ItemWidgetOnInteract(UItemWidget* ItemWidget)
{
	if(EquipSlotArray.Contains(ItemWidget))
	{
		if (SelectedEquipSlot) SelectedEquipSlot->SetSelect(false);
		SelectedEquipSlot = ItemWidget;
		SelectedEquipSlot->SetSelect(true);
	}
	else if(SelectedEquipSlot)
	{
		auto ItemObject = Cast<UItemObject>(ItemWidget->ItemObject);
		if(!ItemObject) ItemObject = Cast<UEquippedItemObject>(ItemWidget->ItemObject)->ItemObject;
		InventoryComponent->EquipItem(ItemObject, SelectedEquipSlot);
	}
	RefreshAll();
}

//ToDo: 실험용 삭제 필요
FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Menu %s"), *InKeyEvent.GetKey().GetFName().ToString());
	if (InKeyEvent.GetKey().GetFName() == FName("Tab"))
		Cast<AProjectMKHUD>(
			GetWorld()->GetFirstPlayerController()->GetHUD())->ToggleMenu();

	// if(InKeyEvent.GetKey().GetFName() == FName("R")) ItemView->RequestNavigateToItem(ItemArray[17]);
	UE_LOG(LogTemp, Warning, TEXT("Bounded From Inventory %hs"), OnFocusDelegate.IsBound() ? "True" : "False")
	UE_LOG(LogTemp, Warning, TEXT("Bounded From Inventory %hs"), OnInteractDelegate.IsBound() ? "True" : "False")
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Mouse Down"));

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
