#include "RotateNotifyState.h"

#include "MathUtil.h"
#include "GameFramework/Character.h"


URotateNotifyState::URotateNotifyState(): Owner(nullptr), Target(nullptr), AngleToTarget(0), MinSpeed(0), MaxSpeed(100),
                                          CurrentSpeed(0),
                                          bIgnoreZAxis(true)
{
	
}

void URotateNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if(MeshComp)
	{
		if(const auto OwnerActor = MeshComp->GetOwner())
		{
			Owner = OwnerActor;
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

	TotalPlayTime = TotalDuration;
	UE_LOG(LogTemp, Warning, TEXT("%f"), TotalPlayTime)
	Timer = 0;
	SetRotation();
}

void URotateNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if(!Owner) return;

	if(bUpdateTargetRotationEveryFrame) SetRotation();

	if(bRotateAccordingToSpeed)
	{
		const auto Angle = FMathf::Abs(FMath::RadiansToDegrees(FMathf::ACos(FVector::DotProduct(OriginRotation.Vector(), TargetRotation.Vector()))));
		const auto Alpha = FMathf::Clamp(CurrentSpeed * FrameDeltaTime / Angle , 0, 1);
		Owner->SetActorRotation(FMath::Lerp(Owner->GetActorRotation(), TargetRotation, Alpha));
	}
	else
	{
		Timer += FrameDeltaTime;
		Owner->SetActorRotation(FMath::Lerp(OriginRotation, TargetRotation, Timer / TotalPlayTime));
	}
}

void URotateNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
}

void URotateNotifyState::SetRotation()
{
	OriginRotation = Owner->GetActorRotation();
	auto TargetDirection = FVector::ForwardVector;
	if(Target)
	{
		TargetDirection = Target->GetActorLocation() - Owner->GetActorLocation();
		if(bIgnoreZAxis)
			TargetDirection.Z = 0;
		TargetDirection.Normalize();
	}
	
	// if(bAngleSignIgnore)
	// {
	// 	auto Direction1 = TargetDirection.RotateAngleAxis(AngleToTarget, FVector(0, 0, 1));
	//
	// 	auto Direction2 = TargetDirection.RotateAngleAxis(-AngleToTarget, FVector(0, 0, 1));
	//
	// 	auto Angle1 =  FMathf::ACos(FVector::DotProduct(OriginRotation.Vector(), Direction1));
	// 	auto Angle2 =  FMathf::ACos(FVector::DotProduct(OriginRotation.Vector(), Direction2));
	// 	if(Angle1 < Angle2)
	// 		TargetDirection = Direction1;
	// 	else
	// 		TargetDirection = Direction2;
	// }
	// else TargetDirection = TargetDirection.RotateAngleAxis(AngleToTarget, FVector(0, 0, 1));
	TargetDirection = TargetDirection.RotateAngleAxis(AngleToTarget, FVector(0, 0, 1));

	const auto NeedRotateAngle = FMathf::ACos(FVector::DotProduct(OriginRotation.Vector(), TargetDirection));

	CurrentSpeed = FMathf::Clamp(NeedRotateAngle / TotalPlayTime, MinSpeed, MaxSpeed);
	TargetRotation = TargetDirection.Rotation();
}
