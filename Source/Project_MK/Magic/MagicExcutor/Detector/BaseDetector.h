#pragma once

#include "Project_MK/Magic/MagicExcutor/MagicExecutor.h"
#include "BaseDetector.generated.h"

DECLARE_DELEGATE_OneParam(FDetectNotify, TArray<AActor*>)

UCLASS(Abstract)
class UBaseDetector : public UMagicExecutor
{
	GENERATED_BODY()
public:
	virtual void SetExecute(FMagicData* Row, const FMagicInfoContainer* Container) override;

protected:
	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionTypes;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;
	
	virtual void DetectNotify(TArray<AActor*> OtherActors) {DetectNotifyDelegate.Execute(OtherActors);}

private:
	FDetectNotify DetectNotifyDelegate;
};
