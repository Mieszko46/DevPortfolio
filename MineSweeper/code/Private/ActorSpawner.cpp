// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorSpawner.h"

//TODO: przed RandomizeBoardDimensions() trzeba podaæ od gracza liczbê paczek

AActorSpawner::AActorSpawner() :
	X_Width(1),
	Y_Height(1),
	Z_Deep(1),
	ArraySpace(1),
	PickedPackages(0)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AActorSpawner::Setup()
{
	X_Width = 1;
	Y_Height = 1;
	Z_Deep = 1;
	ArraySpace = 1;
	PickedPackages = 0;

	UE_LOG(LogTemp, Warning, TEXT("Packages: %d"), TotalNumberOfPackages);
	UE_LOG(LogTemp, Warning, TEXT("Mines: %d"), TotalNumberOfMines);
	UE_LOG(LogTemp, Warning, TEXT("Complexity: %f"), FactorOfBoardComplexity);

	CreateBoardOnSpawnPoint();
	RandomizeMinesPlacement();
	AssignNumbersOfNearMines();
}

void AActorSpawner::SetupParams(int packages, int mines, float complexity)
{
	TotalNumberOfPackages = static_cast<uint32>(packages);
	TotalNumberOfMines = static_cast<uint32>(mines);
	FactorOfBoardComplexity = complexity;
}

void AActorSpawner::BeginPlay()
{
	Super::BeginPlay();
	Setup();
}

void AActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AActorSpawner::CheckIfPackageCanBePicked(int32 index) const
{
	// We have to check if on top of this package there is no more packages
	// next package in top has Z axis +1 so in array it is actor with 
	// current index +1 
	// Last element in deep for example when we have board (1,2,2) last 
	// element will be (0,0,1) we must check if Z is last because then it will
	// check next index which might be package on different dimension

	if (index == -1 || index + 1 > AllPackages.Num())
		return false;

	// I gave them separate, because if it will be last element in the array
	// it will crash in the next IF statement
	if (Cast<ACubePackage>(AllPackages[index])->GetIsOnLastPositionInZ_Axis())
		return true;

	if (AllPackages[index + 1] == nullptr) 
		return true;

	return false;
}

bool AActorSpawner::CheckIfAllPackagesPicked() const
{
	return (PickedPackages == TotalNumberOfPackages ? true : false);
}

void AActorSpawner::CreateBoardOnSpawnPoint()
{
	RandomizeBoardDimensions();

	// init board with nullptr
	//AllPackages.Init(AActor(), X_Width * Y_Height * Z_Deep);
	UE_LOG(LogTemp, Warning, TEXT("Array size: %d"), ArraySpace);
	AllPackages.Reserve(ArraySpace);
	AllPackages.SetNumZeroed(ArraySpace);

	PlaceThePackages();
}

void AActorSpawner::PrintArray()
{
	for (int32 i = 0; i < AllPackages.Num(); i++)
	{
		auto Package = AllPackages[i];
		if (Package != nullptr)
			UE_LOG(LogTemp, Warning, TEXT("array elem on pos %d is: %d"), i, Package);
		
	}
}

void AActorSpawner::RandomizeBoardDimensions()
{
	while (true)
	{
		uint32 dimensionNumber = FMath::RandHelper(3);

		if (dimensionNumber == 0)
			X_Width += 1;
		else if (dimensionNumber == 1) 
			Y_Height += 1;
		else
			Z_Deep += 1;

		ArraySpace = X_Width * Y_Height * Z_Deep;

		if (ArraySpace >= TotalNumberOfPackages * FactorOfBoardComplexity)
			break;
	}
}

void AActorSpawner::PlaceThePackages()
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, AllPackages[1].GetName());

	UE_LOG(LogTemp, Warning, TEXT("BOARD params: %d, %d, %d"), X_Width, Y_Height, Z_Deep);

	FActorSpawnParameters SpawnParams;

	FTransform SpawnerTransform = SpawnerTargetPoint
		? SpawnerTargetPoint->GetActorTransform()
		: FTransform::Identity;

	uint32 CurrentPlacedPackages = 0;
	while (CurrentPlacedPackages < TotalNumberOfPackages)
	{
		FActorSpawnParameters temp = SpawnParams;

		FTransform InitTransform = SpawnerTransform;

		// We take random position in 2d board and then go in deep 
		uint32 X_RandomPosition = FMath::RandHelper(X_Width);
		uint32 Y_RandomPosition = FMath::RandHelper(Y_Height);

		for (uint32 Z_Position = 0; Z_Position < Z_Deep; Z_Position++)
		{
			int32 index = CalculateIndexFromCoordinates(X_RandomPosition, Y_RandomPosition, Z_Position);

			//check if position is empty
			if (AllPackages[index] == nullptr)
			{
				// Spawn object
				FVector offset = FVector(PackageSize * X_RandomPosition, PackageSize * Y_RandomPosition, PackageSize * Z_Position);
				FRotator randomRotation = FRotator(0.f, 90.f * FMath::RandHelper(4) , 0.f);

				InitTransform.AddToTranslation(offset);
				InitTransform.SetRotation(FQuat(randomRotation));
				AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, InitTransform, temp);

				// Insert package to array
				AllPackages[index] = SpawnedActor;

				CurrentPlacedPackages += 1;

				//Set package index
				ACubePackage* Package = Cast<ACubePackage>(SpawnedActor);
				Package->SetPackageIndex(index);
				Package->SetPackageSpawner(this);

				if (Z_Position + 1 == Z_Deep)
				{
					Package->SetIsOnLastPositionInZ_Axis(true);
				}

				UE_LOG(LogTemp, Warning, TEXT("Actor: %s | Package index: %d | coords: x=%d y=%d z=%d"),
					*(SpawnedActor->GetName()), 
					Package->GetPackageIndex(),
					X_RandomPosition,
					Y_RandomPosition,
					Z_Position
				);

				// To break from the loop
				Z_Position = Z_Deep;
			}
		}
	}
}

bool AActorSpawner::IsPackageAMine(uint32 x, uint32 y, uint32 z) 
{
	if (AllPackages[CalculateIndexFromCoordinates(x, y, z)] != nullptr)
	{
		ACubePackage* Package = Cast<ACubePackage>(AllPackages[CalculateIndexFromCoordinates(x, y, z)]);
		if (Package->IsItMine())
		{
			UE_LOG(LogTemp, Warning, TEXT("index: %d, coords %d, %d ,%d"), CalculateIndexFromCoordinates(x, y, z), x, y, z);
			return true;
		}
	}
	return false;
}

uint32 AActorSpawner::CountNearMines(uint32 index)
{
	uint32 NumberOfMines = 0;
	uint32 x, y, z;
	CalculateCoordinatesFromIndex(index, x, y, z);

	if (x > 0) NumberOfMines += IsPackageAMine(x - 1, y, z);
	if (x < X_Width - 1) NumberOfMines += IsPackageAMine(x + 1, y, z);

	if (y > 0) NumberOfMines += IsPackageAMine(x, y - 1, z);
	if (y < Y_Height - 1) NumberOfMines += IsPackageAMine(x, y + 1, z);

	if (z > 0) NumberOfMines += IsPackageAMine(x, y, z - 1);
	if (z < Z_Deep - 1) NumberOfMines += IsPackageAMine(x, y, z + 1);

	return NumberOfMines;
}

void AActorSpawner::RandomizeMinesPlacement()
{
	if (TotalNumberOfMines > TotalNumberOfPackages)
		TotalNumberOfMines = TotalNumberOfPackages - TotalNumberOfPackages * 0.1;

	uint32 counter = TotalNumberOfMines;
	while (counter > 0)
	{
		uint32 packageIndex = FMath::RandHelper(ArraySpace);
		if (AllPackages[packageIndex] != nullptr)
		{
			ACubePackage* Package = Cast<ACubePackage>(AllPackages[packageIndex]);
			if (Package->IsItMine() == false) 
			{
				Package->SetIsMineToTrue();
				UE_LOG(LogTemp, Warning, TEXT("Mine set to index: %d"), Package->GetPackageIndex());
				--counter;
			}
		}
	}
}

void AActorSpawner::AssignNumbersOfNearMines()
{
	for (int32 packageIndex = 0; packageIndex < AllPackages.Num(); packageIndex++)
	{
		if (AllPackages[packageIndex] != nullptr)
		{
			ACubePackage* Package = Cast<ACubePackage>(AllPackages[packageIndex]);
			Package->SetNumberOfNearMines(CountNearMines(packageIndex));
			UE_LOG(LogTemp, Warning, TEXT("Index: %d, mines: %d"), Package->GetPackageIndex(),  Package->GetNumberOfNearMines());
		}
	}
}

AActor* AActorSpawner::GetActorByIndex(int32 index) const
{
	auto Package = AllPackages[index];
	return Package;
}

void AActorSpawner::RemoveActorFromArray(uint32 index)
{
	if (index >= 0 && index < ArraySpace)
	{
		AllPackages[index]->Destroy();
		AllPackages[index] = nullptr;
		PickedPackages += 1;
	}
}

void AActorSpawner::CalculateCoordinatesFromIndex(uint32 index, uint32& out_x, uint32& out_y, uint32& out_z)
{
	out_z = index % Z_Deep;
	index /= Z_Deep;
	out_y = index % Y_Height;
	index /= Y_Height;
	out_x = index;
}

uint32 AActorSpawner::CalculateIndexFromCoordinates(uint32 x, uint32 y, uint32 z) const
{
	return x * Y_Height * Z_Deep + y * Z_Deep + z;
}

void AActorSpawner::SetTotalNumberOfPackages(uint32 packages)
{
	TotalNumberOfPackages = packages;
}

void AActorSpawner::SetTotalNumberOfMines(uint32 mines)
{
	TotalNumberOfMines = mines;
}
