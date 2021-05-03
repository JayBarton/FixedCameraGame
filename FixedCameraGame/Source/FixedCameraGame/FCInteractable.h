// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FCInteractableInterface.h"
#include "FCInteractable.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class FIXEDCAMERAGAME_API AFCInteractable : public AActor, public IFCInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFCInteractable(); 

	//Not sure if this needs to be blueprint readonly, using it for now until I know how I will be displaying items
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool active = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		FString prompt = "";

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
		void Action();  // prototype declaration
	virtual void Action_Implementation() override;  // actual implementation of our interact method

};
