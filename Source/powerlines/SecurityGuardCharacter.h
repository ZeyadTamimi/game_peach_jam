// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "powerlinesCharacter.h"
#include "SecurityGuardCharacter.generated.h"



UCLASS()
class POWERLINES_API ASecurityGuardCharacter : public APaperCharacter
{
	GENERATED_BODY()


private:

	bool Alive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* BulletSpawnComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State")
	EPlayerState State;

	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* DeadAnimation;

	void UpdateCharacter();

	void UpdateCharacterState();

	void UpdateAnimation();

	UFUNCTION(BlueprintCallable)
	void Fire();

	
public:
	ASecurityGuardCharacter();

	virtual void Tick(float DeltaSeconds) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USphereComponent* GetBulletSpawnComponent() const { return BulletSpawnComponent; }
};
