#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

bool test_totalEVs() {
    State aStates[10];
    int res;
    
    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 10;

    printf(" Checking totalEVs() for 5 States:\n");
    res = totalEVs(aStates,5);
    if (res != 38) {
        printf("  individual state EVs are 5, 8, 13, 6, 7\n");
        printf("  expected total EVs = 38, actual total EVs = %d\n",res);
        return 0;
    }

    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 15;
    aStates[6].electoralVotes = 12;
    aStates[7].electoralVotes = 8;
    aStates[8].electoralVotes = 14;
    aStates[9].electoralVotes = 13;
    
    printf(" Checking totalEVs() for 10 States:\n");
    res = totalEVs(aStates,10);
    if (res != 100) {
        printf("  expected total EVs = 100, actual total EVs = %d\n",res);
        return false;
    }

    return true;
}

bool test_totalPVs() {
    //--------------------------------------------------------   
    // TODO: Task 6 - write your own test case for totalPVs();
    //                use test_totalEVs() as a sample;
    //                make sure to test all components
    //--------------------------------------------------------
    return false; 
}

bool test_setSettings() {
    // command-line argument settings
    // [-y year] = sets the election year for the program
    //                  valid [year] values are perfect multiples of 4,
    //                  between 1828 and 2020, inclusively; 
    //                  default is 0 ([year] then set by user-input later)   
    // [-q] = quiet mode; if ON, do not print the full State list read-in
    //                    from file AND do no print the subset of States
    //                    needed to win with minimum popular votes
    //                    default is OFF
    // [-f] = fast mode; if ON, use the "fast" version of the functions
    //                   that include memoization to find the minimum 
    //                   number of popular votes to win the election
    //                   default is OFF
    // these arguments are optional to run program; if any is absent,
    //  then use the default value

    int year1;
    bool test1, pass1, fastMode1, quietMode1;
    int argc_test1 = 5;
    char* argv_test1[] = {"./app.exe", "-f", "-q", "-y", "2020"}; // Expect true
    pass1 = setSettings(argc_test1, argv_test1, &year1, &fastMode1, &quietMode1);
    test1 = (pass1 && fastMode1 == true && quietMode1 == true && (year1 == 2020)) ? true : false;


    int year2;
    bool test2, pass2, fastMode2 , quietMode2;
    int argc_test2 = 4;
    char* argv_test2[] = {"./app.exe","-y","1999","-f"}; // Expect true and year to be 0 rest false.
    pass2 = setSettings(argc_test2, argv_test2, &year2, &fastMode2, &quietMode2);
    test2 = (pass2 && fastMode2 && quietMode2 == false && (year2 == 0)) ? true : false;

    int year3;
    bool test3, pass3, fastMode3 , quietMode3;
    int argc_test3 = 5;
    char* argv_test3[] = {"./app.exe","-y","2020","-f", "-q"}; // Expect true and year to be 2020, fastMode -> true, quietMode -> true.
    pass3 = setSettings(argc_test3, argv_test3, &year3, &fastMode3, &quietMode3);
    test3 = (pass3 == true && fastMode3 == true && quietMode3 == true && (year3 == 2020)) ? true : false;

    int year4;
    bool test4, pass4, fastMode4 , quietMode4;
    int argc_test4 = 5;
    char* argv_test4[] = {"./a.out", "-y", "1860", "-v", "-f"}; // Expect false and year to be 0 and rest false.
    pass4 = setSettings(argc_test4, argv_test4, &year4, &fastMode4, &quietMode4);
    test4 = (pass4 == false && fastMode4 == false && quietMode4 == false && (year4 == 1860)) ? true : false;

    return (test1 && test2 && test3 && test4);
}

bool test_inFilename() {
    int test1 = 1828, test2 = 1920, test3 = 2020;
    char check1[] = "data/1828.csv";
    char check2[] = "data/1920.csv";
    char check3[] = "data/2020.csv";
    char out1[20] = "";
    char out2[20] = "";
    char out3[20] = "";
    bool pass1, pass2, pass3;

    inFilename(out1, test1);
    inFilename(out2, test2);
    inFilename(out3, test3);

    pass1 = strcmp(out1, check1) == 0 ? true : false;
    pass2 = strcmp(out2, check2) == 0 ? true : false;
    pass3 = strcmp(out3, check3) == 0 ? true : false;

    return pass1 && pass2 && pass3;
}

bool test_outFilename() {
    int test1 = 1828, test2 = 1920, test3 = 2020;
    char check1[] = "toWin/1828_win.csv";
    char check2[] = "toWin/1920_win.csv";
    char check3[] = "toWin/2020_win.csv";
    char out1[20] = "";
    char out2[20] = "";
    char out3[20] = "";
    bool pass1, pass2, pass3;

    outFilename(out1, test1);
    outFilename(out2, test2);
    outFilename(out3, test3);

    pass1 = strcmp(out1, check1) == 0 ? true : false;
    pass2 = strcmp(out2, check2) == 0 ? true : false;
    pass3 = strcmp(out3, check3) == 0 ? true : false;

    return pass1 && pass2 && pass3; 
}

bool test_parseLine() {
    State out1, out2, out3;
    char input1[] = "Connecticut,CT,6,64616";
    char input2[] = "North Carolina,NC,82521";
    char input3[] = "Virginia,VA,17,95539";
    bool result1, result2, result3, test1, test2, test3;

    result1 = parseLine(input1, &out1); // Expect true.
    result2 = parseLine(input2, &out2); // Expect false.
    result3 = parseLine(input3, &out3); // Expect true.

    test1 = (result1 && strcmp(out1.name, "Connecticut") == 0 && strcmp(out1.postalCode, "CT") == 0 && out1.electoralVotes == 6 && out1.popularVotes == 64616) ? true : false;

    test2 = (result2 && strcmp(out2.name, "North Carolina") == 0 && strcmp(out2.postalCode, "NC") == 0 && out2.popularVotes == 82521) ? false : true;

    test3 = (result3 && strcmp(out3.name, "Virginia") == 0 && strcmp(out3.postalCode, "VA") == 0 && out3.electoralVotes == 17 && out3.popularVotes == 95539) ? true : false;



    return test1 && test2 && test3; 
}

bool test_readElectionData() {
    State state[51];
    int ct = 0;
    char input1[] = "data/1880.csv"; // Expect true.
    char input2[] = "data/2021.csv"; // Expect false.

    bool test1 = readElectionData(input1, state, &ct);
    bool test2 = readElectionData(input2, state, &ct);

    return test1 && !test2;
}

bool test_minPVsSlow() {
    State states[51];
    int ct;
    MinInfo won;
    readElectionData("data/1828.csv", states, &ct);
    won = minPopVoteToWin(states, ct);
    int electoralVotes = totalEVs(won.someStates, won.szSomeStates);

    return electoralVotes == 131; 
}

bool test_minPVsFast() {
    State states[51];
    int ct;
    MinInfo won;
    readElectionData("data/1828.csv", states, &ct);
    won = minPopVoteToWinFast(states, ct);
    int electoralVotes = totalEVs(won.someStates, won.szSomeStates);

    return electoralVotes == 131; 
}

int main() {
    printf("Welcome to the Popular Vote Minimizer Testing Suite!\n\n");
    
    printf("Testing totalEVs()...\n"); 
    if (test_totalEVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing totalPVs()...\n"); 
    if (test_totalPVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing setSettings()...\n"); 
    if (test_setSettings()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing inFilename()...\n"); 
    if (test_inFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing outFilename()...\n"); 
    if (test_outFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing parseLine()...\n"); 
    if (test_parseLine()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing readElectionData()...\n"); 
    if (test_readElectionData()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing minPopVoteToWin()...\n"); 
    if (test_minPVsSlow()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWinFast()...\n"); 
    if (test_minPVsFast()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    return 0;
}
