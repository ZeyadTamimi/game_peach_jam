// Fill out your copyright notice in the Description page of Project Settings.

#include "SecurityGuardController.h"

ASecurityGuardController::ASecurityGuardController()
{

}

void ASecurityGuardController::BeginPlay()
{
	RunBehaviorTree(BehaviorTree);
	Super::BeginPlay();
}




