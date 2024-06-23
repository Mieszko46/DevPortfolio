#include "Grabber.h"

#define OUT
#define NOMINE "IA_Pick_No_Mine"
#define MINE "IA_Pick_Mine"

// Sets default values
UGrabber::UGrabber():
	FocusedActor(nullptr),
	bDebugFlag(true), 
	DebugIndex(-1),
	NearMines(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	AActor* ActorHit = HitResult.GetActor();
	
	if (ActorHit) {
		HighlightObject(ActorHit);
	}
	// It makes ungightlight object if we are not looking at it
	//else {
	//	if (FocusedActor)
	//		Cast<ACubePackage>(FocusedActor)->SetIsActorFocued(false, false);
	//		FocusedActor = nullptr;
	//}

}

void UGrabber::HandlePickup(FString mouseButton)
{
	if (FocusedActor != nullptr)
	{
		ACubePackage* PickedPackage = Cast<ACubePackage>(FocusedActor);
		AActorSpawner* Spawner = Cast<AActorSpawner>(PickedPackage->GetPackageSpawner());
		uint32 PackageIndex = PickedPackage->GetPackageIndex();

		if (Spawner->CheckIfPackageCanBePicked(PackageIndex))
		{
			if (mouseButton == NOMINE && !(PickedPackage->IsItMine()))
			{
				FocusedActor = nullptr;
				Spawner->RemoveActorFromArray(PackageIndex);
				UGameplayStatics::PlaySound2D(GetWorld(), PickupSound);
			}
			else if (mouseButton == MINE && PickedPackage->IsItMine())
			{
				PickedPackage->Defuse();
				FocusedActor = nullptr;
				Spawner->RemoveActorFromArray(PackageIndex);
				Spawner->AssignNumbersOfNearMines();
				UGameplayStatics::PlaySound2D(GetWorld(), DefuseSound);
			}
			else 
			{
				PickedPackage->Explode();
				GameOver(mouseButton);
			}

			// Win condition
			if (Spawner->CheckIfAllPackagesPicked())
			{
				Win();
			}
		}
	}
}

void UGrabber::UpdateScannerDisplayValue(int packageMines)
{
	NearMines = packageMines;
	FOutputDeviceNull OutputDeviceNull;
	GetOwner()->CallFunctionByNameWithArguments(TEXT("UpdateMineScannerValue"), OutputDeviceNull, nullptr, true);
}

void UGrabber::GameOver(FString mouseButton)
{ 
	GameOverText = mouseButton != MINE ? TEXT("You picked a bomb!") : TEXT("It was not a bomb!");
	FOutputDeviceNull OutputDeviceNull;
	GetOwner()->CallFunctionByNameWithArguments(TEXT("GameLost"), OutputDeviceNull, nullptr, true);
}

void UGrabber::Win()
{
	FOutputDeviceNull OutputDeviceNull;
	GetOwner()->CallFunctionByNameWithArguments(TEXT("GameWon"), OutputDeviceNull, nullptr, true);
}

FVector UGrabber::GetPlayerLocation() const
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);

	return PlayerViewLocation;
}

FRotator UGrabber::GetPlayerRotation() const
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewLocation, OUT PlayerViewRotation);

	return PlayerViewRotation;
}

FVector UGrabber::GetPlayerReach() const
{
	FVector PlayerViewLocation = GetPlayerLocation();
	FRotator PlayerViewRotation = GetPlayerRotation();

	FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * m_Reach;

	//UE_LOG(LogTemp, Warning, TEXT("Actor location: %s"), *LineTraceEnd.ToString());

	//DrawDebugLine(
	//	GetWorld(),
	//	PlayerViewLocation,
	//	LineTraceEnd,
	//	FColor(255, 0, 0),
	//	false,
	//	0.f,
	//	0,
	//	2.0f
	//);

	return LineTraceEnd;
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerLocation(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(),
		TraceParams
	);

	return Hit;
}

void UGrabber::HighlightObject(AActor* ActorHit)
{
	if (ActorHit->IsA<ACubePackage>()) 
	{
		if (FocusedActor != ActorHit) 
		{
			// unset highlight of passed object
			if (FocusedActor != nullptr) 
			{
				Cast<ACubePackage>(FocusedActor)->SetIsActorFocued(false, false);
			}

			FocusedActor = ActorHit;
			ACubePackage* HittedPackage = Cast<ACubePackage>(FocusedActor);

			AActorSpawner* Spawner = Cast<AActorSpawner>(HittedPackage->GetPackageSpawner());
			if (Spawner->CheckIfPackageCanBePicked(HittedPackage->GetPackageIndex()))
			{
				HittedPackage->SetIsActorFocued(true, true);
			}
			else {
				HittedPackage->SetIsActorFocued(true, false);
			}

			UpdateScannerDisplayValue(HittedPackage->GetNumberOfNearMines());

			// DEBUG
			UE_LOG(LogTemp, Warning, TEXT("Hited package: %s"), *(HittedPackage->GetName()));
			UE_LOG(LogTemp, Warning, TEXT("Hited package index: %d"), HittedPackage->GetPackageIndex());
			UE_LOG(LogTemp, Warning, TEXT("Hited package near mines: %d"), HittedPackage->GetNumberOfNearMines());
			UE_LOG(
				LogTemp, Warning, TEXT("Does package can be picked: %s"),
				(Spawner->CheckIfPackageCanBePicked(HittedPackage->GetPackageIndex()) ? TEXT("TRUE") : TEXT("false"))
			);
			UE_LOG(
				LogTemp, Warning, TEXT("Is it last in Z: %s"),
				((HittedPackage->GetIsOnLastPositionInZ_Axis()) ? TEXT("TRUE") : TEXT("false"))
			);
			UE_LOG(
				LogTemp, Warning, TEXT("Does package contain mine: %s"),
				((HittedPackage->IsItMine()) ? TEXT("TRUE") : TEXT("false"))
			);
		}
	}
}
