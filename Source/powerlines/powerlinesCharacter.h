// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PowerOutlet.h"
#include "powerlinesCharacter.generated.h"

class UTextRenderComponent;


UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	RUN 	UMETA(DisplayName = "Run"),
	IDLE 	UMETA(DisplayName = "Idle"),
	JUMP	UMETA(DisplayName = "Jump"),
	CROUCH  UMETA(DisplayName = "Crouch"),
	HIT     UMETA(DisplayName = "HIT"),
	DEAD    UMETA(DisplayName = "Dead")
};

/**
 * This class is the default character for powerlines, and it is responsible for all
 * physical interaction between the player and the world.
 *
 * The capsule component (inherited from ACharacter) handles collision with the world
 * The CharacterMovementComponent (inherited from ACharacter) handles movement of the collision capsule
 * The Sprite component (inherited from APaperCharacter) handles the visuals
 */
UCLASS(config=Game)
class ApowerlinesCharacter : public APaperCharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* InteractionSphere;

	UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, Category = "Player State")
	int HP;

protected:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animations)
	class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* JumpingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* CrouchingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* KnockBackAnnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* DeadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State")
	EPlayerState State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State")
	int InitialHP;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateCharacter();

	void UpdateCharacterState();

	void UpdateAnimation();

	/** Input Handlers */
	void MoveRight(float Value);

	virtual void Jump();

	virtual void Landed(const FHitResult & Hit);

	void Use();

	void HandleOutlet(APowerOutlet* const InteractionOutlet);

	void Rig();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	ApowerlinesCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USphereComponent* GetInteractionSphere() const { return InteractionSphere; }
};
