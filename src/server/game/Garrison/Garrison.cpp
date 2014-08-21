#include "Garrison.h"
#include "Player.h"

uint32 gGarrisonEmptyPlotGameObject[GARRISON_PLOT_TYPE_MAX] = 
{
    229501,     ///< GARRISON_PLOT_TYPE_SMALL
    0,          ///< GARRISON_PLOT_TYPE_MEDIUM
    232143      ///< GARRISON_PLOT_TYPE_LARGE
};

uint32 gGarrisonBuildingPlotGameObject[GARRISON_PLOT_TYPE_MAX] =
{
    233957,     ///< GARRISON_PLOT_TYPE_SMALL
    0,          ///< GARRISON_PLOT_TYPE_MEDIUM
    0           ///< GARRISON_PLOT_TYPE_LARGE
};

GarrisonPlotInstanceInfoLocation gGarrisonPlotInstanceInfoLocation[GARRISON_PLOT_INSTANCE_COUNT] = {
    /// SiteLevelID PlotInstanceID      X            Y            Z           O
    /// Alliance Level 1                                                    
    {       5,          19,         1829.896f,    197.5504f,    72.00920f,   1.884956f   },
    {       5,          23,         1911.550f,    232.9792f,    76.65489f,   2.795270f   },
    /// Alliance Level 2                                                    
    {     444,          18,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     444,          19,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     444,          22,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     444,          23,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     444,          59,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     444,          63,            0.0f,        0.0f,        0.0f,       0.0f        },
    /// Alliance Level 3                                                  
    {       6,          18,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          19,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          20,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          22,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          23,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          24,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          25,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          59,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          63,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          67,            0.0f,        0.0f,        0.0f,       0.0f        },
    {       6,          81,            0.0f,        0.0f,        0.0f,       0.0f        },
    /// Horde Level 1                                                       
    {     258,          18,         5645.124f,   4508.960f,    119.2701f,    2.0423500f  },
    {     258,          23,         5575.461f,   4459.338f,    130.3681f,    0.9599311f  },
    /// Horde level 2                                                       
    {     445,          18,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          19,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          22,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          23,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          59,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          63,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     445,          67,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     444,          67,            0.0f,        0.0f,        0.0f,       0.0f        },
    /// Horde level 3                                                      
    {     259,          18,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          19,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          20,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          22,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          23,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          24,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          25,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          59,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          63,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          67,            0.0f,        0.0f,        0.0f,       0.0f        },
    {     259,          81,            0.0f,        0.0f,        0.0f,       0.0f        }
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Constructor
Garrison::Garrison(Player * p_Owner)
    : m_Owner(p_Owner)
{
    m_GarrisonLevel     = 1;
    m_GarrisonLevelID   = 0;
    m_GarrisonSiteID    = 0;

    /// Select Garrison site ID
    switch (GetGarrisonFactionIndex())
    {
        case GARRISON_FACTION_ALLIANCE:
            m_GarrisonSiteID = 2;
            break;

        case GARRISON_FACTION_HORDE:
            m_GarrisonSiteID = 71;
            break;
    }

    InitDataForLevel();
    InitGameObjects();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Save this garrison to DB
void Garrison::Save()
{

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get GarrSiteLevelEntry for current garrison
const GarrSiteLevelEntry * Garrison::GetGarrisonSiteLevelEntry()
{
    return sGarrSiteLevelStore.LookupEntry(m_GarrisonLevelID);
}
/// Get Garrison Faction Index
GarrisonFactionIndex Garrison::GetGarrisonFactionIndex()
{
    assert(m_Owner);
    
    switch (m_Owner->GetTeam())
    {
        case ALLIANCE:
            return GARRISON_FACTION_ALLIANCE;

        case HORDE:
            return GARRISON_FACTION_HORDE;
    }

    return GARRISON_FACTION_HORDE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get slots for level
std::vector<GarrisonPlotInstanceInfoLocation> Garrison::GetPlots()
{
    return m_Plots;
}
/// Get plot instance plot type
uint32 Garrison::GetPlotType(uint32 p_PlotInstanceID)
{
    const GarrPlotInstanceEntry * l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(p_PlotInstanceID);

    if (!l_PlotInstanceEntry)
        return 0;

    const GarrPlotEntry * l_PlotEntry = sGarrPlotStore.LookupEntry(l_PlotInstanceEntry->PlotID);

    if (!l_PlotEntry)
        return 0;

    const GarrPlotUICategoryEntry * l_PlotUICategoryEntry = sGarrPlotUICategoryStore.LookupEntry(l_PlotEntry->PlotUiCategoryID);

    if (!l_PlotUICategoryEntry)
        return 0;

    return l_PlotUICategoryEntry->Type;
}
/// Plot is free ?
bool Garrison::PlotIsFree(uint32 p_PlotInstanceID)
{
    for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
            return false;

    return true;
}
/// Get plot location
GarrisonPlotInstanceInfoLocation Garrison::GetPlot(uint32 p_PlotInstanceID)
{
    for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
    {
        if (m_Plots[l_I].SiteLevelID == m_GarrisonLevelID && m_Plots[l_I].PlotInstanceID == p_PlotInstanceID)
            return m_Plots[l_I];
    }

    return GarrisonPlotInstanceInfoLocation();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Add mission
bool Garrison::AddMission(uint32 p_MissionRecID)
{
    const GarrMissionEntry * l_MissionEntry = sGarrMissionStore.LookupEntry(p_MissionRecID);

    if (!l_MissionEntry)
        return false;

    if (HaveMission(p_MissionRecID))
        return false;

    if (l_MissionEntry->RequiredLevel > m_Owner->getLevel())
        return false;

    if (l_MissionEntry->RequiredItemLevel > m_Owner->GetAverageItemLevel())
        return false;

    GarrisonMission l_Mission;

    l_Mission.MissionID         = p_MissionRecID;
    l_Mission.OfferTime         = time(0);
    l_Mission.OfferMaxDuration  = l_MissionEntry->OfferDuration;
    l_Mission.State             = GARRISON_MISSION_AVAILABLE;
    l_Mission.StartTime         = 0;

    m_Missions.push_back(l_Mission);

    return true;
}
/// Player have mission
bool Garrison::HaveMission(uint32 p_MissionRecID)
{
    for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        if (m_Missions[l_I].MissionID == p_MissionRecID)
            return true;

    return false;
}
/// Get missions
std::vector<GarrisonMission> Garrison::GetMissions()
{
    std::vector<GarrisonMission> l_Result;

    for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        if (m_Missions[l_I].State != GARRISON_MISSION_COMPLETE)
            l_Result.push_back(m_Missions[l_I]);

    return l_Result;
}
/// Get all completed missions
std::vector<GarrisonMission> Garrison::GetCompletedMissions()
{
    std::vector<GarrisonMission> l_Result;

    for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        if (m_Missions[l_I].State == GARRISON_MISSION_COMPLETE)
            l_Result.push_back(m_Missions[l_I]);

    return l_Result;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Can build building X at slot instance Y
bool Garrison::IsBuildingPlotInstanceValid(uint32 p_BuildingRecID, uint32 p_PlotInstanceID)
{
    const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

    if (!l_BuildingEntry)
        return false;

    const GarrPlotInstanceEntry * l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(p_PlotInstanceID);

    if (!l_PlotInstanceEntry)
        return false;

    /// Search building plot ID 
    for (uint32 l_I = 0; l_I < sGarrPlotBuildingStore.GetNumRows(); ++l_I)
    {
        const GarrPlotBuildingEntry * l_PlotBuildingEntry = sGarrPlotBuildingStore.LookupEntry(l_I);

        if (l_PlotBuildingEntry && l_PlotBuildingEntry->PlotId == l_PlotInstanceEntry->PlotID && l_PlotBuildingEntry->BuildingID == p_BuildingRecID)
            return true;
    }

    return false;
}
/// Player fill all condition
GarrisonPurchaseBuildingResult Garrison::CanPurchaseBuilding(uint32 p_BuildingRecID)
{
    const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

    if (!l_BuildingEntry)
        return GARRISON_PURCHASE_BUILDING_INVALID_BUILDING_ID;

    if (l_BuildingEntry->BuildCostCurrencyID != 0)
    {
        if (!m_Owner->HasCurrency(l_BuildingEntry->BuildCostCurrencyID, l_BuildingEntry->BuildCostCurrencyAmount))
            return GARRISON_PURCHASE_BUILDING_NOT_ENOUGH_CURRENCY;
    }

    return GARRISON_PURCHASE_BUILDING_OK;
}
/// PurchaseBuilding
GarrisonBuilding Garrison::PurchaseBuilding(uint32 p_BuildingRecID, uint32 p_PlotInstanceID)
{
    const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

    GarrisonBuilding l_Building;

    memset(&l_Building, 0, sizeof(l_Building));

    if (!l_BuildingEntry)
        return l_Building;

    if (l_BuildingEntry->BuildCostCurrencyID != 0)
        m_Owner->ModifyCurrency(l_BuildingEntry->BuildCostCurrencyID, -(int32)l_BuildingEntry->BuildCostCurrencyAmount);

    WorldPacket l_PlotRemoved(SMSG_GARRISON_PLOT_REMOVED, 4);
    l_PlotRemoved << uint32(p_PlotInstanceID);
    m_Owner->SendDirectMessage(&l_PlotRemoved);

    l_Building.BuildingID       = p_BuildingRecID;
    l_Building.PlotInstanceID   = p_PlotInstanceID;
    l_Building.TimeBuiltStart   = time(0);
    l_Building.TimeBuiltEnd     = time(0) + l_BuildingEntry->BuildTime;           ///< 5/5/1905 18:45:05
    l_Building.SpecID           = 0;
    l_Building.Active           = false;

    m_Buildings.push_back(l_Building);

    UpdatePlotGameObject(p_PlotInstanceID);
    
    return l_Building;
}
/// Get building
GarrisonBuilding Garrison::GetBuilding(uint32 p_PlotInstanceID)
{
    for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
            return m_Buildings[l_I];
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get known blueprints
std::vector<int32> Garrison::GetKnownBlueprints()
{
    return m_KnownBlueprints;
}
/// Learn blue print
bool Garrison::LearnBlueprint(uint32 p_BuildingRecID)
{
    GarrisonLearnBluePrintResult l_ResultCode;

    if (std::find(m_KnownBlueprints.begin(), m_KnownBlueprints.end(), p_BuildingRecID) == m_KnownBlueprints.end())
    {
        const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

        if (l_BuildingEntry)
        {
            m_KnownBlueprints.push_back(p_BuildingRecID);
            l_ResultCode = GARRISON_LEARN_BLUEPRINT_LEARNED;
        }
        else
        {
            l_ResultCode = GARRISON_LEARN_BLUEPRINT_UNABLE_TO_LEARN;
        }
    }
    else
    {
        l_ResultCode = GARRISON_LEARN_BLUEPRINT_KNOWN;
    }

    WorldPacket l_Result(SMSG_GARRISON_LEARN_BLUEPRINT_RESULT, 8);
    l_Result << uint32(l_ResultCode);
    l_Result << uint32(p_BuildingRecID);

    m_Owner->SendDirectMessage(&l_Result);

    return true;
}
/// Known blue print
bool Garrison::KnownBlueprint(uint32 p_BuildingRecID)
{
    return std::find(m_KnownBlueprints.begin(), m_KnownBlueprints.end(), p_BuildingRecID) != m_KnownBlueprints.end();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get known specializations
std::vector<int32> Garrison::GetKnownSpecializations()
{
    return m_KnownSpecializations;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Init data for level
void Garrison::InitDataForLevel()
{
    const GarrSiteLevelEntry * l_SiteEntry = nullptr;

    /// Search garrison site entry by SideID & Level
    for (uint32 l_I = 0; l_I < sGarrSiteLevelStore.GetNumRows(); ++l_I)
    {
        const GarrSiteLevelEntry * l_CurrentSiteEntry = sGarrSiteLevelStore.LookupEntry(l_I);

        if (l_CurrentSiteEntry && l_CurrentSiteEntry->Level == m_GarrisonLevel && l_CurrentSiteEntry->SiteID == m_GarrisonSiteID)
        {
            l_SiteEntry = l_CurrentSiteEntry;
            break;
        }
    }

    if (!l_SiteEntry)
        assert(false && "Garrison::InitDataForLevel() not data found");

    m_GarrisonLevelID = l_SiteEntry->SiteLevelID;

    /// Find all plots for garrison level
    m_Plots.clear();

    for (uint32 l_I = 0; l_I < GARRISON_PLOT_INSTANCE_COUNT; ++l_I)
    {
        if (gGarrisonPlotInstanceInfoLocation[l_I].SiteLevelID == m_GarrisonLevelID)
            m_Plots.push_back(gGarrisonPlotInstanceInfoLocation[l_I]);
    }

    m_KnownBlueprints.push_back(26);
    m_KnownBlueprints.push_back(76);
}
/// Init Game objects
void Garrison::InitGameObjects()
{
    for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
    {
        UpdatePlotGameObject(m_Plots[l_I].PlotInstanceID);
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Update plot gameobject
void Garrison::UpdatePlotGameObject(uint32 p_PlotInstanceID)
{
    GarrisonPlotInstanceInfoLocation l_PlotInfo = GetPlot(p_PlotInstanceID);

    if (m_PlotsGob[p_PlotInstanceID] != 0)
    {
        GameObject * l_Gob = sObjectAccessor->GetGameObjects().at(m_PlotsGob[p_PlotInstanceID]);

        if (l_Gob)
        {
            l_Gob->SetRespawnTime(0);
            l_Gob->Delete();
        }
    }


    if (PlotIsFree(p_PlotInstanceID))
    {
        GameObject * l_Gob = m_Owner->SummonGameObject(gGarrisonEmptyPlotGameObject[GetPlotType(p_PlotInstanceID)], l_PlotInfo.X, l_PlotInfo.Y, l_PlotInfo.Z, l_PlotInfo.O, 0, 0, 0, 0, 0);
        m_PlotsGob[p_PlotInstanceID] = l_Gob->GetGUID();
    }
    else
    {
        GarrisonBuilding l_Building = GetBuilding(p_PlotInstanceID);

        if (l_Building.TimeBuiltEnd > time(0))
        {
            GameObject * l_Gob = m_Owner->SummonGameObject(gGarrisonBuildingPlotGameObject[GetPlotType(p_PlotInstanceID)], l_PlotInfo.X, l_PlotInfo.Y, l_PlotInfo.Z, l_PlotInfo.O, 0, 0, 0, 0, 0);
            m_PlotsGob[p_PlotInstanceID] = l_Gob->GetGUID();
        }
        else
        {
            const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_Building.BuildingID);

            if (!l_BuildingEntry)
                return;

            GameObject * l_Gob = m_Owner->SummonGameObject(l_BuildingEntry->GameObjects[GetGarrisonFactionIndex()], l_PlotInfo.X, l_PlotInfo.Y, l_PlotInfo.Z, l_PlotInfo.O, 0, 0, 0, 0, 0);
            m_PlotsGob[p_PlotInstanceID] = l_Gob->GetGUID();
        }
    }
}