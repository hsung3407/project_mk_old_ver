// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemWidget.h"

#include "InventoryWidget.h"
#include "Components/ListViewBase.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Project_MK/Data/ItemObject.h"

void UItemWidget::OnFocus()
{
	OnFocusDelegate.ExecuteIfBound(this);
}

void UItemWidget::OnInteract()
{
	OnInteractDelegate.ExecuteIfBound(this);
}

void UItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	IconImage->SetVisibility(ESlateVisibility::Collapsed);
	EmptyIcon->SetVisibility(ESlateVisibility::Visible);
	QuantityText->SetVisibility(ESlateVisibility::Collapsed);
	EquipIconImage->SetVisibility(ESlateVisibility::Collapsed);
}

void UItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	SetEquip(false);
	UItemObject* SetItemObject = Cast<UItemObject>(ListItemObject);
	if(!SetItemObject)
	{
		SetEquip(true);
		SetItemObject = Cast<UEquippedItemObject>(ListItemObject)->ItemObject;
	}
	SetItem(SetItemObject);
}

void UItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	SetFocus();
}

FReply UItemWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	OnFocus();
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
}

FReply UItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnInteract();
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UItemWidget::SetItem(UItemObject* Item)
{
	ItemObject = Item;
	if (!Item)
	{
		IconImage->SetVisibility(ESlateVisibility::Collapsed);
		EmptyIcon->SetVisibility(ESlateVisibility::Visible);
		QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	IconImage->SetBrushFromTexture(Item->Icon);
	EmptyIcon->SetVisibility(ESlateVisibility::Collapsed);
	IconImage->SetVisibility(ESlateVisibility::Visible);
	ItemOverlay->SetVisibility(ESlateVisibility::Visible);
}

void UItemWidget::SetSelect(bool Active)
{
	//선택되었을 때 표시 - 예) 테두리가 밝게 빛나는 등의 효과
}