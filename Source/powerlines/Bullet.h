// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class POWERLINES_API ABullet : public AActor
{
	GENERATED_BODY()
	
private:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComponent;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* SpriteComponent;



protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* FlyingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class ABullet* BulletClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	ABullet();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComponent() const { return CollisionComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class UPaperFlipbookComponent* GetSpriteComponent() const { return SpriteComponent; }
	
};
