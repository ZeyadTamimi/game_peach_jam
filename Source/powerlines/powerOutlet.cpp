// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerOutlet.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "EngineUtils.h"



// Sets default values
APowerOutlet::APowerOutlet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set component hirerarchy
	UBoxComponent* PowerOutletRootComponent;
	PowerOutletRootComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = PowerOutletRootComponent;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Outlet Animation"));
	FlipbookComponent->SetupAttachment(RootComponent);

	// Set up actor state
	InitialUses = 3;
	Uses = InitialUses;
	ConnectedPowerOutlet = NULL;

}

// Called when the game starts or when spawned
void APowerOutlet::BeginPlay()
{

	// Find the other outlet we are connected to
	for (TActorIterator<APowerOutlet> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (*ActorItr != this)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found an object"));
			ConnectedPowerOutlet = *ActorItr;
		}
	}
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Finished begin play"));
	
}

// Called every frame
void APowerOutlet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

