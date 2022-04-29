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
	if (dynamicMaterial)
	{
		dynamicMaterial->SetScalarParameterValue("Alpha", alpha);
	//	dynamicMaterial->SetVectorParameterValue("Color", FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("2"));
	}

}