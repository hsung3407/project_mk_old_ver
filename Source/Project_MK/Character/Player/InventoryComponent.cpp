#include "InventoryComponent.h"

#include "Project_MK/Data/RuneData.h"
#include "Project_MK/Data/RuneItemObject.h"
#include "Project_MK/Magic/Magic.h"
#include "Project_MK/UserInterface/ItemWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	EquipRunes.Reset();
	EquipRunes.Init(None,5);
}

void UInventoryComponent::EquipItem(UItemObject* ItemObject, UItemWidget* EquipSlot)
{
	FEquippedItem RemoveByItem {nullptr, nullptr};
	FEquippedItem RemoveBySlot {nullptr, nullptr};
	for (const auto Item : EquipList)
	{
		if (Item.ItemObject == ItemObject && Item.EquipSlot == EquipSlot) return;
		else if (Item.ItemObject == ItemObject) RemoveByItem = Item;
		else if (Item.EquipSlot == EquipSlot) RemoveBySlot = Item;
	}
	RemoveItem(&RemoveByItem);
	RemoveItem(&RemoveBySlot);

	EquipList.Emplace(FEquippedItem(ItemObject, EquipSlot));

	if (ItemObject->ItemType == EItemType::Rune)
		EquipRunes[EquipSlot->RuneSlotIndex] = Cast<URuneItemObject>(ItemObject)->RuneType;
}

void UInventoryComponent::UnEquipItem(UItemWidget* EquipSlot)
{
	if (const auto FoundEquippedItem = EquipList.FindByPredicate([&EquipSlot](const FEquippedItem EquippedItem) {
		return EquipSlot == EquippedItem.EquipSlot;
	}))
		RemoveItem(FoundEquippedItem);
}

void UInventoryComponent::RemoveItem(const FEquippedItem* EquippedItem)
{
	EquipList.Remove(*EquippedItem);
	if(const auto EquipSlot =EquippedItem->EquipSlot)
		if(const auto ItemObject = EquipSlot->ItemObject)
			if(ItemObject->ItemType == EItemType::Rune)
				EquipRunes[EquippedItem->EquipSlot->RuneSlotIndex] = None;
}

UItemObject* UInventoryComponent::GetItemObjectByName(FName Name)
{
	if (const auto FoundItemPointer = ItemList.FindByPredicate([&Name](const UItemObject* ItemObject) {
		return ItemObject->Name == Name;
	}))
		return *FoundItemPointer;
	return CreateItemObjectByName(Name);
}

UItemObject* UInventoryComponent::CreateItemObjectByName(const FName Name)
{
	const auto ItemTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTable/Item.Item"));

	if (!ItemTable->GetRowNames().Contains(Name))
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Name Not Found : ""%s"" "), *Name.ToString())
		return nullptr;
	}

	const auto Row = ItemTable->FindRow<FItemData>(Name, "");
	UItemObject* ItemObject = nullptr;
	if(Row->ItemType == EItemType::Rune)
	{
		const auto RuneTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTable/Rune.Rune"));
		const auto RuneItemObject = NewObject<URuneItemObject>(GetTransientPackage(), URuneItemObject::StaticClass());
		RuneItemObject->RuneType = RuneTable->FindRow<FRuneData>(Name, "")->RuneType;
		ItemObject = RuneItemObject;
	}
	else ItemObject = NewObject<UItemObject>(GetTransientPackage(), UItemObject::StaticClass());
	ItemObject->Name = Name;
	ItemObject->Icon = Row->Icon;
	ItemObject->Description = Row->Description;
	ItemObject->ItemType = Row->ItemType;
	return ItemObject;
}
