// Fill out your copyright notice in the Description page of Project Settings.


#include "FCDescription.h"
#include "FCInfoTextWidget.h"
#include "Kismet/GameplayStatics.h" 
#include "FixedCameraGameGameMode.h"
#include "FCPlayerCamera.h"

AFCDescription::AFCDescription()
{
	prompt = "Examine";
}

void AFCDescription::Action_Implementation()
{
	/*if (descriptionCamera)
	{
		auto playerCamera = Cast<ACameraActor>(pc->GetViewTarget());
		FViewTargetTransitionParams transitionParams;
		pc->SetViewTarget(descriptionCamera, transitionParams);
	}*/
	//if camera
	/*
	playerCamera = Cast<ACameraActor>(pc->GetViewTarget());
	FViewTargetTransitionParams transitionParams;
	pc->SetViewTarget(descriptionCamera, transitionParams);
	*/
	auto gameMode = Cast<AFixedCameraGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	gameMode->DisplayText(description, nullptr, nullptr, nullptr, true, descriptionCamera);
}
