// Fill out your copyright notice in the Description page of Project Settings.

#include "SecurityGuardCharacter.h"
#include "SecurityGuardController.h"


ASecurityGuardCharacter::ASecurityGuardCharacter()
{
	AIControllerClass = ASecurityGuardController::StaticClass();
}

