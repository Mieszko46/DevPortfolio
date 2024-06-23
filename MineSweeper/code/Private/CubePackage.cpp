// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePackage.h"

// Sets default values
ACubePackage::ACubePackage():
	bIsMine(false),
	bWasPicked(false),
	isOnLastPositionIn_Z(false),
	IndexInArray(-1),
	NumberOfNearMines(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);
}

// Called when the game starts or when spawned
void ACubePackage::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACubePackage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubePackage::SetPackageIndex(uint32 index)
{
	IndexInArray = index;
}

uint32 ACubePackage::GetPackageIndex() const
{
	return IndexInArray;
}

void ACubePackage::SetPackageSpawner(AActor* Spawner)
{
	PackageSpawner = Spawner;
}

AActor* ACubePackage::GetPackageSpawner() const
{
	return PackageSpawner;
}

void ACubePackage::SetIsMineToTrue()
{
	bIsMine = true;
}

bool ACubePackage::IsItMine() const
{
	return bIsMine;
}

void ACubePackage::SetNumberOfNearMines(uint32 nearMines)
{
	NumberOfNearMines = nearMines;
}

uint32 ACubePackage::GetNumberOfNearMines() const
{
	return NumberOfNearMines;
}

void ACubePackage::SetIsActorFocued(bool isFocused, bool canBePicked)
{
	IsActorFocued = isFocused;
	if (canBePicked) 
	{
		BaseMesh->SetRenderCustomDepth(true);
		BaseMesh->SetCustomDepthStencilValue(2);
	}
	else if (isFocused && !canBePicked)
	{
		BaseMesh->SetRenderCustomDepth(true);
		BaseMesh->SetCustomDepthStencilValue(0);
	}
	else 
	{
		BaseMesh->SetRenderCustomDepth(false);
	}
}

bool ACubePackage::GetIsActorFocued() const
{
	return IsActorFocued;
}

void ACubePackage::SetIsOnLastPositionInZ_Axis(bool isLast)
{
	isOnLastPositionIn_Z = isLast;
}

bool ACubePackage::GetIsOnLastPositionInZ_Axis() const
{
	return isOnLastPositionIn_Z;
}

void ACubePackage::Explode() const
{
	if (ExplodeParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeParticles, GetActorLocation(), GetActorRotation());
	}
	if (ExplodeSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());
	}
}

void ACubePackage::Defuse()
{
	FOutputDeviceNull OutputDeviceNull;
	CallFunctionByNameWithArguments(TEXT("DefusePackage"), OutputDeviceNull, nullptr, true);
}
