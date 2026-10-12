#pragma once

#include "Models.h"
#include "Neo4jDatabase.h"
#include "GraphManager.h"
#include "OpportunityService.h"

class RecommendationService
{
public:
    RecommendationService(Neo4jDatabase& database,
                         GraphManager& graphManager,
                         OpportunityService& opportunityService)
        : db(database), graph(graphManager), opportunities(opportunityService) {}

    void recommendFriends(int userId);
    void recommendOpportunities(int userId);

private:
    Neo4jDatabase& db;
    GraphManager& graph;
    OpportunityService& opportunities;
};
