#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Misc/OutputDeviceNull.h"
#include "Kismet/GameplayStatics.h"
#include "CubePackage.h"
#include "ActorSpawner.h"
#include "Grabber.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINESWEEPER_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
		FString GameOverText;

	UPROPERTY(EditAnywhere, Category = "Game")
		TObjectPtr<USoundBase> PickupSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Game")
		TObjectPtr<USoundBase> DefuseSound = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mines")
		int NearMines;

	UFUNCTION(BlueprintCallable)
		void HandlePickup(FString mouseButton);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	// Checking for Physics Handle Component. Protection from nullptr
	void UpdateScannerDisplayValue(int packageMines);
	void GameOver(FString text);
	void Win();

	// Return the Player position in world
	FVector GetPlayerLocation() const;

	// Return the Player rotation in world
	FRotator GetPlayerRotation() const;

	// Return the Line Trace End
	FVector GetPlayerReach() const;

	// Return the first Actor within reach with physics body
	FHitResult GetFirstPhysicsBodyInReach() const;

	void HighlightObject(AActor* HitActor);

	#pragma region Attributes
	AActor* FocusedActor;

	// sets debug line length
	float m_Reach = 200.f;

	#pragma region Debugging
	bool bDebugFlag;
	uint32 DebugIndex;
	#pragma endregion
};
