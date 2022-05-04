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

//�̵��� ��, �� �࿡ ���� �̵� + ���콺 ȸ������ ó����
void ATank::Move(float Value) 
{
	//�����Ϳ� �ð����� ������(���� ������ ������ ���� �ʴ� �������� ����)
	FVector DeltaLocation = FVector::ZeroVector;
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	// X = Value * DeltaTime * Speed

	//�������� X��(��, �� �̵��� ��)�� �Է°�(Value) * �̵��ӵ� * �ð��� ����
	//���⼭ Value�� w, sŰ�� ������ input
	DeltaLocation.X = Value * Speed * DeltaTime;

	//�÷��̾� ������Ʈ ���� x�� = �÷��̾ ���� ����
	//�÷��̾� ���Ϳ� ���� ���������� �ռ� ����� x���� ���ϸ�, �׸�ŭ ��, �ڷ� �̵��Ѵٴ� ���� ��
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
	//�̵��� ������ ������� ���. �������� Vector ��� Rotator�� �����
	FRotator DeltaRotation = FRotator::ZeroRotator;
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	//��, �� ȸ������ Z�� ���� ȸ�� = Yaw�� �����ؾ� �Ѵ�.
	//���⼭ Value�� a, sŰ�� ������ input
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