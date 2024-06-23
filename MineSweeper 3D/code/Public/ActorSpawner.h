// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubePackage.h"
#include "SpawnerTarget.h"
#include <Kismet/GameplayStatics.h>
#include "ActorSpawner.generated.h"

UCLASS()
class MINESWEEPER_API AActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Setup();
	void PrintArray();
	bool CheckIfPackageCanBePicked(int32 index) const;
	void RemoveActorFromArray(uint32 index);
	void AssignNumbersOfNearMines();
	bool CheckIfAllPackagesPicked() const;

	UFUNCTION(BlueprintCallable)
		void SetupParams(int packages, int mines, float complexity);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	#pragma region Board methods
	void CreateBoardOnSpawnPoint();

	void RandomizeBoardDimensions();

	void PlaceThePackages();
	#pragma endregion


	#pragma region Packages methods
	// get package from an array of all packages 
	// @param x - package position in x-axis
	// @param y - package position in y-axis
	// @param z - package position in z-axis
	AActor* GetActorByIndex(int32 index) const;

	void CalculateCoordinatesFromIndex(uint32 index, uint32& out_x, uint32& out_y, uint32& out_z);

	uint32 CalculateIndexFromCoordinates(uint32 x, uint32 y, uint32 z) const;

	void SetTotalNumberOfPackages(uint32 packages);
	#pragma endregion


	#pragma region Mines methods
	void SetTotalNumberOfMines(uint32 mines);

	bool IsPackageAMine(uint32 x, uint32 y, uint32 z);

	//Checks if near packages have mines and return total number of them
	uint32 CountNearMines(uint32 index);

	void RandomizeMinesPlacement();
	#pragma endregion


	#pragma region ATTRIBUTES
	// Actor that will be spawned
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AActor> ActorToSpawn;

	// Position of selected spawn point
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSoftObjectPtr<ASpawnerTarget> SpawnerTargetPoint;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		uint32 PackageSize = 100;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "1.0", ClampMax = "100.0", UIMin = "1.0", UIMax = "100.0"))
		uint32 TotalNumberOfPackages = 10;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "1.0", ClampMax = "100.0", UIMin = "1.0", UIMax = "100.0"))
		uint32 TotalNumberOfMines = 5;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (ClampMin = "1.0", ClampMax = "3.0", UIMin = "1.0", UIMax = "3.0"))
		float FactorOfBoardComplexity = 1.5f;

	TArray<AActor*> AllPackages;
	uint32 ArraySpace;

	// Board dimension
	uint32 X_Width, Y_Height, Z_Deep;

	uint32 PickedPackages;
	#pragma endregion

};
