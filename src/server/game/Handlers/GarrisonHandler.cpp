#include "Common.h"
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "MapManager.h"
#include "Garrison.h"

void WorldSession::HandleGetGarrisonInfoOpcode(WorldPacket & p_RecvData)
{
    /// TEMP
    if (!m_Player->GetGarrison())
        m_Player->CreateGarrison();
    //////////////////////////////////////////////////////////////////////////

    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;
    l_Garrison->AddMission(44);
    
    std::vector<GarrisonPlotInstanceInfoLocation>   l_Plots             = l_Garrison->GetPlots();
    std::vector<GarrisonMission>                    l_CompletedMission  = l_Garrison->GetCompletedMissions();
    std::vector<GarrisonMission>                    l_Missions          = l_Garrison->GetMissions();

    WorldPacket l_Infos(SMSG_GET_GARRISON_INFO_RESULT, 200);

    l_Infos << int32(l_Garrison->GetGarrisonSiteLevelEntry()->SiteID);          ///< Site ID
    l_Infos << int32(l_Garrison->GetGarrisonSiteLevelEntry()->SiteLevelID);     ///< Site Level ID
    l_Infos << int32(l_Garrison->GetGarrisonFactionIndex());                    ///< Faction Index
    
    l_Infos << uint32(0);
    l_Infos << uint32(l_Plots.size());   ///< JamGarrisonPlotInfo Count
    l_Infos << uint32(0);
    l_Infos << uint32(l_Missions.size());
    l_Infos << uint32(l_CompletedMission.size());

    for (uint32 l_I = 0; l_I < l_Plots.size(); ++l_I)
    {
        l_Infos << int32(l_Plots[l_I].PlotInstanceID);
        l_Infos << float(l_Plots[l_I].X);
        l_Infos << float(l_Plots[l_I].Y);
        l_Infos << float(l_Plots[l_I].Z);
        l_Infos << float(l_Plots[l_I].O);
        l_Infos << uint32(l_Garrison->GetPlotType(l_Plots[l_I].PlotInstanceID));
    }

    for (uint32 l_I = 0; l_I < l_Missions.size(); ++l_I)
    {
        uint32 l_TravelDuration     = 0;
        uint32 l_MissionDuration    = 0;

        if (l_Missions[l_I].State == GARRISON_MISSION_IN_PROGRESS && sGarrMissionStore.LookupEntry(l_Missions[l_I].MissionID))
        {
            l_TravelDuration    = sGarrMissionStore.LookupEntry(l_Missions[l_I].MissionID)->TravelDuration;
            l_MissionDuration   = sGarrMissionStore.LookupEntry(l_Missions[l_I].MissionID)->MissionDuration;
        }

        l_Infos << uint64(l_Missions[l_I].DB_ID);
        l_Infos << uint32(l_Missions[l_I].MissionID);
        l_Infos << uint32(l_Missions[l_I].OfferTime);
        l_Infos << uint32(l_Missions[l_I].OfferMaxDuration);
        l_Infos << uint32(l_Missions[l_I].StartTime);
        l_Infos << uint32(l_TravelDuration);
        l_Infos << uint32(l_MissionDuration);
        l_Infos << uint32(l_Missions[l_I].State);
    }

    for (uint32 l_I = 0; l_I < l_CompletedMission.size(); ++l_I)
        l_Infos << int32(l_CompletedMission[l_I].MissionID);

    SendPacket(&l_Infos);
}
void WorldSession::HandleRequestGarrisonUpgradeableOpcode(WorldPacket & p_RecvData)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    bool l_CanUpgrade = false;

    WorldPacket l_Data(SMSG_GARRISON_REQUEST_UPGRADEABLE_RESULT, 4);

    l_Data << uint32(!l_CanUpgrade);

    SendPacket(&l_Data);
}
void WorldSession::HandleRequestLandingPageShipmentInfoOpcode(WorldPacket & p_RecvData)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

}
void WorldSession::HandleGarrisonMissionNPCHelloOpcode(WorldPacket & p_RecvData)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_MISSION_NPC);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonMissionNPCHelloOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    SendGarrisonOpenMissionNpc(l_NpcGUID);
}
void WorldSession::HandleGarrisonRequestBlueprintAndSpecializationDataOpcode(WorldPacket & p_RecvData)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    std::vector<int32> l_KnownBlueprints        = l_Garrison->GetKnownBlueprints();
    std::vector<int32> l_KnownSpecializations   = l_Garrison->GetKnownSpecializations();

    WorldPacket l_Data(SMSG_GARRISON_REQUEST_BLUEPRINT_AND_SPECIALIZATION_DATA_RESULT, 200);

    l_Data << uint32(l_KnownBlueprints.size());
    l_Data << uint32(l_KnownSpecializations.size());

    for (uint32 l_I = 0; l_I < l_KnownBlueprints.size(); ++l_I)
        l_Data << int32(l_KnownBlueprints[l_I]);

    for (uint32 l_I = 0; l_I < l_KnownSpecializations.size(); ++l_I)
        l_Data << int32(l_KnownSpecializations[l_I]);

    SendPacket(&l_Data);
}
void WorldSession::HandleGarrisonPurchaseBuildingOpcode(WorldPacket & p_RecvData)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID = 0;

    uint32 l_BuildingID     = 0;
    uint32 l_PlotInstanceID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_PlotInstanceID;
    p_RecvData >> l_BuildingID;

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_ARCHITECT);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonPurchaseBuildingOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    GarrisonPurchaseBuildingResult l_Result = GARRISON_PURCHASE_BUILDING_OK;

    if (!sGarrBuildingStore.LookupEntry(l_BuildingID))
        l_Result = GARRISON_PURCHASE_BUILDING_INVALID_BUILDING_ID;

    if (!l_Result && !sGarrPlotInstanceStore.LookupEntry(l_PlotInstanceID))
        l_Result = GARRISON_PURCHASE_BUILDING_INVALID_PLOT;

    if (!l_Result && !l_Garrison->KnownBlueprint(l_BuildingID))
        l_Result = GARRISON_PURCHASE_BUILDING_REQUIRE_BLUE_PRINT;

    if (!l_Result && !l_Garrison->PlotIsFree(l_PlotInstanceID))
        l_Result = GARRISON_PURCHASE_BUILDING_BUILDING_EXIST;

    if (!l_Result && !l_Garrison->IsBuildingPlotInstanceValid(l_BuildingID, l_PlotInstanceID))
        l_Result = GARRISON_PURCHASE_BUILDING_INVALID_PLOT_BUILDING;

    if (!l_Result)
        l_Result = l_Garrison->CanPurchaseBuilding(l_BuildingID);

    WorldPacket l_PlaceResult(SMSG_GARRISON_PLACE_BUILDING_RESULT, 26);
    l_PlaceResult << uint32(l_Result);

    if (l_Result == GARRISON_PURCHASE_BUILDING_OK)
    {
        GarrisonBuilding l_Building = l_Garrison->PurchaseBuilding(l_BuildingID, l_PlotInstanceID);

        l_PlaceResult << uint32(l_PlotInstanceID);
        l_PlaceResult << uint32(l_BuildingID);
        l_PlaceResult << uint32(l_Building.TimeBuiltStart);
        l_PlaceResult << uint32(l_Building.SpecID);
        l_PlaceResult << uint32(l_Building.TimeBuiltEnd);
        l_PlaceResult.WriteBit(l_Building.Active);
        l_PlaceResult.FlushBits();
    }
    else
    {
        l_PlaceResult << uint32(0);
        l_PlaceResult << uint32(0);
        l_PlaceResult << uint32(0);
        l_PlaceResult << uint32(0);
        l_PlaceResult << uint32(0);
        l_PlaceResult.WriteBit(false);
        l_PlaceResult.FlushBits();
    }

    l_PlaceResult.WriteBit(false);                      ///< Unk bit
    l_PlaceResult.FlushBits();

    SendPacket(&l_PlaceResult);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void WorldSession::SendGarrisonOpenArchitect(uint64 p_CreatureGUID)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    WorldPacket l_Data(SMSG_GARRISON_OPEN_ARCHITECT, 18);

    l_Data.appendPackGUID(p_CreatureGUID);

    SendPacket(&l_Data);
}
void WorldSession::SendGarrisonOpenMissionNpc(uint64 p_CreatureGUID)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    WorldPacket l_Data(SMSG_GARRISON_OPEN_MISSION_NPC, 18);

    l_Data.appendPackGUID(p_CreatureGUID);

    SendPacket(&l_Data);
}