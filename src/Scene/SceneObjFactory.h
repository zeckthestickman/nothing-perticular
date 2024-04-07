#pragma once

#include <basis/seadTypes.h>

namespace al {
class SceneObjHolder;
}

enum class SceneObjID : s32 {
    AmiiboNpcDirector,
    BgmAnimeSyncDirector,
    BirdPlayerGlideCtrl,
    CapManHeroDemoDirector,
    CapMessageDirector,
    CapMessageMoonNotifier,
    CheckpointFlagWatcher,
    CoinCollectHolder,
    CoinCollectWatcher,
    CollectBgmPlayer,
    alDataStoreDirector,
    EchoEmitterHolder,
    ElectricWireCameraTicketHolder,
    EventDemoCtrl,
    _e,
    alFootPrintServer,
    alFriendsProfileDownloader,
    FukankunZoomObjHolder,
    GameDataHolder,
    _13,
    GiantWanderBoss,
    GrowPlantDirector,
    GuidePosInfoHolder,
    HintPhotoLayoutHolder,
    HosuiTrailKeeper,
    HosuiWaterBallHolder,
    HtmlViewerRequester,
    InformationWindowDirector,
    _1c,
    KidsModeLayoutAccessor,
    KoopaStopHackCtrl,
    KoopaShipDemoRequester,
    alLayoutTextureRenderer,
    LifeUpItemGenerateWatcher,
    LoginLotteryDirector,
    MapLayout,
    MoviePlayer,
    _25,
    NpcEventCtrlInfo,
    PaintObjHolder,
    _28,
    PeachOnKoopaAnimRequester,
    PlayerStartInfoHolder,
    PlayerHackStartTexKeeper,
    QuestInfoHolder,
    ProjectRaceCheckPointHolder,
    RaceManRace,
    RaceWatcher,
    RadiConRaceWatcher,
    RandomItemSelector,
    alRankingDirector,
    RankingNameHolder,
    ReactionObjectActionIndexHolder,
    RhyhtmInfoWatcher,
    RocketFlowerEquipWatcher,
    RouteGuideDirector,
    SceneEventNotifier,
    SessionMusicianManager,
    ShineChipWatcherHolder,
    SphinxQuizRouteKillExecutor,
    alStageSyncCounter,
    _3d,  // seems like it was meant to be al::registSupportFreezeSyncGroup (based on
          // ClockMapParts::init + 0x6c),
    TalkNpcParamHolder,
    TalkNpcSceneEventSwitcher,
    TestStageTimeDirector,
    TimeBalloonDirector,
    _42,
    alTrafficAreaDirector,
    TrafficRailWatcher,
    TRexScrollBreakMapPartsBreakJudge,
    TsukkunTraceHolder,
    WipeHolderRequester,
    YoshiFruitWatcher,
    HelpAmiiboDirector,

    Max,
};

class SceneObjFactory {
public:
    static al::SceneObjHolder* createSceneObjHolder();
};
