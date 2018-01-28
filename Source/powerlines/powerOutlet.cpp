// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerOutlet.h"
#include "Components/BoxComponent.h"
#include "powerlinesCharacter.h"
#include "PaperFlipbookComponent.h"
#include "EngineUtils.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"



// Sets default values
APowerOutlet::APowerOutlet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set component hirerarchy
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Outlet Animation"));
	FlipbookComponent->SetupAttachment(RootComponent);
	RootComponent = FlipbookComponent;

	InteractionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Interaction Box"));
	InteractionComponent->SetupAttachment(RootComponent);

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System"));
	ParticleComponent->SetupAttachment(RootComponent);

	// Set up actor state
	InitialUses = 3;
	Uses = InitialUses;
	ConnectedPowerOutlet = NULL;
	ConnectionNumber = -1;
	RequieredLevel = 1;

}

// Called when the game starts or when spawned
void APowerOutlet::BeginPlay()
{
	// Find the other outlet we are connected to
	for (TActorIterator<APowerOutlet> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (((*ActorItr)->ConnectionNumber == this->ConnectionNumber) &&
			(*ActorItr != this))
		{
			ConnectedPowerOutlet = *ActorItr;
			break;
		}

		UE_LOG(LogTemp, Warning, TEXT("Did not find an object that matches our connection number"));
	}

	ParticleComponent->SetTemplate(TeleportationEffect);

	Super::BeginPlay();	
}

bool APowerOutlet::Use(int level)
{
	// Check that the player satisfies all conditions to use the outlet
	if (!ConnectedPowerOutlet)
		return false;
	if (Uses <= 0)
		return false;
	if (level < RequieredLevel)
		return false;

	Uses--;
	ConnectedPowerOutlet->Uses--;

	ParticleComponent->Activate();
	ConnectedPowerOutlet->ParticleComponent->Activate();

	return true;
}

// Called every frame
void APowerOutlet::Tick(float DeltaTime)
{
	// Get all overlapping actors
	TArray<AActor*> InteractionActors;
	InteractionComponent->GetOverlappingActors(InteractionActors);
	// Specific Item handling
	for (int32 InteractionIndex = 0; InteractionIndex < InteractionActors.Num(); ++InteractionIndex)
	{
		ApowerlinesCharacter* const player = Cast<ApowerlinesCharacter>(InteractionActors[InteractionIndex]);
		if (player && !player->IsPendingKill() && Uses > 0)
		{
			ConnectedPowerOutlet->ParticleComponent->Activate();
		}
	}
	Super::Tick(DeltaTime); 
	//To DO: Clean up HAcky loop
	//To Do: Change particles/ change flipbook


}

