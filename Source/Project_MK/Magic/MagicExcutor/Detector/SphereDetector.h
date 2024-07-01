#pragma once

#include "BaseDetector.h"
#include "Project_MK/Magic/MagicExcutor/MagicExecutor.h"
#include "SphereDetector.generated.h"

UCLASS()
class USphereDetector : public UBaseDetector
{
	GENERATED_BODY()
public:
	virtual void SetExecute(FMagicData* Row, const FMagicInfoContainer* Container) override;
	virtual void TickEvent(const float DeltaTime) override;
	
private:
	UPROPERTY()
	AActor* OwnerActor;
	
	UPROPERTY()
	float Radius;
};
