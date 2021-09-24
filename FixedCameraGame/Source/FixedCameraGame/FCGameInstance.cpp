// Fill out your copyright notice in the Description page of Project Settings.


#include "FCGameInstance.h"
#include "Kismet/GameplayStatics.h" 
#include "Components/AudioComponent.h" 


void UFCGameInstance::PlayMusic(USoundBase* bgm, float volume, bool persist)
{
	//music = UGameplayStatics::SpawnSound2D(GetWorld(), bgm, volume, 1.0f, 0.0f, (USoundConcurrency*)nullptr, persist);
	isPlayingMusic = true;
	music = UGameplayStatics::CreateSound2D(GetWorld(), bgm, 0.5f, 1.0f, 0.0f, (USoundConcurrency*)nullptr, true);
	music->FadeIn(1.0f);
}

void UFCGameInstance::StopMusic(float transitionTime)
{
	music->FadeOut(transitionTime, 0.0f);
	isPlayingMusic = false;
}
