// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfoWidget.h"
#include "Project_MK/Data/ItemObject.h"

void UItemInfoWidget::SetDisplayItemInfo(const UItemObject* Item) const
{
	if(Item == nullptr)
	{
		NameT->SetText(INVTEXT("-"));
		DescriptionT->SetText(INVTEXT("-"));
		BehindInformationT->SetText(INVTEXT("-"));
	}
	else
	{
		NameT->SetText(FText::FromName(Item->Name));
		DescriptionT->SetText(Item->Description);
		// BehindInformationT->SetText(Item->Data->DisplayData.BehindInformation);
	}
}
