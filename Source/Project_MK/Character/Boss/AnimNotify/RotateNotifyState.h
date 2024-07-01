#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RotateNotifyState.generated.h"

UCLASS()
class URotateNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	URotateNotifyState();
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY()
	AActor* Owner;
		
	UPROPERTY()
	AActor* Target;

	UPROPERTY(EditAnywhere)
	bool bUpdateTargetRotationEveryFrame;

	UPROPERTY(EditAnywhere)
	bool bRotateAccordingToSpeed;

	//없어도 아래 설명대로 자동으로 적용됨
	//아래에서 설정된 각도를 기반으로 +와 -중 현재 회전과 더 가까운 각도로 회전하게 만듬
	// UPROPERTY(EditAnywhere)
	// bool bAngleSignIgnore;

	UPROPERTY(EditAnywhere, meta=(ClampMin=-180, ClampMax=180))
	float AngleToTarget;

	UPROPERTY(EditAnywhere)
	float MinSpeed;
	
	UPROPERTY(EditAnywhere)
	float MaxSpeed;
	
	UPROPERTY(VisibleAnywhere)
	float CurrentSpeed;

	UPROPERTY(EditAnywhere)
	bool bIgnoreZAxis;

	UPROPERTY()
	float TotalPlayTime;

	UPROPERTY()
	float Timer;
	
	UPROPERTY()
	FRotator OriginRotation;

	UPROPERTY()
	FRotator TargetRotation;

	void SetRotation();
};
