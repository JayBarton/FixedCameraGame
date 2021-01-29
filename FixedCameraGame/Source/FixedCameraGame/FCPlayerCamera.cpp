// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPlayerCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h" 
#include "Materials/MaterialInstanceDynamic.h" 

AFCPlayerCamera::AFCPlayerCamera()
{
	//tick will probably be false later
	PrimaryActorTick.bCanEverTick = false;
	GetCameraComponent()->bConstrainAspectRatio = false;
	//GetCameraComponent()->SetAspectRatio(2.35);
	//GetCameraComponent()->SetFieldOfView(75.0f);
}

void AFCPlayerCamera::BeginPlay()
{
	Super::BeginPlay();

	SetDynamicMaterial();

	//dynamicMaterial->SetScalarParameterValue("Alpha", 1.0f);
}

void AFCPlayerCamera::SetDynamicMaterial()
{
	if (!materialSetUp)
	{
		UE_LOG(LogTemp, Warning, TEXT("hey hey"));
		dynamicMaterial = UMaterialInstanceDynamic::Create(MaterialTest, this);
		GetCameraComponent()->AddOrUpdateBlendable(dynamicMaterial);
		materialSetUp = true;
	}
}

void AFCPlayerCamera::UpdateMaterial(float t, float transitionTime)
{
	float alpha = t / transitionTime;
	SetMaterial(alpha);
}

void AFCPlayerCamera::SetMaterial(float alpha)
{
	dynamicMaterial->SetScalarParameterValue("Alpha", alpha);

}