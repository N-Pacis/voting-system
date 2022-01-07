#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char CANDIDATES[4][100] = {"David Hull", "Kristin Canella", "Jim Brar", "Donald Truimph"};

void saveToFile(char *filename,char *str)
{
    FILE *out = fopen(filename, "a+");
    fputs(str, out);
    fclose(out);
}

void rewriteToFile(char *filename,char *str){
    FILE *out = fopen(filename, "w+");
    fputs(str, out);
    fclose(out);
}

void migrateFiles(char *fname1, char *fname2){
    char fileBuffer[255];
    FILE *inTemp = fopen(fname1, "r");
    while(fgets(fileBuffer,255,inTemp)){
        char *candidate = fileBuffer;
        saveToFile(fname2,candidate);
    }
    rewriteToFile(fname1,"");
    remove(fname1);
}

void updateVotes(char *str){
    FILE *in = fopen("storage/candidates.txt", "r");
    FILE *inTemp = fopen("storage/temp.txt", "r");

    char buffer[255];
    char fileBuffer[255];
    while(fgets(buffer,255,in)){
        char *candidate = buffer;
        char *candidateName = strtok(buffer,",");
        int votes;
        if(strcmp(str,candidateName) == 0){
             int count = 0;
             while( candidate != NULL ) {
                 count++;
                 if(count == 2){
                    votes = atoi(candidate) + 1;
                 }
                candidate = strtok(NULL, " ");
            }
            char votesUpdated[10];
            itoa(votes,votesUpdated,10);
            char sVotesUpdated[10] = {','};
            int i;
            for(i=1;i<sizeof(votesUpdated)/sizeof(votesUpdated[0]);i++){
                sVotesUpdated[i] = votesUpdated[i - 1];
            }
            char *updatedCandidate = strcat(candidateName,sVotesUpdated);
            char *sUpdatedCandidate = strcat(updatedCandidate,"\n");
            saveToFile("temp.txt",sUpdatedCandidate);
        }
        else{
            int count = 0;
             while( candidate != NULL ) {
                 count++;
                 if(count == 2){
                    votes = atoi(candidate);
                 }
                candidate = strtok(NULL, " ");
            }
            char sVotes[10];
            itoa(votes,sVotes,10);
            char sVotesUpdated[10] = {','};
            int i;
            for(i=1;i<sizeof(sVotes)/sizeof(sVotes[0]);i++){
                sVotesUpdated[i] = sVotes[i - 1];
            }
            char *updatedCandidate = strcat(candidateName,sVotesUpdated);
            char *sUpdatedCandidate = strcat(updatedCandidate,"\n");
            saveToFile("temp.txt",sUpdatedCandidate);
        }
        rewriteToFile("candidates.txt","");
        while(fgets(fileBuffer,255,inTemp)){
            char *candidate = fileBuffer;
            saveToFile("candidate.txt",candidate);
        }
    }
    migrateFiles("candidate.txt","candidates.txt");
    remove("temp.txt");
    rewriteToFile("temp.txt","");
}

void castVote()
{
    int choice;
    printf("\n\n ### Please choose your Candidate ####\n\n");

    for (int i = 0; i < (sizeof(CANDIDATES) / sizeof(CANDIDATES[0])); i++)
    {
        char *candidate = strtok(CANDIDATES[i],",");
        printf("\n%d. %s",(i+1),candidate);
    }

    printf("\n\n Input your choice: ");
    scanf("%d", &choice);
    
    int length = sizeof(CANDIDATES)/sizeof(CANDIDATES[0]);

    choice = choice -1;
    if (!(choice >= 0 && choice<length))
    {
        printf("\n Invalid Choice");
        exit(-1);
    }
    else{
        char *candidate = strtok(CANDIDATES[choice],",");
        updateVotes(candidate);

        printf("\n thanks for vote !!");
    }
    
}

void getVotesCount(){
    char fileBuffer[255];
    FILE *inTemp = fopen("storage/candidates.txt", "r");
    while(fgets(fileBuffer,255,inTemp)){
        char *candidate = fileBuffer;
        printf("- %s",candidate);
    }
}

void getLeadingCandidate(){
    char fileBuffer[255];
    FILE *inTemp = fopen("storage/candidates.txt", "r");
    int highestVotes = 0;
    while(fgets(fileBuffer,255,inTemp)){
        char *candidate = strtok(fileBuffer,",");
        int count = 0;
        while(candidate != NULL ) {
            count++;
            if(count == 2){
                if(atoi(candidate) > highestVotes){
                    highestVotes = atoi(candidate);
                }
            }
            candidate = strtok(NULL, " ");
        }
    }
    if(highestVotes == 0){
        getVotesCount();
    }
    else{
        fclose(inTemp);
        FILE *in = fopen("storage/candidates.txt","r");
        char buffer[255];
        while(fgets(buffer,255,in)){
            char *candidate = strtok(buffer,",");
            int count = 0;
            int votes;
            while(candidate != NULL ) {
                count++;
                if(count == 2){
                    votes = atoi(candidate);
                    if(atoi(candidate) == highestVotes){
                        printf("- %s with %d votes",buffer,votes);
                    }
                }
                candidate = strtok(NULL, " ");
            }
        } 
    }
}

int main(int argc, char *argv[])
{
    int i;
    int choice;
    rewriteToFile("candidate.txt","");
    rewriteToFile("candidate.txt","");

    FILE *fp = fopen("storage/candidates.txt","r");
    if (NULL != fp) {
        int size;
        fseek (fp, 0, SEEK_END);
        size = ftell(fp);

        if (0 == size) {
            for (int i = 0; i < (sizeof(CANDIDATES) / sizeof(CANDIDATES[0])); i++)
            {
                char *str = strcat(CANDIDATES[i],",0\n");
                saveToFile("candidates.txt",str);
            }        
        }
    }

    do
    {
        printf("\n\n ###### Welcome to Election/Voting 2019 #####");
        printf("\n\n 1. Cast the Vote");
        printf("\n 2. Find Vote Count");
        printf("\n 3. Find leading Candidate");
        printf("\n 0. Exit");

        printf("\n\n Please enter your choice : ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            castVote();
            break;
        case 2:
            getVotesCount();
            break;
        case 3:
            getLeadingCandidate();
            break;
        case 0:
            exit(0);
            break;
        default:
            printf("\n Error: Invalid Choice");
        }
    } while (choice != 0);

    //hold the screen
    getchar();
    remove("temp.txt");
    remove("candidate.txt");
    return 0;
}
