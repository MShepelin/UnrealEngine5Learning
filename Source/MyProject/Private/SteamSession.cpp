#include "SteamSession.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"

USteamSessionSubsystem::USteamSessionSubsystem()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (Subsystem)
    {
        SessionInterface = Subsystem->GetSessionInterface();
        SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USteamSessionSubsystem::OnCreateSessionComplete);
        SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USteamSessionSubsystem::OnFindSessionsComplete);
        SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USteamSessionSubsystem::OnJoinSessionComplete);
        SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &USteamSessionSubsystem::OnDestroySessionComplete);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[USteamSessionSubsystem] Online subsystem was nullptr"));
    }

    // Log which online settings are used
    FString NetDriverName;
    GConfig->GetString(TEXT("/Script/OnlineSubsystemUtils.IpNetDriver"), TEXT("NetConnectionClassName"), NetDriverName, GEngineIni);
    UE_LOG(LogTemp, Log, TEXT("[USteamSessionSubsystem] Net Driver name is [%s]"), *NetDriverName);

    FString OnlineSubsystemName;
    GConfig->GetString(TEXT("/Script/Engine.OnlineSubsystem"), TEXT("DefaultPlatformService"), OnlineSubsystemName, GEngineIni);
    UE_LOG(LogTemp, Log, TEXT("[USteamSessionSubsystem] Subsystem name is [%s]"), *OnlineSubsystemName);
}

void USteamSessionSubsystem::CreateSteamLobby(int32 MaxPlayers, FString LobbyName, bool IsLAN)
{
    if (!SessionInterface.IsValid()) return;

    FOnlineSessionSettings SessionSettings;
    SessionSettings.bIsLANMatch = IsLAN;
    SessionSettings.NumPublicConnections = MaxPlayers;
    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bAllowJoinViaPresence = true;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.bUseLobbiesIfAvailable = true;
    SessionSettings.bAllowInvites = true;
    SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
    SessionSettings.bIsDedicated = false;

    SessionSettings.Set(SETTING_MAPNAME, LobbyName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    SessionSettings.Set(SETTING_GAMEMODE, LOBBY_GAMEMODE, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
    
    const ULocalPlayer* LC = GetWorld()->GetFirstLocalPlayerFromController();
    if (LC)
    {
        SessionInterface->CreateSession(*LC->GetPreferredUniqueNetId(), NAME_GameSession, SessionSettings);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[USteamSessionSubsystem] Local player was nullptr"));
    }
}

void USteamSessionSubsystem::FindSteamLobbies(bool IsLAN)
{
    if (!SessionInterface.IsValid()) return;

    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->MaxSearchResults = 50;
    SessionSearch->bIsLanQuery = IsLAN;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
    SessionSearch->QuerySettings.Set(SETTING_GAMEMODE, LOBBY_GAMEMODE, EOnlineComparisonOp::Equals);

    const ULocalPlayer* LC = GetWorld()->GetFirstLocalPlayerFromController();
    if (LC)
    {
        SessionInterface->FindSessions(*LC->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[USteamSessionSubsystem] Local player was nullptr"));
    }
}

void USteamSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
    TArray<FString> FilteredLobbyNames;
    if (SessionSearch.IsValid())
    {
        for (auto& Result : SessionSearch->SearchResults)
        {
            FString DisplayName;
            if (Result.Session.SessionSettings.Get(SETTING_MAPNAME, DisplayName))
            {
                FilteredLobbyNames.Add(DisplayName);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[USteamSessionSubsystem] Sessions request failed"));
    }

    OnLobbySearchComplete.Broadcast(FilteredLobbyNames);
}

void USteamSessionSubsystem::JoinSteamLobby(int32 ListIndex)
{
    if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) return;

    const ULocalPlayer* LC = GetWorld()->GetFirstLocalPlayerFromController();
    if (LC)
    {
        SessionInterface->JoinSession(*LC->GetPreferredUniqueNetId(), NAME_GameSession, SessionSearch->SearchResults[ListIndex]);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[USteamSessionSubsystem] Local player was nullptr"));
    }
}

void USteamSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    const bool WasSuccessful = Result == EOnJoinSessionCompleteResult::Success;
    if (!WasSuccessful)
    {
        UE_LOG(LogTemp, Error, TEXT("[USteamSessionSubsystem] Local player was nullptr"));
    }

    OnJoinSessionCompleteDelegate.Broadcast(WasSuccessful);
}

void USteamSessionSubsystem::StartGameFromLobby()
{
    // Should be called from host only
    FNamedOnlineSession* Session = SessionInterface->GetNamedSession(NAME_GameSession);
    if (!Session)
    {
        UE_LOG(LogTemp, Error, TEXT("[USteamSessionSubsystem] Tried to call StartGameFromLobby when no session was created"));
        return;
    }

    FString MapPath;
    const bool DidFindMapName = Session->SessionSettings.Get<FString>(SETTING_MAPNAME, MapPath);
    if (!DidFindMapName)
    {
        UE_LOG(LogTemp, Error, TEXT("[USteamSessionSubsystem] Didn't find a map name in the session settings"));
        return;
    }

    UWorld* World = GetWorld();
    if (World)
    {
        World->ServerTravel(MapPath + "?listen");
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[USteamSessionSubsystem] World was nullptr"));
    }
}

void USteamSessionSubsystem::DestroySteamLobby()
{
    if (!SessionInterface.IsValid()) return;

    SessionInterface->DestroySession(NAME_GameSession);
}

void USteamSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    OnDestroySessionCompleteDelegate.Broadcast(bWasSuccessful);
}

void USteamSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    OnCreateSessionCompleteDelegate.Broadcast(bWasSuccessful);
}

TArray<FSessionPlayerInfo> USteamSessionSubsystem::GetSessionPlayers()
{
    TArray<FSessionPlayerInfo> Result;

    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("[USteamSessionSubsystem] World was nullptr"));
        return Result;
    }

    FNamedOnlineSession* Session = SessionInterface->GetNamedSession(NAME_GameSession);
    if (!Session)
    {
        UE_LOG(LogTemp, Error, TEXT("[USteamSessionSubsystem] Tried to call GetSessionPlayers when no session was created or joined"));
        return Result;
    }

    TSharedPtr<const FUniqueNetId> HostId = Session->OwningUserId;
    FString HostName = Session->OwningUserName;

    Result.Add(FSessionPlayerInfo(HostName, true));

    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
    if (!Identity.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("[USteamSessionSubsystem] Tried to use Identity, but the pointer is empty"));
    }

    for (const TSharedRef<const FUniqueNetId>& PlayerId : Session->RegisteredPlayers)
    {
        if (HostId.IsValid() && (*PlayerId == *HostId))
        {
            continue;
        }

        FString PlayerName;
        if (Identity.IsValid())
        {
            PlayerName = Identity->GetPlayerNickname(*PlayerId);
        }
        else
        {
            PlayerName = PlayerId->ToString();
        }

        Result.Add(FSessionPlayerInfo(PlayerName, false));
    }

    return Result;
}
