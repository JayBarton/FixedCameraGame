// Fill out your copyright notice in the Description page of Project Settings.


#include "FCPlayerCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h" 
#include "Materials/MaterialInstanceDynamic.h" 

AFCPlayerCamera::AFCPlayerCamera()
{
	//tick will probably be false later
	PrimaryActorTick.bCanEverTick = true;
	GetCameraComponent()->bConstrainAspectRatio = false;
	//GetCameraComponent()->SetAspectRatio(2.35);
	//GetCameraComponent()->SetFieldOfView(75.0f);
}

void AFCPlayerCamera::BeginPlay()
{
	Super::BeginPlay();
	dynamicMaterial = UMaterialInstanceDynamic::Create(MaterialTest, this);

	GetCameraComponent()->AddOrUpdateBlendable(dynamicMaterial);
	//dynamicMaterial->SetScalarParameterValue("Alpha", 1.0f);
}

void AFCPlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float alpha = 0.0f;
	alpha = FMath::Pow(FMath::Sin(UGameplayStatics::GetTimeSeconds(GetWorld()) * 5.0f), 2);
	dynamicMaterial->SetScalarParameterValue("Alpha", alpha);

}
