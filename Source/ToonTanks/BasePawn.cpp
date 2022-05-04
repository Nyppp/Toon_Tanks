// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Camera/CameraShakeBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ĸ�� �ݶ��̴��� ��Ʈ ������Ʈ�� ���� -> �浹 ������ �����ϰ� �� + ������Ʈ�� ��Ʈ ������Ʈ
	CapsulComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsulComp;

	//����ƽ ������Ʈ�� ĸ�� �ݶ��̴�(��Ʈ������Ʈ�� ������)�� �ڽ� ������Ʈ�� ����
	//��ũ�� ��ü�� �Ǵ� �޽� ������Ʈ�� �ڸ���
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsulComp);

	//��ũ�� ���밡 �Ǵ� �޽��� ��ü�� �ڽ� ������Ʈ�� ����
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	//���뿡�� ���� ���� ������ �� ������Ʈ�� ���� ��, ���� ������Ʈ�� �ڽ����� ����
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	
	TurretMesh->SetWorldRotation(LookAtRotation);
}

void ABasePawn::Fire()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		Location,
		Rotation
		);

	Projectile->SetOwner(this);
}

void ABasePawn::HandleDestruction()
{
	//TODO: visual and sound effects
	if (DeathParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation(), GetActorRotation());
	}

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	if (DeathCameraClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(DeathCameraClass);
	}
}