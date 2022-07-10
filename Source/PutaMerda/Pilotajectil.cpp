// Fill out your copyright notice in the Description page of Project Settings.

#include "Pilotajectil.h"


// Sets default values
APilotajectil::APilotajectil()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(15.0f);
		CollisionComponent->SetSimulatePhysics(true);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}
	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 1000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
	}
	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("StaticMesh'/Game/Sphere.Sphere'"));
		if (Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/PilotajectilMaterial.PilotajectilMaterial'"));
	if (Material.Succeeded())
	{
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
	}
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	// Set the sphere's collision profile name to "Projectile".
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Balo"));

	// Event called when component hits something.
	CollisionComponent->OnComponentHit.AddDynamic(this, &APilotajectil::OnHit);

	// Default initial values
	firstFly = false;
}

// Called when the game starts or when spawned
void APilotajectil::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APilotajectil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (firstFly)
	{
		FVector ForceToAdd = FVector(100.0, 0.0, 0.0);
		ProjectileMeshComponent->AddImpulse(ForceToAdd);
	}
}

// Function that initializes the projectile's velocity in the shoot direction.
void APilotajectil::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
	firstFly = true;
}

// Function that is called when the projectile hits something.
void APilotajectil::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
		firstFly = false;
	}

	//Destroy();
}