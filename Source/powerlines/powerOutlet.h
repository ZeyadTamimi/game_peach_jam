// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperFlipbookComponent.h"
#include "PowerOutlet.generated.h"


UCLASS()
class POWERLINES_API APowerOutlet : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleComponent;

	UPROPERTY(VisibleAnywhere, Category = "Actor State")
	int Uses;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UParticleSystem* TeleportationEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor State")
	int InitialUses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor State")
	int RequieredLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor State")
	int ConnectionNumber;
public:

	class APowerOutlet* ConnectedPowerOutlet;

	// Sets default values for this actor's properties
	APowerOutlet();

	bool Use(int level);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns Flipbook subobject **/
	FORCEINLINE class UPaperFlipbookComponent* GetFlipbookComponent() const { return FlipbookComponent; }

	/** Returns Flipbook subobject **/
	FORCEINLINE class UBoxComponent* GetInteractionBox() const { return InteractionComponent; }

	/** Returns Flipbook subobject **/
	FORCEINLINE class UParticleSystemComponent* GetParticleComponent() const { return ParticleComponent; }
};
