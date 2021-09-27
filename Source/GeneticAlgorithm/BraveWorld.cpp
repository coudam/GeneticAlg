// Fill out your copyright notice in the Description page of Project Settings.


#include "BraveWorld.h"

// Sets default values
ABraveWorld::ABraveWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	if (Population == 0)
		Population = 40;
	if (Generations == 0)
		Generations = 10;
}

void ABraveWorld::SpawnCockroach()
{
}

// Called when the game starts or when spawned
void ABraveWorld::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < Population * Generations; ++i) {
		float rand_r = std::rand() % 4000;
		float rand_f = std::rand() * 6.282f;
		FVector Location(cos(rand_f) * rand_r, sin(rand_f) * rand_r, 70.f);
		FRotator Rotation(0, 0, 0);
		auto crocodile = GetWorld()->SpawnActor<ACrocodile>(AnimalType, Location, Rotation);
		Zoo.push_back(crocodile);
	}

	for (int i = 0; i < Population * 3; ++i) {
		float rand_r = std::rand() % 4000;
		float rand_f = std::rand() * 6.282f;
		FVector Location(cos(rand_f) * rand_r, sin(rand_f) * rand_r, 20.f);
		FRotator Rotation(0.f, 0.f, 0.f);
		auto donat = GetWorld()->SpawnActor<AFood>(FoodType, Location, Rotation);
		Fridge.push_back(donat);
	}

	generation.init(this);
}

// Called every frame
void ABraveWorld::Tick(float DeltaTime)
{

	DrawDebugLine(GetWorld(), FVector(-4000, -4000, 30), FVector(-4000, 4000, 30), FColor::Red, true, -1, 0, 10);
	DrawDebugLine(GetWorld(), FVector(-4000, 4000, 30), FVector(4000, 4000, 30), FColor::Red, true, -1, 0, 10);
	DrawDebugLine(GetWorld(), FVector(4000, 4000, 30), FVector(4000, -4000, 30), FColor::Red, true, -1, 0, 10);
	DrawDebugLine(GetWorld(), FVector(4000, -4000, 30), FVector(-4000, -4000, 30), FColor::Red, true, -1, 0, 10);


	Super::Tick(DeltaTime);
	if (generation.isRuning()) {
		generation.step(DeltaTime);
	} else {
		generation.newGeneration();
	}
}

ABraveWorld::Run::Run()
{

}

void ABraveWorld::Run::init(ABraveWorld* ctx_)
{
	ctx = ctx_;
	ZooGet = ctx->Zoo.begin();
	FridgeGet = ctx->Fridge.begin();
	for (int i = 0; i < ctx->Population; ++i) {
		Crocodiles.push_back(*ZooGet);
		++ZooGet;
		Crocodiles.back()->Spawn(Crocodiles, Food);
		Crocodiles.back()->RandomStart();

		Food.push_back(*++FridgeGet);
		Food.back()->Spawn();
	}
}

ABraveWorld::Run::Run(Run&& privGen)
{

}

ABraveWorld::Run::~Run()
{

}


void ABraveWorld::Run::start()
{

}

template<typename Iter, typename RandomGenerator>
static Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
	std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
	std::advance(start, dis(g));
	return start;
}


template<typename Iter>
static Iter select_randomly(Iter start, Iter end) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return select_randomly(start, end, gen);
}


void ABraveWorld::Run::crossover()
{
	std::vector<ACrocodile*> newGen;
	while (Crocodiles.size() >= 2) {
		auto first = select_randomly(Crocodiles.begin(), Crocodiles.end());
		auto firstCroc = *first;
		Crocodiles.erase(first);
	
		auto second = select_randomly(Crocodiles.begin(), Crocodiles.end());
		auto secondCroc = *second;
		Crocodiles.erase(second);

		ACrocodile::crossover(firstCroc, secondCroc, ZooGet, newGen);
		firstCroc->Dead();
		secondCroc->Dead();
	}
	
	Crocodiles.insert(Crocodiles.end(), std::make_move_iterator(newGen.begin()),
		std::make_move_iterator(newGen.end()));

}

void ABraveWorld::Run::mutation()
{

}

bool ABraveWorld::Run::isRuning()
{
	for (auto it = Crocodiles.end()-1; it != Crocodiles.begin(); --it) {
		if ((*it)->isDead) {
			Crocodiles.erase(it);
		}
	}
	
	if (Crocodiles.size() <= ctx->Population * 0.5) {
		return false;
	}
	return true;
}

void ABraveWorld::Run::step(float DeltaTime)
{
	for (auto croc : Crocodiles) {
		croc->Step(DeltaTime);
	}
}
static int gen_num = 0;
void ABraveWorld::Run::newGeneration()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Generation num = %d"), ++gen_num));
	for (auto croc : Crocodiles)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Vision : %f | Move spead : %f | Aggression : %f | Vegie : %f | Carniorous : %f |"), croc->stats.Vision, croc->stats.MoveSpead, croc->stats.Aggression, croc->stats.Vegie, croc->stats.Carnivorous));

	crossover();
	mutation();
	for (auto croc : Crocodiles) {
		croc->Spawn(Crocodiles, Food);
	}
}

std::vector<ACrocodile*>* ABraveWorld::getCroco()
{
	return &generation.Crocodiles;
}