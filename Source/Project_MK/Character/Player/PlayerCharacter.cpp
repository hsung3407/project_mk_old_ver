// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CharacterAnimInstance.h"
#include "InventoryComponent.h"
#include "MathUtil.h"
#include "Project_MK/Data/MagicData.h"
#include "Project_MK/Magic/Magic.h"
#include "Project_MK/UserInterface/ProjectMKHUD.h"

#pragma region Setup

APlayerCharacter::APlayerCharacter(): BaseMaxMana(300), BaseMaxSpirit(500)
{
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetMesh());
	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeLocation(FVector::ZeroVector);

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> ContextObject(
		TEXT("/Game/Input/IMC_Default.IMC_Default"));
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveObject(TEXT("/Game/Input/IA_Move.IA_Move"));
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveNegateObject(
		TEXT("/Game/Input/IA_MoveNegate.IA_MoveNegate"));
	static ConstructorHelpers::FObjectFinder<UInputAction> SprintObject(TEXT("/Game/Input/IA_Sprint.IA_Sprint"));
	static ConstructorHelpers::FObjectFinder<UInputAction> ManifestObject(TEXT("/Game/Input/IA_Manifest.IA_Manifest"));
	static ConstructorHelpers::FObjectFinder<UInputAction> ChangeObject(TEXT("/Game/Input/IA_Change.IA_Change"));
	static ConstructorHelpers::FObjectFinder<UInputAction> BlinkObject(TEXT("/Game/Input/IA_Blink.IA_Blink"));
	static ConstructorHelpers::FObjectFinder<UInputAction> CameraRotateObject(
		TEXT("/Game/Input/IA_CameraRotate.IA_CameraRotate"));
	static ConstructorHelpers::FObjectFinder<UInputAction> MenuObject(TEXT("/Game/Input/IA_Menu.IA_Menu"));
	static ConstructorHelpers::FObjectFinder<UInputAction> ConcentrateObject(
		TEXT("/Game/Input/IA_Concentrate.IA_Concentrate"));

	InputMappingContext = ContextObject.Object;
	IA_Move = MoveObject.Object;
	IA_MoveNegate = MoveNegateObject.Object;
	IA_Sprint = SprintObject.Object;
	IA_Manifest = ManifestObject.Object;
	IA_Change = ChangeObject.Object;
	IA_Blink = BlinkObject.Object;
	IA_CameraRotate = CameraRotateObject.Object;
	IA_Menu = MenuObject.Object;
	IA_Concentrate = ConcentrateObject.Object;

	Speed = 20;
	MouseSensitivity = 1;

	bBlink = false;
	BlinkDistance = 500;
	BlinkCoolTime = 1.4f;

	CurrentMana = BaseMaxMana;
	CurrentSpirit = BaseMaxSpirit;

	EquippedMagic = nullptr;

	MaxConcentrateLevel = 2;

	CommandInputTime = 3;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogTemp, Warning, TEXT("Setup Player Input Component"))
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(IA_CameraRotate, ETriggerEvent::Triggered, this,
										   &APlayerCharacter::CameraRotate);
	EnhancedInputComponent->BindAction(IA_Manifest, ETriggerEvent::Triggered, this,
									   &APlayerCharacter::Manifest);
	EnhancedInputComponent->BindAction(IA_Change, ETriggerEvent::Triggered, this,
									   &APlayerCharacter::ChangeMagic);
	EnhancedInputComponent->BindAction(IA_Blink, ETriggerEvent::Triggered, this, &APlayerCharacter::Blink);

	EnhancedInputComponent->BindAction(IA_Manifest, ETriggerEvent::Triggered, this,
									   &APlayerCharacter::CommandInput, 0);
	EnhancedInputComponent->BindAction(IA_Change, ETriggerEvent::Triggered, this,
									   &APlayerCharacter::CommandInput, 1);
	EnhancedInputComponent->BindAction(IA_Blink, ETriggerEvent::Triggered, this,
									   &APlayerCharacter::CommandInput, 2);

	EnhancedInputComponent->BindAction(IA_Concentrate, ETriggerEvent::Triggered, this,
									   &APlayerCharacter::Concentrate);
	EnhancedInputComponent->BindAction(IA_Menu, ETriggerEvent::Triggered, this, &APlayerCharacter::ToggleMenu);
	MoveInput = &EnhancedInputComponent->BindActionValue(IA_Move);
	MoveNegateInput = &EnhancedInputComponent->BindActionValue(IA_MoveNegate);
	SprintInput = &EnhancedInputComponent->BindActionValue(IA_Sprint);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetLocalViewingPlayerController()->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->
	                                   AddMappingContext(InputMappingContext, 0);

	AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	//Init
	Controller->SetControlRotation(FRotator::ZeroRotator);
}

#pragma endregion Setup

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
	UpdateAnim();

	if (bConcentrate)
	{
		CommandTimer += DeltaTime;
		if (CommandTimer >= CommandInputTime)
		{
			bConcentrate = false;
			ConcentrateLevel = 0;
			Manifest();
		}
	}
}

void APlayerCharacter::UpdateAnim()
{
	AnimInstance->Speed = GetVelocity().Length();
}

#pragma region BaseMovement

void APlayerCharacter::CameraRotate(const FInputActionInstance& Instance)
{
	const FRotator CurrentRotation = SpringArm->GetRelativeRotation();
	const FVector2D InputValue = Instance.GetValue().Get<FVector2D>() * MouseSensitivity;
	SpringArm->SetRelativeRotation(FRotator(FMathf::Clamp(CurrentRotation.Pitch + InputValue.Y, -75, 75),
	                                        CurrentRotation.Yaw + InputValue.X, 0));
}

void APlayerCharacter::Move(const float DeltaTime)
{
	//Check it's possible
	if (bBlink) return;

	float SpeedMultiplier = 1;
	if (SprintInput->GetValue().Get<bool>())
		SpeedMultiplier = 2;

	FVector2D InputValue = MoveInput->GetValue().Get<FVector2D>() + MoveNegateInput->GetValue().Get<FVector2D>();
	InputValue.Normalize();
	FRotator Rotation = SpringArm->GetRelativeRotation();
	Rotation.Pitch = 0;
	const FVector MoveDirection = Rotation.Quaternion() * FVector(InputValue, 0);
	AddMovementInput(MoveDirection * (Speed * DeltaTime * SpeedMultiplier));

	//Set Rotation
	const FRotator ControlRotation = GetControlRotation();
	auto RotateRotation = ControlRotation;
	if (InputValue != FVector2D::ZeroVector) RotateRotation = MoveDirection.Rotation();
	else if (bConcentrate && ConcentrateLevel > 0) RotateRotation = SpringArm->GetRelativeRotation();
	else return;

	constexpr float RotateSpeed = 450;

	const float Angle = FMath::RadiansToDegrees(
		FMathf::ACos(FVector::DotProduct(ControlRotation.Vector(), RotateRotation.Vector())));
	const float Alpha = FMathf::Clamp(RotateSpeed * DeltaTime / Angle, 0, 1);
	Controller->SetControlRotation(FMath::Lerp(ControlRotation, RotateRotation, Alpha));
}

#pragma endregion BaseMovement

#pragma region Action

void APlayerCharacter::ChangeMagic()
{
	// Cast<AProjectMKHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->InventoryMenu->SelectNextMagicIndex();
}

void APlayerCharacter::Blink()
{
	//ToDo: Remove
	const auto Table = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTable/Item.Item"));
	auto HUD = Cast<AProjectMKHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	auto InventoryWidget = HUD->InventoryWidget;
	for (const auto RowName : Table->GetRowNames())
		InventoryComponent->AddItem(InventoryComponent->GetItemObjectByName(RowName));

	if (bConcentrate) return;
	if (bBlink) return;

	bBlink = true;
	FVector BlinkDirection;
	FVector2D InputValue = MoveInput->GetValue().Get<FVector2D>() + MoveNegateInput->GetValue().Get<FVector2D>();
	InputValue.Normalize();
	if (InputValue == FVector2D::ZeroVector) BlinkDirection = GetActorRotation().Vector();
	else
	{
		FRotator Rotation = SpringArm->GetRelativeRotation();
		Rotation.Pitch = 0;
		BlinkDirection = Rotation.Quaternion() * FVector(InputValue, 0);
	}

	//Move
	AddActorWorldOffset(BlinkDirection * BlinkDistance);

	//Blink Cooldown
	FTimerHandle Handle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this] { bBlink = false; });
	GetWorldTimerManager().SetTimer(Handle, TimerDelegate, BlinkCoolTime, false);
}

void APlayerCharacter::Concentrate()
{
	if (bBlink) return;
	if (bConcentrate)
	{
		bConcentrate = false;
		Manifest();
		UE_LOG(LogTemp, Warning, TEXT("Use"))
		CommandList.Reset();
		return;
	}

	CommandList.Reset();
	bConcentrate = true;
	ConcentrateLevel = 0;
	CommandTimer = 0;

	CommandList.Reset();

	UE_LOG(LogTemp, Warning, TEXT("Concentrate"))
}

void APlayerCharacter::CommandInput(const int Index)
{
	if (!bConcentrate) return;
	CommandList.Emplace(InventoryComponent->GetRuneList()[Index]);
}

void APlayerCharacter::Manifest()
{
	//Check it's possible
	if (bBlink) return;
	if (bConcentrate) return;

	//ToDo: 인챈트된 마법이 있을 경우의 로직
	if (EquippedMagic)
	{
	}

	const auto FindResult = AMagic::MagicFindByRunes(CommandList);

	//TODO: 실패할 경우의 처리
	if (!FindResult.Row) return;

	FMagicInfoContainer Container;
	Container.OrdererActor = this;
	Container.TargetActor = nullptr;
	Container.SpawnLocation = GetActorLocation();
	Container.SpawnRotator = SpringArm->GetRelativeRotation();

	AMagic::SpawnMagic(FindResult.Row, &Container);
}

#pragma endregion Action

void APlayerCharacter::ToggleMenu()
{
	auto HUD = Cast<AProjectMKHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->ToggleMenu();
}
