// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AActorSpawner;
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Misc/OutputDeviceNull.h"
#include "CubePackage.generated.h"

UCLASS()
class MINESWEEPER_API ACubePackage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubePackage();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPackageIndex(uint32 index);
	uint32 GetPackageIndex() const;

	void SetPackageSpawner(AActor* Spawner);
	AActor* GetPackageSpawner() const;

	#pragma region Mines methods
	void SetIsMineToTrue();
	bool IsItMine() const;
	void SetNumberOfNearMines(uint32 nearMines);
	uint32 GetNumberOfNearMines() const;
	#pragma endregion

	void SetIsActorFocued(bool isFocused, bool canBePicked);
	bool GetIsActorFocued() const;

	void SetIsOnLastPositionInZ_Axis(bool isLast);
	bool GetIsOnLastPositionInZ_Axis() const;

	void Explode() const;
	void Defuse();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UCapsuleComponent> CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(EditAnywhere, Category = "Explosion")
		TObjectPtr<UParticleSystem> ExplodeParticles = nullptr;

	UPROPERTY(EditAnywhere, Category = "Explosion")
		TObjectPtr<USoundBase> ExplodeSound = nullptr;

	AActor* PackageSpawner;

	// Does ACTOR HAVE a MINE
	bool bIsMine;

	// Has ACTOR been already PICKED
	bool bWasPicked;

	// Does player look on this actor
	bool IsActorFocued;

	// Index in array
	uint32 IndexInArray;

	// Is it last element in Z axis but it means in max Z value because 
	// if it is then we don't need to search for neighbour
	bool isOnLastPositionIn_Z;

	// How many MINES are NEAR
	uint32 NumberOfNearMines;
};
