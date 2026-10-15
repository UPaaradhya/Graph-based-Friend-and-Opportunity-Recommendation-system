#include "OpportunityService.h"
#include <iostream>
#include <string>

using namespace std;

// Adds escape characters so text can be sent safely in a JSON request.
static string escapeText(const string& text)
{
    string result = "";

    for (int i = 0; i < (int)text.length(); i++)
    {
        if (text[i] == '\\' || text[i] == '"')
        {
            result += '\\';
        }

        if (text[i] != '\n' && text[i] != '\r')
        {
            result += text[i];
        }
    }

    return result;
}

vector<Opportunity> OpportunityService::all()
{
    vector<Opportunity> opportunities;
    vector<simple_json::ptree> rows;

    rows = db.query(
        "MATCH (o:Opportunity) "
        "RETURN o.id,o.name,o.type,o.skill,o.interest ORDER BY o.id");

    for (int i = 0; i < (int)rows.size(); i++)
    {
        Opportunity opportunity;
        simple_json::ptree::const_iterator item = rows[i].begin();

        if (item == rows[i].end())
        {
            continue;
        }

        opportunity.id = item->second.get_value<int>();
        item++;

        if (item != rows[i].end())
        {
            opportunity.name = item->second.get_value<string>();
            item++;
        }

        if (item != rows[i].end())
        {
            opportunity.type = item->second.get_value<string>();
            item++;
        }

        if (item != rows[i].end())
        {
            opportunity.skill = item->second.get_value<string>();
            item++;
        }

        if (item != rows[i].end())
        {
            opportunity.interest = item->second.get_value<string>();
        }

        opportunities.push_back(opportunity);
    }

    return opportunities;
}

bool OpportunityService::addOpportunity()
{
    string name;
    string type;
    string skill;
    string interest;

    cout << "Opportunity name (use underscores for spaces): ";
    cin >> name;

    cout << "Type (Project/Workshop/Course): ";
    cin >> type;

    if (type != "Project" && type != "Workshop" && type != "Course")
    {
        cout << "Invalid opportunity type.\n";
        return false;
    }

    cout << "Required skill: ";
    cin >> skill;

    cout << "Interest area: ";
    cin >> interest;

    vector<simple_json::ptree> idRows;
    idRows = db.query(
        "MATCH (o:Opportunity) RETURN coalesce(max(o.id),0)+1");

    int id = 1;
    if (!idRows.empty())
    {
        try
        {
            id = idRows[0].begin()->second.get_value<int>();
        }
        catch (...)
        {
            id = 1;
        }
    }

    string parameters = "{\"id\":" + to_string(id)
        + ",\"name\":\"" + escapeText(name)
        + "\",\"type\":\"" + escapeText(type)
        + "\",\"skill\":\"" + escapeText(skill)
        + "\",\"interest\":\"" + escapeText(interest) + "\"}";

    bool saved = db.execute(
        "CREATE (o:Opportunity {id:$id,name:$name,type:$type,skill:$skill,interest:$interest})",
        parameters);

    if (!saved)
    {
        cout << "Could not save opportunity: " << db.lastError() << "\n";
        return false;
    }

    cout << "Opportunity saved with ID " << id << ".\n";
    return true;
}
