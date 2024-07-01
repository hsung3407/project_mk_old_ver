// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Project_MK/Data/ItemObject.h"
#include "ItemWidget.generated.h"

enum class EItemType : uint8;
class UOverlay;
class UItemObject;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_MK_API UItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	void SetItem(UItemObject* Item);
	void SetSelect(bool Active);
	void SetEquip(const bool Active) const {EquipIconImage->SetVisibility(Active ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);}
	UPROPERTY()
	UItemObject* ItemObject;
	UPROPERTY(EditAnywhere)
	EItemType EquipItemType;
	UPROPERTY(EditAnywhere)
	int RuneSlotIndex;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> IconImage;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> EquipIconImage;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> QuantityText;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> ItemOverlay;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> EmptyIcon;

	void OnFocus();
	void OnInteract();

	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
};