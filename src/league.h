#include <string>
#include <vector>
using namespace std;

// ==========================================
// 1. DATA STRUCTURES
// ==========================================

struct ConfigLeague {
    string name;
    int VictoryPoints = 0;
    int DrawPoints = 0;
    int LosingPoints = 0;
    vector<string> teams;
};

struct Match {
    string Date;
    string LocalTeam;
    string VisitingTeam;
    int LocalTeamGoals = 0;
    int VisitingTeamGoals = 0;
};

struct Team {
    string TeamName;
    int playedGames = 0;
    int WinGames = 0;
    int DrawGames = 0;
    int LostGames = 0;
    int GoalsScored = 0;
    int GoalsConceded = 0;
    int GoalDifference = 0;
    int points = 0;
};

// ==========================================
// 2. FUNCTION PROTOTYPES
// ==========================================
// File reading functions
ConfigLeague readConfig(string fileName);
vector<Match> readMatches(string fileName);
void showMatchdaysHistory(string fileName);

// File writing functions
void saveMatch(Match newMatch, string fileName);
void saveMatchday(int matchdayNumber, string date, vector<Match> matches, string fileName);
void saveTable(vector<Team> table, string fileName);

// League logic functions
void updateStats(Team* team, int goalsFor, int goalsAgainst, ConfigLeague config);
vector<Team> buildTable(vector<Match> matches, ConfigLeague config);
void sortTable(vector<Team>& table);
void showTable(vector<Team> table);
int getNextMatchdayNumber(string fileName);

// Program control
void runProgram(ConfigLeague config);
int showMenu(string leagueName);

// User input functions
Match enterMatch(ConfigLeague config);

// Display functions
void showAllMatches(vector<Match> matches);

//Export to csv
void exportMatchesToCSV(string txtFileName, string csvFileName);
void exportTableToCSV(vector<Team> table, string csvFileName);

//New Functions

void showSpecialStats(vector<Team> table, vector<Match> matches);
void resetStats(Team* team);
void resetAllStats(vector<Team>& table);