// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "powerlinesCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "PowerOutlet.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// ApowerlinesCharacter

ApowerlinesCharacter::ApowerlinesCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Create an orthographic camera (no perspective) and attach it to the boom
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootstepAudioComp"));
	AudioComponent->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false; 


	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

    // 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
    // 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
    // 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    // 	TextComponent->SetupAttachment(RootComponent);
	
	/** Configure Sound Objects */
	// Load Sound Cue for footsteps
	//static ConstructorHelpers::FObjectFinder<USoundCue> footstepCue(TEXT("'/Game/PowerLines/Sounds/footsteps.footsteps'"));

	// Store a reference to the Cue asset
	//footstepAudioCue = footstepCue.Object;
	

	/** End of sound configuration */

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	// Setup Initial player state
	State = EPlayerState::IDLE;
	PreviousState = EPlayerState::IDLE;
	InitialHP = 3;
	HP = InitialHP;
	Level = 1;
}

//////////////////////////////////////////////////////////////////////////
// Animation

void ApowerlinesCharacter::UpdateAnimation()
{
	switch (State)
	{
	case EPlayerState::IDLE:
		GetSprite()->SetFlipbook(IdleAnimation);
		break;
	case EPlayerState::RUN:
		GetSprite()->SetFlipbook(RunningAnimation);
		break;
	case EPlayerState::JUMP:
		GetSprite()->SetFlipbook(JumpingAnimation);
		break;
	default:
		GetSprite()->SetFlipbook(IdleAnimation);
	}
}

void ApowerlinesCharacter::UpdateAudio()
{

	// Only update audio on change of state
	if (State == PreviousState)
		return;

	/* if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
			FString::Printf(TEXT("State: %s, Prev State: %s"), (char)State, (char)PreviousState)); */

	switch (State)
	{
	case EPlayerState::IDLE:
	{
		AudioComponent->Stop();
		break;
	}
	case EPlayerState::RUN:
		AudioComponent->SetSound(FootstepAudioCue);
		AudioComponent->Play();
		break;
	case EPlayerState::JUMP:
	{	
		AudioComponent->SetSound(JumpAudioCue);
		AudioComponent->Play();
		break;
	}
	case EPlayerState::LANDED:
	{
		AudioComponent->SetSound(LandAudioCue);
		AudioComponent->Play();
		break;
	}
	default:
		AudioComponent->Stop();
	}
}

void ApowerlinesCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCharacter();	
}


//////////////////////////////////////////////////////////////////////////
// Input

void ApowerlinesCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ApowerlinesCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ApowerlinesCharacter::MoveRight);

	PlayerInputComponent->BindAction("Use", IE_Released, this, &ApowerlinesCharacter::Use);
	PlayerInputComponent->BindAction("Rig", IE_Released, this, &ApowerlinesCharacter::Rig);
}

void ApowerlinesCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void ApowerlinesCharacter::Jump()
{
	PreviousState = State;
	State = EPlayerState::JUMP;
	Super::Jump();
}

void ApowerlinesCharacter::Landed(const FHitResult & Hit)
{
	PreviousState = State;
	State = EPlayerState::LANDED;
	Super::Landed(Hit);
}

void ApowerlinesCharacter::Use()
{
	// Get all overlapping actors
	TArray<AActor*> InteractionActors;
	InteractionSphere->GetOverlappingActors(InteractionActors);
	// Specific Item handling
	for (int32 InteractionIndex = 0; InteractionIndex < InteractionActors.Num(); ++InteractionIndex)
	{
		APowerOutlet* const InteractionOutlet = Cast<APowerOutlet>(InteractionActors[InteractionIndex]);
		if (InteractionOutlet && !InteractionOutlet->IsPendingKill())
		{
			HandleOutlet(InteractionOutlet);
			return;
		}
	}
}

void ApowerlinesCharacter::HandleOutlet(APowerOutlet* const InteractionOutlet)
{
	UE_LOG(LogTemp, Warning, TEXT("Touched an outlet"));
	if (!InteractionOutlet->ConnectedPowerOutlet)
		return;

	if (!InteractionOutlet->Use(Level))
		return;

	FVector teleportLocation = InteractionOutlet->ConnectedPowerOutlet->GetActorLocation();

	SetActorLocation(teleportLocation, false, nullptr, ETeleportType::TeleportPhysics);

	PreviousState = EPlayerState::IDLE;
	State = PreviousState;
	AudioComponent->SetSound(ZapAudioCue);
	AudioComponent->Play();
}


void ApowerlinesCharacter::Rig()
{
	// Get all overlapping actors

	// Item Specific Handling
	AudioComponent->Play();
}

void ApowerlinesCharacter::UpdateCharacterState()
{
	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Set the rotation so that the character faces his direction of travel.
	// This is important as allmost all animations are directional
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}

	// When Jumping we dont want to be running
	if (State == EPlayerState::JUMP)
		return;

	UE_LOG(LogTemp, Warning, TEXT("WE ARE RUNNING"));
	State = (PlayerSpeedSqr > 0.0f) ? EPlayerState::RUN : EPlayerState::IDLE;
	
}


void ApowerlinesCharacter::UpdateCharacter()
{
	UpdateCharacterState();
	UpdateAnimation();
	UpdateAudio();
	PreviousState = State;
}
