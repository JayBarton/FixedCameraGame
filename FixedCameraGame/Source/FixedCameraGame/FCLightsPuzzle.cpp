// Fill out your copyright notice in the Description page of Project Settings.


#include "FCLightsPuzzle.h"
#include "Kismet/GameplayStatics.h" 

AFCLightsPuzzle::AFCLightsPuzzle()
{
	int level = size * size;
	gridDefault.SetNum(level);
	grid.SetNum(level);
	boxes.SetNum(level);
}

void AFCLightsPuzzle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Up", IE_Pressed, this, &AFCLightsPuzzle::MoveUp);
	PlayerInputComponent->BindAction("Down", IE_Pressed, this, &AFCLightsPuzzle::MoveDown);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AFCLightsPuzzle::MoveLeft);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AFCLightsPuzzle::MoveRight);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFCLightsPuzzle::ToggleLight);
	PlayerInputComponent->BindAxis("UpAndDown", this, &AFCLightsPuzzle::AxisY);
	PlayerInputComponent->BindAxis("LeftAndRight", this, &AFCLightsPuzzle::AxisX);
}

void AFCLightsPuzzle::MoveRight()
{
	index++;
	if (index % size == 0)
	{
		index -= size;
	}
	updateIndex();
}

void AFCLightsPuzzle::MoveLeft()
{
	index--;
	if (index < 0 || index % size == size - 1)
	{
		index += size;
	}
	updateIndex();
}

void AFCLightsPuzzle::MoveUp()
{
	index -= size;
	if (index < 0)
	{
		index += (grid.Num());
	}
	updateIndex();
}

void AFCLightsPuzzle::MoveDown()
{
	index += size;
	if (index >= grid.Num())
	{
		index -= (grid.Num());
	}
	updateIndex();
}

void AFCLightsPuzzle::ToggleLight()
{
	grid[index] = !grid[index];
	//toggle adjacent
	int32 aIndex = index + 1;
	if (aIndex % size > 0)
	{
		grid[aIndex] = !grid[aIndex];
	}
	aIndex = index - 1;
	if (aIndex >= 0 && aIndex % size < size - 1)
	{
		grid[aIndex] = !grid[aIndex];
	}
	//down check
	aIndex = index + size;
	if (aIndex < grid.Num())
	{
		grid[aIndex] = !grid[aIndex];
	}
	aIndex = index - size;
	if (aIndex >= 0)
	{
		grid[aIndex] = !grid[aIndex];
	}
	if (grid[index])
	{
		UGameplayStatics::PlaySound2D(GetWorld(), onSound);
	}
	else
	{
		UGameplayStatics::PlaySound2D(GetWorld(), offSound);
	}
	Toggle();
	CheckSolution();
}

void AFCLightsPuzzle::AxisX(float value)
{
	if (!moveXAxis)
	{
		if (value == 1)
		{
			moveXAxis = true;
			MoveRight();
		}
		else if (value == -1)
		{
			moveXAxis = true;
			MoveLeft();
		}
	}
	if (value == 0)
	{
		moveXAxis = false;
	}
}

void AFCLightsPuzzle::AxisY(float value)
{
	if (!moveYAxis)
	{
		if (value == 1)
		{
			moveYAxis = true;
			MoveUp();
		}
		else if (value == -1)
		{
			moveYAxis = true;
			MoveDown();
		}
	}
	if (value == 0)
	{
		moveYAxis = false;
	}
}

void AFCLightsPuzzle::StartPuzzle()
{
	Super::StartPuzzle();
	grid = gridDefault;
	index = 0;
	FActorSpawnParameters SpawnParams;
	FVector whatever = GetActorLocation();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
	for (int i = 0; i < grid.Num(); i++)
	{
		/*
				float X = startLocation.X - (i / size) * 110;
		float Y = startLocation.Y + (i % size) * 110;
		*/
		//Just testing using the object location as the start location
		//This is the top left of the grid, so I would need move the camera, or use the actorlocation as the center and calculate the top left
		float X = whatever.X - (i / size) * 110;
		float Y = whatever.Y + (i % size) * 110;

		FVector location(X, Y, startLocation.Z);

		AActor* box = GetWorld()->SpawnActor<AActor>(prop, location, FRotator::ZeroRotator, SpawnParams);

		boxes[i] = box;
	}
	SetUp();
}

void AFCLightsPuzzle::ExitPuzzle()
{
	Super::ExitPuzzle();

	for (int i = 0; i < boxes.Num(); i++)
	{
		boxes[i]->Destroy();
	}
}

void AFCLightsPuzzle::CheckSolution()
{
	for (int i = 0; i < grid.Num(); i++)
	{
		if (grid[i] == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Way"));
			return;
		}
	}
	SolvePuzzle();
}

void AFCLightsPuzzle::SetUp_Implementation()
{
}

void AFCLightsPuzzle::updateIndex_Implementation()
{
}

void AFCLightsPuzzle::Toggle_Implementation()
{
}
