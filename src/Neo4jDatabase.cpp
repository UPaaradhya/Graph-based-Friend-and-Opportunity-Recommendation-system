#include "Neo4jDatabase.h"
#include <windows.h>
#include <winhttp.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using boost::property_tree::ptree;

// Escapes special characters in a Cypher query before adding it to JSON.
static string escapeJson(const string& text)
{
    string result = "";

    for (int i = 0; i < (int)text.length(); i++)
    {
        char character = text[i];

        if (character == '"')
        {
            result += "\\\"";
        }
        else if (character == '\\')
        {
            result += "\\\\";
        }
        else if (character == '\n')
        {
            result += "\\n";
        }
        else if (character == '\r')
        {
            result += "\\r";
        }
        else if (character == '\t')
        {
            result += "\\t";
        }
        else
        {
            result += character;
        }
    }

    return result;
}

Neo4jDatabase::Neo4jDatabase()
{
    uri = "http://localhost:7474/db/neo4j/tx/commit";
    username = "neo4j";
    password = "password";
    database = "neo4j";

    // Read the connection settings from config/neo4j.conf.
    ifstream file("config/neo4j.conf");
    string line;

    while (getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        size_t equalPosition = line.find('=');
        if (equalPosition == string::npos)
        {
            continue;
        }

        string key = line.substr(0, equalPosition);
        string value = line.substr(equalPosition + 1);

        if (key == "uri")
        {
            uri = value;
        }
        else if (key == "username")
        {
            username = value;
        }
        else if (key == "password")
        {
            password = value;
        }
        else if (key == "database")
        {
            database = value;
        }
    }

    // Add the database path if only the server address was supplied.
    if (uri.find("/db/") == string::npos)
    {
        uri = uri + "/db/" + database + "/tx/commit";
    }
}

bool Neo4jDatabase::connect()
{
    string response = request(
        "{\"statements\":[{\"statement\":\"RETURN 1 AS ok\"}]}");

    return !response.empty();
}

// Uses Windows' built-in WinHTTP API, so no libcurl installation is needed.
static std::wstring toWide(const std::string& value)
{
    if (value.empty()) return L"";
    int size = MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, NULL, 0);
    std::wstring result(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, &result[0], size);
    if (!result.empty()) result.pop_back();
    return result;
}

static std::string base64Encode(const std::string& input)
{
    const char* table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out; unsigned int val=0; int bits=-6;
    for (unsigned char c : input) { val=(val<<8)+c; bits+=8; while(bits>=0){out.push_back(table[(val>>bits)&0x3F]);bits-=6;} }
    if(bits>-6) out.push_back(table[((val<<8)>>(bits+8))&0x3F]);
    while(out.size()%4) out.push_back('=');
    return out;
}

string Neo4jDatabase::request(const string& body)
{
    URL_COMPONENTS parts{};
    parts.dwStructSize = sizeof(parts);
    parts.dwSchemeLength = parts.dwHostNameLength = parts.dwUrlPathLength = parts.dwExtraInfoLength = (DWORD)-1;
    std::wstring wideUri = toWide(uri);
    if (!WinHttpCrackUrl(wideUri.c_str(), 0, 0, &parts)) { error = "Invalid Neo4j URL."; return ""; }
    std::wstring host(parts.lpszHostName, parts.dwHostNameLength);
    std::wstring path(parts.lpszUrlPath, parts.dwUrlPathLength);
    if (parts.dwExtraInfoLength) path += std::wstring(parts.lpszExtraInfo, parts.dwExtraInfoLength);
    HINTERNET session = WinHttpOpen(L"FriendOpportunitySystem/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!session) { error = "Could not start Windows HTTP connection."; return ""; }
    HINTERNET connection = WinHttpConnect(session, host.c_str(), parts.nPort, 0);
    HINTERNET requestHandle = connection ? WinHttpOpenRequest(connection, L"POST", path.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, parts.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0) : NULL;
    if (!requestHandle) { error = "Could not open a connection to Neo4j. Check that Neo4j is running."; if(connection)WinHttpCloseHandle(connection);WinHttpCloseHandle(session);return ""; }
    std::string auth = "Authorization: Basic " + base64Encode(username + ":" + password);
    std::wstring headers = L"Content-Type: application/json\r\n" + toWide(auth) + L"\r\n";
    BOOL sent = WinHttpSendRequest(requestHandle, headers.c_str(), (DWORD)-1, (LPVOID)body.data(), (DWORD)body.size(), (DWORD)body.size(), 0);
    if (!sent || !WinHttpReceiveResponse(requestHandle, NULL)) { error = "Could not receive a response from Neo4j. Check the URL and database status."; WinHttpCloseHandle(requestHandle);WinHttpCloseHandle(connection);WinHttpCloseHandle(session);return ""; }
    DWORD status=0, statusSize=sizeof(status); WinHttpQueryHeaders(requestHandle, WINHTTP_QUERY_STATUS_CODE|WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &status, &statusSize, WINHTTP_NO_HEADER_INDEX);
    std::string response; DWORD available=0;
    do { available=0; if(!WinHttpQueryDataAvailable(requestHandle,&available))break; if(available){std::vector<char> buffer(available);DWORD read=0;if(WinHttpReadData(requestHandle,buffer.data(),available,&read))response.append(buffer.data(),read);else break;} } while(available>0);
    WinHttpCloseHandle(requestHandle);WinHttpCloseHandle(connection);WinHttpCloseHandle(session);
    if(status<200||status>=300){error="HTTP status "+std::to_string(status)+": "+response;return "";}
    try { std::stringstream input(response); ptree root; boost::property_tree::read_json(input,root); auto errors=root.get_child_optional("errors"); if(errors && !errors.get().empty()){error=errors.get().front().second.get<std::string>("message", "Neo4j query error");return "";} }
    catch(const std::exception& problem){error=std::string("Could not read Neo4j response: ")+problem.what();return "";}
    return response;
}

bool Neo4jDatabase::execute(const string& cypher, const string& parameters)
{
    string body = "{\"statements\":[{\"statement\":\"";
    body += escapeJson(cypher);
    body += "\",\"parameters\":" + parameters + "}]}";

    string response = request(body);
    return !response.empty();
}

vector<ptree> Neo4jDatabase::query(const string& cypher, const string& parameters)
{
    vector<ptree> rows;
    string body = "{\"statements\":[{\"statement\":\"";
    body += escapeJson(cypher);
    body += "\",\"parameters\":" + parameters + "}]}";

    string response = request(body);
    if (response.empty())
    {
        return rows;
    }

    try
    {
        stringstream input(response);
        ptree root;
        boost::property_tree::read_json(input, root);

        ptree& results = root.get_child("results");
        for (ptree::iterator result = results.begin(); result != results.end(); result++)
        {
            ptree& dataRows = result->second.get_child("data");

            for (ptree::iterator row = dataRows.begin(); row != dataRows.end(); row++)
            {
                boost::optional<ptree&> values = row->second.get_child_optional("row");
                if (values)
                {
                    rows.push_back(values.get());
                }
            }
        }
    }
    catch (const exception& problem)
    {
        error = problem.what();
    }

    return rows;
}

bool Neo4jDatabase::initialize()
{
    bool usersReady = execute(
        "CREATE CONSTRAINT user_id_unique IF NOT EXISTS "
        "FOR (u:User) REQUIRE u.id IS UNIQUE");

    if (!usersReady)
    {
        return false;
    }

    bool opportunitiesReady = execute(
        "CREATE CONSTRAINT opportunity_id_unique IF NOT EXISTS "
        "FOR (o:Opportunity) REQUIRE o.id IS UNIQUE");

    return opportunitiesReady;
}
