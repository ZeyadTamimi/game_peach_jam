// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "PaperFlipbookComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpriteComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Bullet Sprite"));
	RootComponent = SpriteComponent;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	SpriteComponent->SetFlipbook(FlyingAnimation);
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SpriteComponent->SetFlipbook(FlyingAnimation);
	// Get all overlapping actors
	TArray<AActor*> InteractionActors;
	CollisionComponent->GetOverlappingActors(InteractionActors);
	if (InteractionActors.Num() > 0)
		Destroy();


}

