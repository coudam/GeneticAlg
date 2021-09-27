// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <map>
#include <algorithm>

#include "Food.h"
#include "DrawDebugHelpers.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Crocodile.generated.h"

UCLASS()
class GENETICALGORITHM_API ACrocodile : public APawn
{
	GENERATED_BODY()

public:
	ACrocodile();

protected:
	virtual void BeginPlay() override;

public:	
	enum Target {
		NUN,
		DONAT,
		CROCODILE
	};
	struct Solution {
		float Vision;      // 200 - 500
		float MoveSpead;   // 100 - 300;
		float Aggression;  // 0 - 100
		float Vegie;       // 0 - 100
		float Carnivorous; // 0 - 100
	};
	float Energy = 100;
	Target Target = NUN;
	Solution stats;
	FVector2D position;
	FVector2D directionVector;

	std::vector<ACrocodile*>* enemies;
	std::vector<AFood*>* food;

	ACrocodile* CrocTarget;
	AFood* DonatTarget;
	bool isDead;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Step(float DeltaTime);
	void Spawn(std::vector<ACrocodile*>& crocs, std::vector<AFood*>& food_);
	void RandomStart();
	bool Hit();
	void Dead();
	void crossover(ACrocodile* first, ACrocodile* second);
	FVector2D Direction();
private:
	void Turn(const FVector2D& target);
};
