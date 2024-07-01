#pragma once
#include "Project_MK/Data/ItemObject.h"
#include "InventoryComponent.generated.h"

enum ERuneType : uint8;
class UItemWidget;
class UItemObject;

USTRUCT()
struct FEquippedItem
{
	GENERATED_BODY()

	UPROPERTY()
	UItemObject* ItemObject;

	UPROPERTY()
	UItemWidget* EquipSlot;

	bool operator==(const FEquippedItem& OtherStruct) const
	{
		return OtherStruct.ItemObject == ItemObject && OtherStruct.EquipSlot == EquipSlot;
	}
};

UCLASS()
class UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	UInventoryComponent();

public:
	void AddItem(UItemObject* ItemObject)
	{
		if (ItemObject && !ItemList.Contains(ItemObject))
			ItemList.Emplace(ItemObject);
		UE_LOG(LogTemp, Warning, TEXT("Added %s"), *ItemObject->Name.ToString())
	};

	void EquipItem(UItemObject* ItemObject, UItemWidget* EquipSlot);
	void UnEquipItem(UItemWidget* EquipSlot);
	void RemoveItem(const FEquippedItem* EquippedItem);

	UItemObject* GetItemObjectByName(FName Name);

	FORCEINLINE TArray<FEquippedItem> GetEquipList() const { return EquipList; }
	FORCEINLINE TArray<UItemObject*> GetItemList() const { return ItemList; }
	FORCEINLINE TArray<TEnumAsByte<ERuneType>> GetRuneList() const { return EquipRunes; }

private:
	UPROPERTY()
	TArray<FEquippedItem> EquipList;

	UPROPERTY()
	TArray<UItemObject*> ItemList;

	UPROPERTY()
	TArray<TEnumAsByte<ERuneType>> EquipRunes;

	static UItemObject* CreateItemObjectByName(FName Name);
};