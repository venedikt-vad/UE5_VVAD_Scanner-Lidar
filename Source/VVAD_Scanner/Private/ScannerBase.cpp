#include "ScannerBase.h"

UScannerBase::UScannerBase() {
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(false);

	ArrowColor = FColor(255, 100, 0);

	TraceDistance = 10000;
	TraceChannel = ECollisionChannel::ECC_Camera;
	bDrawTraceLines = true;

	ScanSpeed = 100;
	MaxTracesPerFrame = 300;
}


int UScannerBase::CalculateNumTraces() {
	float t = UGameplayStatics::GetRealTimeSeconds(this);

	int count = floor(FMath::Min(ScanSpeed * (t - LastTraceTime), MaxTracesPerFrame));

	if (count > 0) {
		LastTraceTime = t;
	}

	return count;
}

bool UScannerBase::TraceLocalDirection(FVector dir) {

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(GetOwner());
	UKismetMathLibrary::TransformDirection(GetComponentTransform(), dir);
	FVector TraceEnd = UKismetMathLibrary::TransformDirection(GetComponentTransform(), dir).GetUnsafeNormal()*TraceDistance + GetComponentLocation();
	if (!GetWorld()->LineTraceSingleByChannel(Hit, GetComponentLocation(), TraceEnd, TraceChannel, QueryParams)) return false;

	if (bReduceCloseTraces) {
		if (Hit.Distance / 4. - FMath::FRandRange(10.f, 3000.f) < 0) {
			return false;
		}
	}

	PtsLocations.Add(Hit.Location);
	PtsData.Add(Hit.PhysMaterial->SurfaceType.GetIntValue());
	
	if (bDrawTraceLines) {
		FBatchedLine line = FBatchedLine(GetComponentLocation(), Hit.Location, GetColorForTrace(Hit), 0, 0.1, 4);
		LinesToDraw.Add(line);
	}
	
	return true;
}

void UScannerBase::SendPointsToGrid() {
	Grid->AddPoints(PtsLocations, PtsData);
	PtsLocations.Empty();
	PtsData.Empty();

	if (bDrawTraceLines) {
		LineBatchComponent->DrawLines(LinesToDraw);
		LinesToDraw.Empty();
	}
}

void UScannerBase::StartTracing() {
	SetComponentTickEnabled(true);
	LastTraceTime = UGameplayStatics::GetRealTimeSeconds(this);
}

void UScannerBase::StopTracing() {
	SetComponentTickEnabled(false);
	LineBatchComponent->Flush();
}

FLinearColor UScannerBase::GetColorForTrace_Implementation(FHitResult Hit) {
	return FLinearColor((Hit.Distance  / TraceDistance)*360 ,1.f,1.f).HSVToLinearRGB();
}

// Called when the game starts
void UScannerBase::BeginPlay() {
	SetComponentTickEnabled(false);
	Super::BeginPlay();

	Grid = Cast<AScanPointsGrid>(UGameplayStatics::GetActorOfClass(this, AScanPointsGrid::StaticClass()));
	if (!Grid) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Grid = GetWorld()->SpawnActor<AScanPointsGrid>(FVector::Zero(), FRotator::ZeroRotator, SpawnInfo);
	}
	
}


// Called every frame
void UScannerBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	LineBatchComponent->Flush();
}

