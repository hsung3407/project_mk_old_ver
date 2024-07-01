#pragma once

#include "Project_MK/Magic/MagicExcutor/MagicExecutor.h"
#include "BaseApplier.generated.h"

UCLASS()
class UBaseApplier : public UMagicExecutor
{
	GENERATED_BODY()

	UBaseApplier();
	
public:
	virtual void SetExecute(FMagicData* Row, const FMagicInfoContainer* Container) override;

	virtual void DetectEvent(TArray<AActor*> Actors) override;
	
	UPROPERTY()
	float Damage;
};
