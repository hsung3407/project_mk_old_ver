#pragma once

#include "Project_MK/Magic/MagicExcutor/MagicExecutor.h"
#include "DetectDestroyer.generated.h"

UCLASS()
class UDetectDestroyer : public UMagicExecutor
{
	GENERATED_BODY()
public:
	virtual void SetExecute(FMagicData* Row, const FMagicInfoContainer* Container) override;
	
	virtual void DetectEvent(TArray<AActor*> Actors) override;

private:
	UPROPERTY()
	AActor* OwnerActor;
};
