// Fill out your copyright notice in the Description page of Project Settings.
#include "ScanPointsGrid.h"

DEFINE_LOG_CATEGORY(LogVVADscanner);

// Sets default values
AScanPointsGrid::AScanPointsGrid() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CellSize = 3000;
	MaxPointsPerHolder = 15000;
	MaxHoldersPerCell = 5;
	PointRandomOffset = 1.f;
	CullDistance.SetLowerBound(6000);
	CullDistance.SetUpperBound(9000);
	bCanMoveOldPoints = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> InstanceMesh(TEXT("/Script/Engine.StaticMesh'/VVAD_Scanner/Meshes/SM_Plane.SM_Plane'"));
	Mesh = InstanceMesh.Object;
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialInterface(TEXT("/Script/Engine.Material'/VVAD_Scanner/Materials/M_ScannerPointsDynamic.M_ScannerPointsDynamic'"));
	Material = MaterialInterface.Object;
	PointScale = 10.f;
}


// Called when the game starts or when spawned
void AScanPointsGrid::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	
}

// Called every frame
void AScanPointsGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AScanPointsGrid::AddPoints(TArray<FVector> pts, TArray<float> ptData) {
	if (pts.IsEmpty()) return false;
	FIntVector pos = LocationToPos(pts[0]);
	AScanPointsCell* cell = GetGridCell(pos);
	if (!cell) return false;

	size_t len = pts.Num();

	FVector scale = FVector(PointScale);
	FRotator rot = FRotator::ZeroRotator;

	UInstancedStaticMeshComponent* holder = cell->InstanceHolders[cell->InstanceHolders.Num() - 1];

	if (holder->GetInstanceCount() + len > MaxPointsPerHolder) {
		if (cell->InstanceHolders.Num() >= MaxHoldersPerCell) {
			if (!bCanMoveOldPoints) return false;
			holder = cell->InstanceHolders[FMath::RandRange(0, cell->InstanceHolders.Num() - 1)];

			for (size_t i = 0; i < len; i++) {
				FVector RandomOffset = FMath::VRand() * FMath::FRandRange(0.f, PointRandomOffset);
				int indx = FMath::RandRange(0, MaxPointsPerHolder);
				holder->UpdateInstanceTransform(indx, FTransform(rot, pts[i]+RandomOffset, scale), true);
				holder->SetCustomDataValue(indx, 0, ptData[i], true);
			}
			return true;
		} 
		holder = CreateInstanceHolder(pos);
		cell->InstanceHolders.Add(holder);
	}

	for (size_t i = 0; i < len; i++) {
		FVector RandomOffset = FMath::VRand() * FMath::FRandRange(0.f, PointRandomOffset);
		int indx = holder->AddInstance(FTransform(rot, pts[i]+RandomOffset, scale), true);
		holder->SetCustomDataValue(indx, 0, ptData[i], true);
	}
	PointsCountTotal += len;

	UE_LOG(LogVVADscanner, Display, TEXT("Total holders: %d Total points: %d"), HoldersCountTotal, PointsCountTotal);

	return true;
}

void AScanPointsGrid::SetMaterial(UMaterialInterface* NewMaterial) {
	if (NewMaterial != Material) {
		for (auto iter = Grid.CreateConstIterator(); iter; ++iter) {
			iter.Value()->SetMaterial(NewMaterial);
		}
	}
	Material = NewMaterial;
}

void AScanPointsGrid::ClearAll() {

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AScanPointsCell::StaticClass(), FoundActors);
	for (AActor* ActorFound : FoundActors) {
		Cast<AScanPointsCell>(ActorFound)->prepareForDestroy();
		ActorFound->Destroy();
	}

	Grid.Empty();
	PointsCountTotal = 0;
	HoldersCountTotal = 0;
}

UInstancedStaticMeshComponent* AScanPointsGrid::CreateInstanceHolder(FIntVector pos) {
	UInstancedStaticMeshComponent* ISMComp = NewObject<UInstancedStaticMeshComponent>(this);
	ISMComp->RegisterComponent();
	ISMComp->SetStaticMesh(Mesh);
	ISMComp->SetMaterial(0, Material);
	ISMComp->SetCullDistances(CullDistance.GetLowerBoundValue(), CullDistance.GetUpperBoundValue());
	ISMComp->SetCullDistance(CullDistance.GetUpperBoundValue() + (CellSize / 2.));
	ISMComp->bDisableCollision = true;
	ISMComp->SetCanEverAffectNavigation(false);
	ISMComp->SetCastShadow(false);
	ISMComp->bAffectDynamicIndirectLighting = false;
	ISMComp->bAffectDistanceFieldLighting = false;
	ISMComp->bReceivesDecals = false;
	ISMComp->bDisallowNanite = true;
	ISMComp->bVisibleInRealTimeSkyCaptures = false;
	ISMComp->bVisibleInReflectionCaptures = false;

	ISMComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ISMComp->SetNumCustomDataFloats(1);

	ISMComp->SetWorldLocation(PosToLocation(pos), false, nullptr, ETeleportType::TeleportPhysics);
	
	HoldersCountTotal++;
	return ISMComp;
}

AScanPointsCell* AScanPointsGrid::CreateCell(FIntVector pos) {
	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AScanPointsCell* cell = GetWorld()->SpawnActor<AScanPointsCell>(FVector(pos), FRotator::ZeroRotator, SpawnInfo);
	cell->InstanceHolders.Add(CreateInstanceHolder(pos));
	Grid.Add(pos, cell);
	return cell;
}

AScanPointsCell* AScanPointsGrid::GetGridCell(FIntVector pos) {
	AScanPointsCell** CellPtr = Grid.Find(pos);
	AScanPointsCell* Cell = nullptr;
	if (CellPtr) {
		Cell = *Grid.Find(pos);
	}
	if (Cell) return Cell;
	Cell = CreateCell(pos);
	if (Cell) return Cell;
	return nullptr;
}

