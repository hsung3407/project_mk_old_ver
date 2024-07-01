#include "MoveNotifyState.h"
#include "GameFramework/Character.h"

UMoveNotifyState::UMoveNotifyState()
{
}

void UMoveNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if(MeshComp)
	{
		if(const auto SelfActor = MeshComp->GetOwner())
		{
			Owner = SelfActor;
			UE_LOG(LogTemp, Warning, TEXT("Self"))
		}
		else return;
	}
	
	if(const auto World = Owner->GetWorld())
	{
		if(const auto PlayerController = World->GetFirstPlayerController())
		{
			if(const auto PlayerCharacter = PlayerController->GetCharacter())
			{
				UE_LOG(LogTemp, Warning, TEXT("Target"))
				Target = Cast<AActor>(PlayerCharacter);
			}
		}
	}

	SetDirection();
}

void UMoveNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if(!Owner) return;
	if(bUpdateDirectionEveryFrame) SetDirection();
	Owner->AddActorWorldOffset(Direction * Speed * FrameDeltaTime);
}

void UMoveNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UMoveNotifyState::SetDirection()
{
	FVector Origin = Owner->GetActorForwardVector();
	if(Target)
	{
		Origin = Target->GetActorLocation() - Owner->GetActorLocation();
		Origin.Z = 0;
		Origin.Normalize();
	}

	Direction = Origin.RotateAngleAxis(AngleToTarget, FVector::UpVector);
}
