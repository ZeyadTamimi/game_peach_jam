// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SecurityGuardController.generated.h"

/**
 * 
 */
UCLASS()
class POWERLINES_API ASecurityGuardController : public AAIController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBlackboardData* BlackboardData;

public:

	ASecurityGuardController();

	virtual void BeginPlay() override;
};
