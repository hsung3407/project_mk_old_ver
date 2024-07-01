#pragma once
#include "Project_MK/Data/MagicData.h"

#include "MagicExecutor.generated.h"

struct FMagicInfoContainer;
DECLARE_DELEGATE_OneParam(FExecuteRemoveSelf, class UMagicExecutor*)

UENUM()
enum EExecuteType : uint8
{
	Tick,
	DetectEvent,
	DestroyEvent,
	Independent,
};

UCLASS(Abstract)
class UMagicExecutor : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void SetExecute(FMagicData* Row, const FMagicInfoContainer* Container);
	virtual void TickEvent(const float DeltaTime) {}
	virtual void DetectEvent(TArray<AActor*> Actors) {}
	virtual void DestroyedEvent(bool bDestroyed) {}

protected:
	virtual void SelfDispose() {RemoveSelf.Execute(this);}

private:
	FExecuteRemoveSelf RemoveSelf;
};
