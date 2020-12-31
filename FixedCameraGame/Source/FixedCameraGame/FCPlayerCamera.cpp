// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPlayerCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h" 

AFCPlayerCamera::AFCPlayerCamera()
{
	GetCameraComponent()->bConstrainAspectRatio = false;
	//GetCameraComponent()->SetAspectRatio(2.35);
	//GetCameraComponent()->SetFieldOfView(75.0f);
}

/*void AFCPlayerCamera::BeginPlay()
{
	if (startingCamera)
	{
		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (playerController)
		{
			FViewTargetTransitionParams transitionParams;
			playerController->SetViewTarget(this, transitionParams);
		}
	}
}*/
