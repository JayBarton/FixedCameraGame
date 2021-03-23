// Fill out your copyright notice in the Description page of Project Settings.


#include "FCSlidingBlockPuzzle.h"
#include "Engine/World.h" 

AFCSlidingBlockPuzzle::AFCSlidingBlockPuzzle()
{
	int level = size * size;
	gridDefault.SetNum(level);
	grid.SetNum(level);
	boxes.SetNum(level - 1);
}

void AFCSlidingBlockPuzzle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Up", IE_Pressed, this, &AFCSlidingBlockPuzzle::MoveUp);
	PlayerInputComponent->BindAction("Down", IE_Pressed, this, &AFCSlidingBlockPuzzle::MoveDown);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AFCSlidingBlockPuzzle::MoveLeft);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AFCSlidingBlockPuzzle::MoveRight);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFCSlidingBlockPuzzle::MoveBlock);
	PlayerInputComponent->BindAction("Submit", IE_Pressed, this, &AFCSlidingBlockPuzzle::CheckSolution);

}

void AFCSlidingBlockPuzzle::Tick(float DeltaTime)
{
	HandleAnimation(DeltaTime);
}

void AFCSlidingBlockPuzzle::HandleAnimation(float& DeltaTime)
{
	if (isAnimating)
	{
		//move towards newLocation
		movingBox->SetActorLocation(FMath::Lerp(boxStartLocation, newLocation, t));
		t += DeltaTime * slideSpeed;
		if (movingBox->GetActorLocation().Equals(newLocation, 0.5f))
		{
			t = 0;
			movingBox->SetActorLocation(newLocation);
			isAnimating = false;
			EnableInput(Cast<APlayerController>(GetController()));
		}
	}
}

void AFCSlidingBlockPuzzle::MoveRight()
{
	index++;
	if (index % size == 0)
	{
		index -= size;
	}
	updateIndex();
}

void AFCSlidingBlockPuzzle::MoveLeft()
{
	index--;
	if (index < 0 || index % size == size - 1)
	{
		index += size;
	}
	updateIndex();
}

void AFCSlidingBlockPuzzle::MoveUp()
{
	index -= size;
	if (index < 0)
	{
		index += (grid.Num());
	}
	updateIndex();
}

void AFCSlidingBlockPuzzle::MoveDown()
{
	index += size;
	if (index >= grid.Num())
	{
		index -= (grid.Num());
	}
	updateIndex();
}

void AFCSlidingBlockPuzzle::MoveBlock()
{
	UE_LOG(LogTemp, Warning, TEXT("%i"), index);
	int nextIndex = FindAdjacent();
	if (nextIndex >= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%i"), nextIndex);

		grid[nextIndex] = grid[index];

		SetUpAnimation(nextIndex);

		grid[index] = -1;
		updateIndex();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NOPE"));
	}
}

void AFCSlidingBlockPuzzle::SetUpAnimation(int nextIndex)
{
	float X = startLocation.X - (nextIndex / size) * 100;
	float Y = startLocation.Y + (nextIndex % size) * 100;
	newLocation = FVector(X, Y, startLocation.Z);
	movingBox = boxes[grid[index] - 1];
	boxStartLocation = movingBox->GetActorLocation();
	isAnimating = true;
	DisableInput(Cast<APlayerController>(GetController()));
}

int32 AFCSlidingBlockPuzzle::FindAdjacent()
{
	//right check
	int32 indexToCheck = index + 1;
	if (indexToCheck % size > 0)
	{
		if (grid[indexToCheck] < 0)
		{
			return indexToCheck;
		}
	}

	//left check
	indexToCheck = index - 1;
	if (indexToCheck >= 0 && indexToCheck % size < size - 1)
	{
		if (grid[indexToCheck] < 0)
		{
			return indexToCheck;
		}
	}

	//down check
	indexToCheck = index + size;
	if (indexToCheck < grid.Num())
	{
		if (grid[indexToCheck] < 0)
		{
			return indexToCheck;
		}
	}

	indexToCheck = index - size;
	if (indexToCheck >= 0)
	{
		if (grid[indexToCheck] < 0)
		{
			return indexToCheck;
		}
	}

	return -1;
}

void AFCSlidingBlockPuzzle::CheckSolution()
{
	int sum = 0;
	for (int i = 0; i < grid.Num() - 1; i++)
	{
		if (grid[i] == i + 1)
		{
			sum++;
		}
	}
	if (sum == grid.Num() - 1)
	{
		SolvePuzzle();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Done"));
	}
}

void AFCSlidingBlockPuzzle::StartPuzzle()
{
	Super::StartPuzzle();
	grid = gridDefault;
	index = 1;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int i = 0; i < grid.Num(); i++)
	{
		if (grid[i] >= 0)
		{
			float X = startLocation.X - (i / size) * 100;
			float Y = startLocation.Y + (i % size) * 100;

			FVector location(X, Y, startLocation.Z);

			AActor* box = GetWorld()->SpawnActor<AActor>(prop, location, FRotator::ZeroRotator, SpawnParams);

			boxes[grid[i] - 1] = box;
		}
	}
	SetUp();
}

void AFCSlidingBlockPuzzle::ExitPuzzle()
{
	Super::ExitPuzzle();

	for (int i = 0; i < boxes.Num(); i++)
	{
		boxes[i]->Destroy();
	}
}

void AFCSlidingBlockPuzzle::updateIndex_Implementation()
{
}

void AFCSlidingBlockPuzzle::SetUp_Implementation()
{
}

