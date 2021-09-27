// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <algorithm>
#include <unordered_map>
#include <queue>
#include <map>
#include <math.h>
#include <cstdlib>
#include <utility>
#include <random>
#include <vector>
#include <iterator>


#include "Food.h"
#include "Crocodile.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BraveWorld.generated.h"

UCLASS()
class GENETICALGORITHM_API ABraveWorld : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABraveWorld();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "algo")
		int Population;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "algo")
		int Generations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		TSubclassOf<AFood> FoodType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
		TSubclassOf<ACrocodile> AnimalType;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	std::vector<ACrocodile*>* getCroco();

private:
	struct Run {
		std::vector<AFood*> Food;
		std::vector<ACrocodile*> Crocodiles;
		std::vector<ACrocodile*>::iterator ZooGet;
		std::vector<AFood*>::iterator FridgeGet;

		ABraveWorld* ctx;

		Run();
		void init(ABraveWorld* ctx_);
		Run(Run&& privGen);
		~Run();

		void start();
		void crossover();
		void mutation();
		bool isRuning(); 
		void step(float DeltaTime);
		void newGeneration();
	};

	std::vector<AFood*> Fridge;
	std::vector<ACrocodile*> Zoo;
	std::vector<ACrocodile::Solution> Solutions;
	Run generation;

	void SpawnCockroach();
};