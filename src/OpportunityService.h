#pragma once

#include "Models.h"
#include "Neo4jDatabase.h"
#include <vector>

class OpportunityService
{
public:
    explicit OpportunityService(Neo4jDatabase& database) : db(database) {}

    bool addOpportunity();
    std::vector<Opportunity> all();

private:
    Neo4jDatabase& db;
};
