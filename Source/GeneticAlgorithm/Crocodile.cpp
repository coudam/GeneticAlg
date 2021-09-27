// Fill out your copyright notice in the Description page of Project Settings.


#include "Crocodile.h"

// Sets default values
ACrocodile::ACrocodile()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACrocodile::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(true);
}

// Called every frame
void ACrocodile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACrocodile::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACrocodile::Step(float DeltaTime)
{
	if (isDead) return;

	DrawDebugLine(GetWorld(), FVector(position, 50), FVector(position + directionVector * 300, 50), FColor::Blue, false, -1, 0, 10);

	//DrawDebugCircle(GetWorld(), FVector(position, 70), stats.Vision, 50, FColor::Blue, false, -1, 0, 10);

	DrawDebugLine(GetWorld(), FVector(position - FVector2D(0, 50), 220), FVector(position + FVector2D(0, Energy - 50), 220), FColor::Green, false, -1, 0, 20);
	DrawDebugLine(GetWorld(), FVector(position - FVector2D(0, 50), 200), FVector(position + FVector2D(0, 50), 200), FColor::Red, false, -1, 0, 20);


	auto newDirection = Direction();
	if (Target == CROCODILE && FVector2D::Distance(position, CrocTarget->position) < 10) {
		Energy -= 10;
		if (CrocTarget->Hit())
			Energy += 60;
	} else if (Target == DONAT && FVector2D::Distance(position, DonatTarget->position) < 10) {
		Energy += 20;
		DonatTarget->Eat();
	}

	directionVector = newDirection;
	auto position_t = position + directionVector * DeltaTime * stats.MoveSpead;
	while (position_t.X > 4000 || position_t.X < -4000 || position_t.Y > 4000 || position_t.Y < -4000) {
		FVector2D t = FVector2D(rand() % 100 - 50, rand() % 100 - 50);
		t.Normalize();
		Turn(t);
		directionVector = t;
		position_t = position + directionVector * DeltaTime * stats.MoveSpead * 10;
	}
	position = position_t;

	SetActorLocation(FVector(position, 70.f));
	Energy -= stats.Vision * 0.00005 + stats.MoveSpead * 0.00005;
	if (Energy <= 0) {
		Dead();
	}
}

void ACrocodile::Spawn(std::vector<ACrocodile*>& crocs, std::vector<AFood*>& food_)
{
	enemies = &crocs;
	food = &food_;
	isDead = false;

	SetActorHiddenInGame(false);
	float rand_r = std::rand() % 4000;
	float rand_f = std::rand() * 6.282f;
	position = FVector2D(cos(rand_f) * rand_r, sin(rand_f) * rand_r);
	SetActorLocation(FVector(position, 70.f));

	FVector2D t = FVector2D(rand() % 100 - 50, rand() % 100 - 50);
	directionVector = FVector2D(0, 1);
	Turn(t);
	directionVector = t;
	directionVector.Normalize();
}

void ACrocodile::RandomStart()
{
	stats.Vision      = 200  + rand() % 3000 / 10.f;
	stats.MoveSpead   = 200  + rand() % 2000 / 10.f;
	stats.Aggression  = 0    + rand() % 1000 / 10.f;
	stats.Vegie       = 0    + rand() % 1000 / 10.f;
	stats.Carnivorous = 0    + rand() % 1000 / 10.f;
}

FVector2D ACrocodile::Direction()
{
	std::vector<std::pair<ACrocodile*, float>> visibleAnimy;
	std::vector<std::pair<AFood*, float >> visibleFood;

	for (auto enemie : *enemies) {
		float dist = FVector2D::Distance(position, enemie->position);
		if (dist < stats.Vision && enemie != this && !enemie->isDead) {
			visibleAnimy.push_back({enemie, dist});
		}
	}

	for (auto f : *food) {
		float dist = FVector2D::Distance(position, f->position);
		if (dist < stats.Vision && !f->isEaten) {
			visibleFood.push_back({f, dist});
		}
	}

	std::sort(visibleAnimy.begin(), visibleAnimy.end(), [](std::pair<ACrocodile*, float >& a, std::pair<ACrocodile*, float >& b) { return a.second < b.second; });
	std::sort(visibleFood.begin(), visibleFood.end(), [](std::pair<AFood*, float >& a, std::pair<AFood*, float >& b) { return a.second < b.second; });

	FVector2D direction_;

	direction_ = directionVector;
	
	Target = NUN;
	
	if (visibleFood.empty() && !visibleAnimy.empty() && 
		       Energy > stats.Vegie) {

		direction_ = visibleAnimy[0].first->position - position;
		Target = CROCODILE;
	} else if (!visibleFood.empty() && !visibleAnimy.empty() &&
			   visibleFood[0].second > visibleAnimy[0].second &&
			   (visibleFood[0].second / visibleAnimy[0].second)*100 > stats.Aggression &&
		       Energy > stats.Vegie) {

		direction_ = visibleAnimy[0].first->position - position;
		Target = CROCODILE;
	} else if (!visibleFood.empty()) {
		
		direction_ = visibleFood[0].first->position - position;
		Target = DONAT;
	} else if (!visibleAnimy.empty()) {
	
		direction_ = (visibleAnimy[0].first->position - position)*-1.f;
		Target = CROCODILE;
	}

	if (Target == CROCODILE)
		CrocTarget = visibleAnimy[0].first;
	else if (Target == DONAT)
		DonatTarget = visibleFood[0].first;
	
	Turn(direction_);
	
	DrawDebugLine(GetWorld(), FVector(position, 50), FVector(position + direction_, 50), FColor::Red, false, -1, 0, 10);
	
	direction_.Normalize();
	return direction_;
}

void ACrocodile::Turn(const FVector2D& target)
{
	if (directionVector == target) return;
	float angle; FVector a;
	FQuat::FindBetweenNormals(FVector(target, 0.f), FVector(directionVector, 0.f)).ToAxisAndAngle(a, angle);
	angle = FMath::RadiansToDegrees(angle) * a.Z;
	AddActorLocalRotation(FRotator(0, angle, 0));
}

bool ACrocodile::Hit()
{
	Energy -= 30;
	if (Energy <= 0)
		return true;
	return false;
}

void ACrocodile::Dead()
{
	isDead = true;
	SetActorHiddenInGame(true);
}