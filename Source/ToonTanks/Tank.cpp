// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ATank::ATank()
{
	//Spring Arm component
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm Component"));
	SpringArmComp->SetupAttachment(RootComponent);

	//Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Cameara Component"));
	CameraComp->SetupAttachment(SpringArmComp);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
	TankPlayerController = Cast<APlayerController>(GetController());

	
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

//이동은 앞, 뒤 축에 대한 이동 + 마우스 회전으로 처리함
void ATank::Move(float Value) 
{
	//영벡터와 시간값을 가져옴(게임 프레임 영향을 받지 않는 움직임을 위해)
	FVector DeltaLocation = FVector::ZeroVector;
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	// X = Value * DeltaTime * Speed

	//영벡터의 X값(앞, 뒤 이동의 축)에 입력값(Value) * 이동속도 * 시간을 곱함
	//여기서 Value는 w, s키를 누르는 input
	DeltaLocation.X = Value * Speed * DeltaTime;

	//플레이어 오브젝트 기준 x축 = 플레이어가 보는 정면
	//플레이어 액터에 로컬 오프셋으로 앞서 계산한 x값을 더하면, 그만큼 앞, 뒤로 이동한다는 뜻이 됨
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
	//이동과 유사한 방식으로 사용. 차이점은 Vector 대신 Rotator를 사용함
	FRotator DeltaRotation = FRotator::ZeroRotator;
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	//좌, 우 회전값은 Z축 기준 회전 = Yaw를 수정해야 한다.
	//여기서 Value는 a, s키를 누르는 input
	DeltaRotation.Yaw = Value * TurnRate * DeltaTime;

	AddActorLocalRotation(DeltaRotation, true);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility,
			false,
			HitResult);

		ABasePawn::RotateTurret(HitResult.ImpactPoint);
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}