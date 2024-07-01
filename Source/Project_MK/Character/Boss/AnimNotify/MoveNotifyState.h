#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MoveNotifyState.generated.h"


UCLASS()
class UMoveNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	UMoveNotifyState();
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
private:
	UPROPERTY()
	AActor* Owner;
	
	UPROPERTY()
	AActor* Target;

	UPROPERTY(EditAnywhere, meta=(ClampMin=-180, ClampMax=180))
	float AngleToTarget;

	UPROPERTY(EditAnywhere)
	bool bUpdateDirectionEveryFrame;
	
	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY()
	FVector Direction;

	void SetDirection();
};
