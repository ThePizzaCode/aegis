#include "MainController.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Engine/Engine.h"

struct Obj {
    int32 DX;
    int32 DY;
    int32 SX;
    int32 SY;
};

void SetObjectDistance(Obj &o, TArray<FString> &values, int &n) {
    o.DX = FCString::Atoi(*values[n]) / 128; n++;
    o.DY = FCString::Atoi(*values[n]) / 128; n++;
}

void SetObjectSpeed(Obj &o, TArray<FString> &values, int &n) {
    o.SX = FCString::Atoi(*values[n]) / 128; n++;
    o.SY = FCString::Atoi(*values[n]) / 128; n++;
}

struct Line {
    int32 Index;
    Obj First;
    Obj Second;
    Obj Third;
    Obj Fourth;
    int32 Speed;
    double YawRate;
    double TimeStamp;
};

// global state
Line lines[500];
int linesLength = 0;


void AMainController::Parse() {
    // get file path
    FString FilePath = FPaths::GameSourceDir() + "bosch.csv";

    // checking
    UE_LOG(LogTemp, Warning, TEXT("CSV File Path: %s"), *FilePath);

    // getting ahold of the file
    FString FileData;
    FFileHelper::LoadFileToString(FileData, *FilePath);

    // splitting into multiple strings
    TArray<FString> Tokens;
    FileData.ParseIntoArray(Tokens, TEXT("\n"), true);

    for (int i = 1; i < Tokens.Num(); i++) {
        // splitting line into multiple values
        TArray<FString> Values;
        Tokens[i].ParseIntoArray(Values, TEXT(","));

        // index first
        linesLength = i;
        int n = 0;
        lines[i].Index = FCString::Atoi(*Values[n]); n++;

        // objects distances
        SetObjectDistance(lines[i].First, Values, n);
        SetObjectDistance(lines[i].Second, Values, n);
        SetObjectDistance(lines[i].Third, Values, n);
        SetObjectDistance(lines[i].Fourth, Values, n);
        
        // speed
        lines[i].Speed = FCString::Atoi(*Values[n]) / 256; n++;

        // objects speeds
        SetObjectSpeed(lines[i].First, Values, n);
        SetObjectSpeed(lines[i].Second, Values, n);
        SetObjectSpeed(lines[i].Third, Values, n);
        SetObjectSpeed(lines[i].Fourth, Values, n);

        // yaw rate
        lines[i].YawRate = FCString::Atof(*Values[n]); n++;

        // and finally the timestamp
        lines[i].TimeStamp = FCString::Atof(*Values[n]);
    }
}

void AMainController::Debug() {
    UE_LOG(LogTemp, Warning, TEXT("dx: %d, dy: %d"), lines[1].First.DX, lines[1].First.DY);
}

AMainController::AMainController() {
    ElapsedTime = 0.0f;
    UpdateInterval = 0.5f;
    iterator = 0;
}

void AMainController::BeginPlay() {
    UE_LOG(LogTemp, Warning, TEXT("starting..."));

    Parse();
    Debug();

    if (FirstObj) {
        FVector NewLocation = FVector(10, 10, 20);
        FRotator NewRotation = FRotator(0, 0, 0);

        FirstObj->SetActorLocation(NewLocation);
    }
}


void AMainController::Tick(float DeltaTime) {
    if (iterator < 420) {
        ElapsedTime += DeltaTime;
        int v = 5;

        if (ElapsedTime >= UpdateInterval) {

            FVector LocationFirst = FVector(
                (lines[iterator].First.DX  > v || lines[iterator].First.DX  < -v ) ? lines[iterator].First.DX * 10 : 1000 , 
                (lines[iterator].First.DY  > v || lines[iterator].First.DY < -v ) ? lines[iterator].First.DY * 10 : 1000 , 
                0
                );

            FVector LocationSecond = FVector(
                (lines[iterator].Second.DX > v || lines[iterator].Second.DX  < -v ) ? lines[iterator].Second.DX * 10 : 1000 , 
                (lines[iterator].Second.DY  > v || lines[iterator].Second.DY  < -v ) ? lines[iterator].Second.DY * 10 : 1000 , 
                0
                );

            FVector LocationThird = FVector(
                (lines[iterator].Third.DX > v || lines[iterator].Third.DX  < -v ) ? lines[iterator].Third.DX * 10 : 1000 , 
                (lines[iterator].Third.DY  > v || lines[iterator].Third.DY  < -v ) ? lines[iterator].Third.DY * 10 : 1000 , 
                0
                );

            FVector LocationFourth = FVector(
                (lines[iterator].Fourth.DX  > v || lines[iterator].Fourth.DX  < -v ) ? lines[iterator].Fourth.DX * 10 : 1000 , 
                (lines[iterator].Fourth.DY  > v || lines[iterator].Fourth.DY  < -v ) ? lines[iterator].Fourth.DY * 10 : 1000 , 
                0
                );

            UE_LOG(LogTemp, Warning, TEXT("dx: %d, dy: %d"), lines[iterator].First.DX, lines[iterator].First.DY);
            UE_LOG(LogTemp, Warning, TEXT("et %f"), ElapsedTime);
            

            iterator++;

            FirstObj->SetActorLocation(LocationFirst);
            SecondObj->SetActorLocation(LocationSecond);
            ThirdObj->SetActorLocation(LocationThird);
            FourthObj->SetActorLocation(LocationFourth);

            ElapsedTime = 0.0f;
        }
    }
}
