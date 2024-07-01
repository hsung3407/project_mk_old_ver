#pragma once

#include "Project_MK/Magic/MagicExcutor/MagicExecutor.h"
#include "ForwardMover.generated.h"

UCLASS()
class UForwardMover : public UMagicExecutor
{
	GENERATED_BODY()

public:
	virtual void SetExecute(FMagicData* Row, const FMagicInfoContainer* Container) override;

	virtual void TickEvent(const float DeltaTime) override;

private:
	UPROPERTY()
	AActor* OwnerActor;
	
	UPROPERTY()
	float Speed;
};
