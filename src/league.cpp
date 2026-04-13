#include "league.h"
#include <fstream>
#include <iostream>
#include <sstream>


ConfigLeague readConfig(string fileName) {
    ConfigLeague config;
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo de configuracion" << "\n";
        return config;
    }

    string line;

    while (getline(file, line)) {
        if (line == "") {
            continue;
        }

        if (line[0] == '#') {
            continue;
        }

        int equalPosition = line.find('=');

        if (equalPosition == -1) {
            continue;
        }

        string key = line.substr(0, equalPosition);
        string value = line.substr(equalPosition + 1);

        if (key == "liga") {
            config.name = value;
        }
        else if (key == "victoria") {
            config.VictoryPoints = stoi(value);
        }
        else if (key == "empate") {
            config.DrawPoints = stoi(value);
        }
        else if (key == "derrota") {
            config.LosingPoints = stoi(value);
        }
        else if (key == "equipo") {
            config.teams.push_back(value);
        }
    }

    file.close();
    return config;
}

int showMenu(string leagueName) {
    int option;

    cout << "\n===== " << leagueName << " =====" << "\n";
    cout << "1. Ver tabla de posiciones" << "\n";
    cout << "2. Registrar resultado de un partido" << "\n";
    cout << "3. Ver historial de jornadas" << "\n";
    cout << "4. Ver todos los partidos jugados" << "\n";
    cout << "5. Exportar archivos a CSV" << "\n";
    cout << "6. Ver estadisticas especiales" << "\n";
    cout << "7. Reiniciar estadisticas de la tabla actual" << "\n";
    cout << "8. Salir" << "\n";
    cout << "Elige una opcion: ";
    cin >> option;

    return option;
}

void runProgram(ConfigLeague config) {
    int option = 0;

    while (option != 8) {
        option = showMenu(config.name);

        switch (option) {
            case 1: {
                vector<Match> matches = readMatches("data/partidos.txt");
                vector<Team> table = buildTable(matches, config);
                sortTable(table);
                showTable(table);
                saveTable(table, "data/tabla.txt");
                break;
            }

            case 2: {
                Match newMatch = enterMatch(config);
                saveMatch(newMatch, "data/partidos.txt");

                vector<Match> oneMatchday;
                oneMatchday.push_back(newMatch);

                int matchdayNumber = getNextMatchdayNumber("data/fechas.txt");
                saveMatchday(matchdayNumber, newMatch.Date, oneMatchday, "data/fechas.txt");

                cout << "Partido guardado" << "\n";
                break;
            }

            case 3:
                showMatchdaysHistory("data/fechas.txt");
                break;

            case 4: {
                vector<Match> matches = readMatches("data/partidos.txt");
                showAllMatches(matches);
                break;
            }

            case 5: {
                vector<Match> matches = readMatches("data/partidos.txt");
                vector<Team> table = buildTable(matches, config);
                sortTable(table);

                exportMatchesToCSV("data/partidos.txt", "data/partidos.csv");
                exportTableToCSV(table, "data/tabla.csv");

                cout << "Exportacion a CSV completa" << "\n";
                break;
            }

            case 6: {
                vector<Match> matches = readMatches("data/partidos.txt");
                vector<Team> table = buildTable(matches, config);
                sortTable(table);
                showSpecialStats(table, matches);
                break;
            }

            case 7: {
                vector<Match> matches = readMatches("data/partidos.txt");
                vector<Team> table = buildTable(matches, config);

                resetAllStats(table);
                showTable(table);
                break;
            }

            case 8:
                cout << "Saliendo del programa" << "\n";
                break;

            default:
                cout << "Opcion invalida" << "\n";
        }
    }
}

Match enterMatch(ConfigLeague config) {
    Match newMatch;
    int localOption;
    int visitingOption;

    cout << "\nEquipos disponibles:" << "\n";
    for (int i = 0; i < config.teams.size(); i++) {
        cout << i + 1 << ". " << config.teams[i] << "\n";
    }

    cout << "Ingresa la fecha del partido: ";
    cin >> newMatch.Date;

    cout << "Elige el numero del equipo local: ";
    cin >> localOption;

    while (localOption < 1 || localOption > config.teams.size()) {
        cout << "Equipo local invalido. Intenta de nuevo: ";
        cin >> localOption;
    }

    cout << "Elige el numero del equipo visitante: ";
    cin >> visitingOption;

    while (visitingOption < 1 || visitingOption > config.teams.size() || visitingOption == localOption) {
        cout << "Equipo visitante invalido. Intenta de nuevo: ";
        cin >> visitingOption;
    }

    newMatch.LocalTeam = config.teams[localOption - 1];
    newMatch.VisitingTeam = config.teams[visitingOption - 1];

    cout << "Ingresa los goles de " << newMatch.LocalTeam << ": ";
    cin >> newMatch.LocalTeamGoals;

    cout << "Ingresa los goles de " << newMatch.VisitingTeam << ": ";
    cin >> newMatch.VisitingTeamGoals;

    return newMatch;
}


vector<Match> readMatches(string fileName) {
    vector<Match> matches;
    ifstream file(fileName);

    if (file.is_open()) {
        Match match;
        char separator;

        while (getline(file, match.Date, ';')) {
            getline(file, match.LocalTeam, ';');
            getline(file, match.VisitingTeam, ';');
            file >> match.LocalTeamGoals;
            file >> separator;
            file >> match.VisitingTeamGoals;
            file.ignore();

            matches.push_back(match);
        }

        file.close();
    }
    else {
        cout << "No se pudo abrir el archivo de partidos." << "\n";
    }

    return matches;
}

void updateStats(Team* team, int goalsFor, int goalsAgainst, ConfigLeague config) {
    (*team).playedGames = (*team).playedGames + 1;
    (*team).GoalsScored = (*team).GoalsScored + goalsFor;
    (*team).GoalsConceded = (*team).GoalsConceded + goalsAgainst;
    (*team).GoalDifference = (*team).GoalsScored - (*team).GoalsConceded;

    if (goalsFor > goalsAgainst) {
        (*team).WinGames = (*team).WinGames + 1;
        (*team).points = (*team).points + config.VictoryPoints;
    }
    else if (goalsFor == goalsAgainst) {
        (*team).DrawGames = (*team).DrawGames + 1;
        (*team).points = (*team).points + config.DrawPoints;
    }
    else {
        (*team).LostGames = (*team).LostGames + 1;
        (*team).points = (*team).points + config.LosingPoints;
    }
}

vector<Team> buildTable(vector<Match> matches, ConfigLeague config) {
    vector<Team> table;

    for (int i = 0; i < config.teams.size(); i++) {
        Team newTeam;
        newTeam.TeamName = config.teams[i];
        table.push_back(newTeam);
    }

    for (int i = 0; i < matches.size(); i++) {
        for (int j = 0; j < table.size(); j++) {
            if (table[j].TeamName == matches[i].LocalTeam) {
                updateStats(&table[j], matches[i].LocalTeamGoals, matches[i].VisitingTeamGoals, config);
            }

            if (table[j].TeamName == matches[i].VisitingTeam) {
                updateStats(&table[j], matches[i].VisitingTeamGoals, matches[i].LocalTeamGoals, config);
            }
        }
    }

    return table;
}


void sortTable(vector<Team>& table) {
    for (int i = 0; i < table.size() - 1; i++) {
        for (int j = i + 1; j < table.size(); j++) {
            if (table[j].points > table[i].points) {
                Team temp = table[i];
                table[i] = table[j];
                table[j] = temp;
            }
        }
    }
}


void showTable(vector<Team> table) {
    cout << "\nTABLA DE POSICIONES" << "\n";
    cout << "-------------------------------------------------------------" << "\n";
    cout << "#  Equipo              PJ  PG  PE  PP  GF  GC  DG  PTS" << "\n";
    cout << "-------------------------------------------------------------" << "\n";

    for (int i = 0; i < table.size(); i++) {
        cout << i + 1 << "  ";
        cout << table[i].TeamName << "  ";
        cout << table[i].playedGames << "   ";
        cout << table[i].WinGames << "   ";
        cout << table[i].DrawGames << "   ";
        cout << table[i].LostGames << "   ";
        cout << table[i].GoalsScored << "   ";
        cout << table[i].GoalsConceded << "   ";
        cout << table[i].GoalDifference << "   ";
        cout << table[i].points << "\n";
    }

    cout << "-------------------------------------------------------------" << "\n";
}


void saveTable(vector<Team> table, string fileName) {
    ofstream file(fileName);

    if (file.is_open()) {
        file << "TABLA DE POSICIONES" << "\n";
        file << "-------------------------------------------------------------" << "\n";
        file << "#  Equipo              PJ  PG  PE  PP  GF  GC  DG  PTS" << "\n";
        file << "-------------------------------------------------------------" << "\n";

        for (int i = 0; i < table.size(); i++) {
            file << i + 1 << "  "
                 << table[i].TeamName << "  "
                 << table[i].playedGames << "   "
                 << table[i].WinGames << "   "
                 << table[i].DrawGames << "   "
                 << table[i].LostGames << "   "
                 << table[i].GoalsScored << "   "
                 << table[i].GoalsConceded << "   "
                 << table[i].GoalDifference << "   "
                 << table[i].points << "\n";
        }

        file << "-------------------------------------------------------------" << "\n";
        file.close();

        cout << "La tabla fue guardada en partidos.txt" << "\n";
    }
    else {
        cout << "No se pudo abrir el archivo de la tabla" << "\n";
    }
}


void showAllMatches(vector<Match> matches) {
    cout << "\nPARTIDOS JUGADOS" << "\n";
    cout << "------------------------" << "\n";

    if (matches.size() == 0) {
        cout << "No hay partidos registrados." << "\n";
    }
    else {
        for (int i = 0; i < matches.size(); i++) {
            cout << "Fecha: " << matches[i].Date << "\n";
            cout << matches[i].LocalTeam << " vs " << matches[i].VisitingTeam << "\n";
            cout << "Marcador: " << matches[i].LocalTeamGoals << " - " << matches[i].VisitingTeamGoals << "\n";
            cout << "------------------------" << "\n";
        }
    }
}

int getNextMatchdayNumber(string fileName) {
    ifstream file(fileName);
    string line;
    int count = 0;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.find("JORNADA=") == 0) {
                count = count + 1;
            }
        }

        file.close();
    }

    return count + 1;
}

void saveMatch(Match newMatch, string fileName) {
    ofstream file(fileName, ios::app);

    if (file.is_open()) {
        file << newMatch.Date << ";"
             << newMatch.LocalTeam << ";"
             << newMatch.VisitingTeam << ";"
             << newMatch.LocalTeamGoals << ";"
             << newMatch.VisitingTeamGoals << "\n";

        file.close();
    }
    else {
        cout << "No se pudo abrir el archivo de partidos." << "\n";
    }
}

void saveMatchday(int matchdayNumber, string date, vector<Match> matches, string fileName) {
    ofstream file(fileName, ios::app);

    if (file.is_open()) {
        file << "JORNADA=" << matchdayNumber << "\n";
        file << "FECHA=" << date << "\n";

        for (int i = 0; i < matches.size(); i++) {
            file << matches[i].LocalTeam << ";"
                 << matches[i].VisitingTeam << ";"
                 << matches[i].LocalTeamGoals << ";"
                 << matches[i].VisitingTeamGoals << "\n";
        }

        file << "FIN_JORNADA" << "\n";
        file.close();
    }
    else {
        cout << "No se pudo abrir el archivo de jornadas." << "\n";
    }
}


void showMatchdaysHistory(string fileName) {
    ifstream file(fileName);
    string line;

    if (file.is_open()) {
        cout << "\nHISTORIAL DE JORNADAS" << "\n";
        cout << "----------------------" << "\n";

        while (getline(file, line)) {
            cout << line << "\n";
        }

        file.close();
    }
    else {
        cout << "No se pudo abrir el archivo de jornadas." << "\n";
    }
}

//Export to csv

void exportMatchesToCSV(string txtFileName, string csvFileName) {
    ifstream txtFile(txtFileName);
    ofstream csvFile(csvFileName);

    if (txtFile.is_open() && csvFile.is_open()) {
        string line;

        csvFile << "Fecha,Local,Visitante,GoleEquipoLocal,GolesEquipoVisitante" << "\n";

        while (getline(txtFile, line)) {
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == ';') {
                    line[i] = ',';
                }
            }

            csvFile << line << "\n";
        }

        txtFile.close();
        csvFile.close();

        cout << "Partidos exportados a CSV" << "\n";
    }
    else {
        cout << "No se pudieron abrir los archivos" << "\n";
    }
}


void exportTableToCSV(vector<Team> table, string csvFileName) {
    ofstream file(csvFileName);

    if (file.is_open()) {
        file << "Posicion,Equipo,PJ,PG,PE,PP,GF,GC,DG,Puntos" << "\n";

        for (int i = 0; i < table.size(); i++) {
            file << i + 1 << ",";
            file << table[i].TeamName << ",";
            file << table[i].playedGames << ",";
            file << table[i].WinGames << ",";
            file << table[i].DrawGames << ",";
            file << table[i].LostGames << ",";
            file << table[i].GoalsScored << ",";
            file << table[i].GoalsConceded << ",";
            file << table[i].GoalDifference << ",";
            file << table[i].points << "\n";
        }

        file.close();
        cout << "Tabla exportada a CSV" << "\n";
    }
    else {
        cout << "No se pudo abrir el archivo CSV" << "\n";
    }
}

// New Functions

void showSpecialStats(vector<Team> table, vector<Match> matches) {
    if (table.size() == 0) {
        cout << "No hay datos suficientes para mostrar estadisticas." << "\n";
        return;
    }

    Team leader = table[0];
    Team lastPlace = table[0];
    Team bestAttack = table[0];
    Team bestDefense = table[0];

    for (int i = 0; i < table.size(); i++) {
        if (table[i].points > leader.points) {
            leader = table[i];
        }

        if (table[i].points < lastPlace.points) {
            lastPlace = table[i];
        }

        if (table[i].GoalsScored > bestAttack.GoalsScored) {
            bestAttack = table[i];
        }

        if (table[i].GoalsConceded < bestDefense.GoalsConceded) {
            bestDefense = table[i];
        }
    }

    cout << "\nESTADISTICAS ESPECIALES" << "\n";
    cout << "------------------------" << "\n";
    cout << "Lider actual: " << leader.TeamName << " con " << leader.points << " puntos" << "\n";
    cout << "Ultimo lugar: " << lastPlace.TeamName << " con " << lastPlace.points << " puntos" << "\n";
    cout << "Mejor ataque: " << bestAttack.TeamName << " con " << bestAttack.GoalsScored << " goles a favor" << "\n";
    cout << "Mejor defensa: " << bestDefense.TeamName << " con " << bestDefense.GoalsConceded << " goles en contra" << "\n";

    if (matches.size() > 0) {
        Match mostGoalsMatch = matches[0];
        int maxGoals = matches[0].LocalTeamGoals + matches[0].VisitingTeamGoals;

        for (int i = 1; i < matches.size(); i++) {
            int totalGoals = matches[i].LocalTeamGoals + matches[i].VisitingTeamGoals;

            if (totalGoals > maxGoals) {
                maxGoals = totalGoals;
                mostGoalsMatch = matches[i];
            }
        }

        cout << "Partido mas goleador: ";
        cout << mostGoalsMatch.LocalTeam << " ";
        cout << mostGoalsMatch.LocalTeamGoals << " - ";
        cout << mostGoalsMatch.VisitingTeamGoals << " ";
        cout << mostGoalsMatch.VisitingTeam;
        cout << " con " << maxGoals << " goles en total" << "\n";
    }
    else {
        cout << "No hay partidos registrados para calcular el partido mas goleador" << "\n";
    }
}


void resetStats(Team* team) {
    (*team).playedGames = 0;
    (*team).WinGames = 0;
    (*team).DrawGames = 0;
    (*team).LostGames = 0;
    (*team).GoalsScored = 0;
    (*team).GoalsConceded = 0;
    (*team).GoalDifference = 0;
    (*team).points = 0;
}

void resetAllStats(vector<Team>& table) {
    for (int i = 0; i < table.size(); i++) {
        resetStats(&table[i]);
    }

    cout << "Las estadisticas de la tabla actual fueron reiniciadas." << "\n";
}

