#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SteamSession.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbySearchComplete, const TArray<FString>&, LobbyNames);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionActionComplete, bool, bWasSuccessful);


USTRUCT(BlueprintType)
struct FSessionPlayerInfo
{
    GENERATED_BODY()

public:
    FSessionPlayerInfo() {}

    FSessionPlayerInfo(const FString& InName, bool InIsHost)
        : PlayerName(InName)
        , IsHost(InIsHost)
    {}

    UPROPERTY(BlueprintReadOnly)
    FString PlayerName;

    UPROPERTY(BlueprintReadOnly)
    bool IsHost = false;    
};


UCLASS()
class MYPROJECT_API USteamSessionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    USteamSessionSubsystem();

    // Blueprint Callable Functions
    UFUNCTION(BlueprintCallable)
    void CreateSteamLobby(int32 MaxPlayers, FString LobbyName, bool IsLAN);

    UFUNCTION(BlueprintCallable)
    void FindSteamLobbies(bool IsLAN);

    UFUNCTION(BlueprintCallable)
    void JoinSteamLobby(int32 ListIndex);

    UFUNCTION(BlueprintCallable)
    void DestroySteamLobby();

    UFUNCTION(BlueprintCallable)
    void StartGameFromLobby();

    UFUNCTION(BlueprintCallable)
    void CleanupSession();

    UFUNCTION(BlueprintCallable)
    TArray<FSessionPlayerInfo> GetSessionPlayers();

    // Delegates for Blueprint UI
    UPROPERTY(BlueprintAssignable)
    FOnLobbySearchComplete OnLobbySearchComplete;

    UPROPERTY(BlueprintAssignable)
    FOnSessionActionComplete OnDestroySessionCompleteDelegate;

    UPROPERTY(BlueprintAssignable)
    FOnSessionActionComplete OnCreateSessionCompleteDelegate;

    UPROPERTY(BlueprintAssignable)
    FOnSessionActionComplete OnJoinSessionCompleteDelegate;

protected:
    // Internal Callbacks tied to OnlineSubsystem
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    void OnFindSessionsComplete(bool bWasSuccessful);
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
    void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

private:
    TSharedPtr<FOnlineSessionSearch> SessionSearch;
    IOnlineSessionPtr SessionInterface;

    // Constant for filtering
    const FString LOBBY_GAMEMODE = "SuperDeathmatch";
    const FString LOBBY_MAP_NAME = "LobbyMap";
    const FString HUB_MAP_NAME = "PlayerHub";
};
