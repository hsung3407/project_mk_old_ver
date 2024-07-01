// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemWidget.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UCanvasPanel;
class UItemInfoWidget;
class UTileView;
class UItemObject;
class UInventoryComponent;

DECLARE_DELEGATE_OneParam(FItemWidgetDelegate, UItemWidget*)

inline FItemWidgetDelegate OnFocusDelegate;
inline FItemWidgetDelegate OnInteractDelegate;

/**
 * 
 */
UCLASS()
class PROJECT_MK_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

public:

	void SetInventory(bool Active);
	void RefreshAll();

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTileView> ItemView;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UItemInfoWidget> ItemInfoView;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> EquipSlotCanvas;
	
	UPROPERTY()
	TArray<UItemWidget*> EquipSlotArray;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY()
	TObjectPtr<UItemWidget> SelectedEquipSlot;

	void ItemWidgetOnFocus(UItemWidget* ItemWidget) const;
	void ItemWidgetOnInteract(UItemWidget* ItemWidget);

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
