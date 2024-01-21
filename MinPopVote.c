/// @file MinPopVote.c
/// @author Muhammad Talha Adnan Khan
/// @date October 19, 2023
/// @brief This file contains the definations of functions used in the app.c file. This program 
///        uses recursion to find the Minimum number of votes to win in an election.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

/// @brief This function sets the settings for running the program and the year from the command line arguments.
/// @param argc 
/// @param argv 
/// @param year 
/// @param fastMode 
/// @param quietMode 
/// @return boolean -> state of the function that tells if we have a valid command line argument or not.
bool setSettings(int argc, char** argv, int* year, bool* fastMode, bool* quietMode) {
    *year = 0; *quietMode = false; *fastMode = false;

    for (int i = 1; i < argc; i++){
    // [-y year] = sets the election year for the program
    //                  valid [year] values are perfect multiples of 4,
    //                  between 1828 and 2020, inclusively; 
    //                  default is 0 ([year] then set by user-input later)
        if (strcmp(argv[i], "-y") == 0){
            if ((i + 1) < argc){
                if (atoi(argv[i + 1]) != 0){
                    (*year) = atoi(argv[i + 1]);
                    if ( !((*year > 1827) && (*year < 2021) && (*year % 4 == 0)) ){
                        *year = 0;
                        i++;
                    }else{
                        i++;
                        continue;
                    }
                }else{ return false;}
            }else{ return false;}
        }
    // [-q] = quiet mode; if ON, do not print the full State list read-in
    //                    from file AND do no print the subset of States
    //                    needed to win with minimum popular votes
    //                    default is OFF
        else if(strcmp(argv[i], "-q") == 0){
            *quietMode = true;
        }
    // [-f] = fast mode; if ON, use the "fast" version of the functions
    //                   that include memoization to find the minimum 
    //                   number of popular votes to win the election
    //                   default is OFF
        else if (strcmp(argv[i], "-f") == 0){
            *fastMode = true;
        }
        else{
             return false;
        }
    }
    return true;
}
/// @brief Sets the file name of data to be read from.
/// @param filename 
/// @param year 
void inFilename(char* filename, int year) { sprintf(filename,"data/%d.csv",year); }

/// @brief Sets the file name for the data to be outputed to
/// @param filename 
/// @param year 
void outFilename(char* filename, int year) { sprintf(filename, "toWin/%d_win.csv", year);}

bool parseLine(char* line, State* myState){

    int numScans = sscanf(line, "%49[^,],%2[^,],%d,%d", myState->name, myState->postalCode, &myState->electoralVotes, &myState->popularVotes);

    return (numScans == 4);
}

/// @brief Reads in the election data and stores it in the appropriate data structures like State array.
/// @param filename 
/// @param allStates 
/// @param nStates 
/// @return boolean -> if it was successfull in reading file or not.
bool readElectionData(char* filename, State* allStates, int* nStates) {

    FILE* inFile = fopen(filename, "r");
    *nStates = 0;
    char line[256];
    if (inFile == NULL) return false;

    while(fgets(line, 256, inFile) != NULL){
        State stateDiscription;
        if (parseLine(line, &stateDiscription)){
            allStates[(*nStates)] = stateDiscription;
            (*nStates)++;
        }
    }
    fclose(inFile);

    return true;
}
 
/// @brief This function returns the total number for Electoral votes of the state array passed in.
/// @param states 
/// @param szStates 
/// @return integer
int totalEVs(State* states, int szStates) {
    int ct = 0;
    for (int i = 0; i < szStates; i++){
        ct += states[i].electoralVotes;
    }
    return ct;
}
/// @brief This function returns the total number of popular votes of the state array passed in
/// @param states 
/// @param szStates 
/// @return integer
int totalPVs(State* states, int szStates) {
    int ct = 0;
    for (int i = 0; i < szStates; i++){
        ct += states[i].popularVotes;
    }
    return ct;
}

/// @brief This function is a brute force implementation of the backpack problem but opposite to it. 
///        in this we choose our upper bound and see the least votes needed that exceed that upper bound.
/// @param states 
/// @param szStates 
/// @param start 
/// @param EVs 
/// @return MinInfo
MinInfo minPopVoteAtLeast(State* states, int szStates, int start, int EVs) {
    MinInfo retMinInfo;
    if (start == szStates) {
        // Base case: Subset is complete
        retMinInfo.sufficientEVs = (EVs <= 0);
        retMinInfo.subsetPVs = 0;
        retMinInfo.szSomeStates = 0;
        return retMinInfo;
    }
    if (EVs <= 0) {
        retMinInfo.subsetPVs = 0;
        retMinInfo.sufficientEVs = true;
        retMinInfo.szSomeStates = 0;
        return retMinInfo;
    }

    // Recursive case: Include the current state or exclude it
    MinInfo included = minPopVoteAtLeast(states, szStates, start + 1, EVs - states[start].electoralVotes);

    if ((EVs - states[start].electoralVotes) <= 0 || included.sufficientEVs ) {
        included.subsetPVs += (states[start].popularVotes/2 + 1);
        included.someStates[included.szSomeStates++] = states[start];
    }

    MinInfo excluded = minPopVoteAtLeast(states, szStates, start + 1, EVs);

    if (excluded.subsetPVs <= included.subsetPVs && excluded.sufficientEVs) {
        return excluded;
    } else {
        return included;
    }
}

/// @brief This function calculate the required number of electoral votes to win the election 
///        (with a call to totalEVs()) and then call the recursive function minPopVoteAtLeast().
/// @param states 
/// @param szStates 
/// @return MinInfo
MinInfo minPopVoteToWin(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}

/// @brief This function is a faster implementation of the backpack problem using memoization but opposite to it. 
///        in this we choose our upper bound and see the least votes needed that exceed that upper bound.
/// @param states 
/// @param szStates 
/// @param start 
/// @param EVs 
/// @param memo 
/// @return MinInfo
MinInfo minPopVoteAtLeastFast(State* states, int szStates, int start, int EVs, MinInfo** memo) {
    MinInfo retMinInfo;
    if (start == szStates) {
        // Base case: Subset is complete
        retMinInfo.sufficientEVs = (EVs <= 0);
        retMinInfo.subsetPVs = 0;
        retMinInfo.szSomeStates = 0;
        return retMinInfo;
    }
    if (EVs <= 0) {
        retMinInfo.subsetPVs = 0;
        retMinInfo.sufficientEVs = true;
        retMinInfo.szSomeStates = 0;
        return retMinInfo;
    }

    // Recursive case: Include the current state or exclude it
    MinInfo included = minPopVoteAtLeastFast(states, szStates, start + 1, EVs - states[start].electoralVotes, memo);
    if (memo[start][EVs].subsetPVs >= 0){
        return memo[start][EVs];
    }
    if ((EVs - states[start].electoralVotes) <= 0 || included.sufficientEVs ) {
        included.subsetPVs += (states[start].popularVotes/2 + 1);
        included.someStates[included.szSomeStates++] = states[start];
    }

    MinInfo excluded = minPopVoteAtLeastFast(states, szStates, start + 1, EVs, memo);

    if (excluded.subsetPVs <= included.subsetPVs && excluded.sufficientEVs) {
        memo[start][EVs] = excluded;
        return excluded;
    } else {
        memo[start][EVs] = included;
        return included;
    }
}
/// @brief This function calculate the required number of electoral votes to win the election 
///        (with a call to totalEVs()) and then call the recursive function minPopVoteAtLeastFast().
/// @param states 
/// @param szStates 
/// @return MinInfo.
MinInfo minPopVoteToWinFast(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election

    MinInfo** memo = (MinInfo**)malloc((szStates+1)*sizeof(MinInfo*));
    for (int i = 0; i < szStates+1; ++i) {
        memo[i] = (MinInfo*)malloc((reqEVs+1)*sizeof(MinInfo));
        for (int j = 0; j < reqEVs+1; ++j) {
            memo[i][j].subsetPVs = -1;
        }
    }
    MinInfo res = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo);
    
    // memo goes out of scope so I free all heap allocated memory for memo before return.
    for (int i = 0; i < szStates+1; ++i) {
       free(memo[i]);
    }
    free(memo);

    return res;
}

/// @brief This function writes the winning data in to a file using file pointer and fprintf().
/// @param filenameW 
/// @param totEVs 
/// @param totPVs 
/// @param wonEVs 
/// @param toWin 
/// @return boolean -> returns whether write was successful or not.
bool writeSubsetData(char* filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin) {
    FILE* outFile = fopen(filenameW, "w");
   
    if (outFile == NULL) {
        return false; 
    }

    fprintf(outFile, "%d,%d,%d,%d\n", totEVs, totPVs, wonEVs, toWin.subsetPVs);
    for (int i = toWin.szSomeStates - 1; i >= 0; i--){
        fprintf(outFile,"%s,%s,%d,%d\n", toWin.someStates[i].name, toWin.someStates[i].postalCode, toWin.someStates[i].electoralVotes, ((toWin.someStates[i].popularVotes/2)+1));
    }
    fclose(outFile);
    return true; 
}