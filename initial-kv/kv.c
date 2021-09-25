////////////////////////////////////////////////////////////////////////////////
// Main File:        p1a
// This File:        kv.c
// Other Files:     
// Semester:         CS 537 Fall 2021
// Instructor:       Remzi
// 
// Discussion Group: dis301
// Author:           Zexin (Archer) Li
// Email:            zli885@wisc.edu
// CS Login:         archerl
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          None
//
// Online sources:  stackoverflow
//////////////////////////// 80 columns wide ///////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

const char *DELIM = ",";  // commas ','
const char *FILENAME = "./db.txt";//database filename
//const long long int *HUGENUMBER= 9999999999999999999999999999999999999999999999;

/*
 * Reads and get a certain value in database base on key value
 */
char* kvget(char*key, int p){

    FILE *fp;
    fp = fopen( "./db.txt" , "r" );
    if (!fp)
    {
        printf("Error opening file db.txt\n");
        exit(1);
    }
    char *line = NULL;
    size_t len = 0;
    char *res = NULL;
    char *id = NULL;
    while (getline(&line, &len, fp)!=-1)
    {

        id = strtok(line, DELIM);

        if (strcmp(id, key)==0){

            res = strtok(NULL, DELIM);
            if (p){
                printf("%s,%s",id,res);
            }
            break;
        }
    }
    fclose(fp);
    if (p&&!res){
        printf("%s not found\n", key);
    }
    return res;
}

/*
 * delete a certain key/value pair
 */
int kvdelete(char *key){

    FILE *fp;
    fp = fopen( "./db.txt" , "r" );
    if (!fp)
    {
        printf("Error opening file db.txt\n");
        exit(1);
    }

    FILE *fp2;
    fp2 = fopen( "./tempdb.txt" , "w" );
    if (!fp2)
    {
        printf("Error opening file tempdb.txt");
        exit(1);
    }
    
    char *line = NULL;
    size_t len = 0;
    char tempLine[99999] = "";
    char *id = NULL;
    int found = 0;
    while (getline(&line, &len, fp)!=-1)
    {

        strcpy(tempLine,line);
        id = strtok(line, DELIM);
        
        if (strcmp(id, key)==0){
            found = 1;
            continue;
        }
        fprintf(fp2, "%s", tempLine);
    }
    fclose(fp);
    fclose(fp2);
    if (!found){
        printf("%s not found\n", key);
    }
    else{
        fp = fopen( "./db.txt" , "w" );
        fp2 = fopen( "./tempdb.txt" , "r");
        while (getline(&line, &len, fp2)!=-1){
            fprintf(fp, "%s", line);
        }
        fclose(fp);
        fclose(fp2);
        
    }
    return found;
}

/*
 * clear database
 */
void kvclear(){
    FILE *fp;
    fp = fopen( "./db.txt" , "w" );
    fclose(fp);
}

/*
 * print all data in database
 */
void kvall(){
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    fp = fopen( "./db.txt" , "r" );
    if (!fp)
    {
        printf("Error opening file db.txt\n");
        exit(1);
    }
    while (getline(&line, &len, fp)!=-1){
            printf("%s", line);
    }
}

/*
 * put a key/value pair into database, if key exist then update value
 */
void kvput(char *key, char *value){
    if (kvget(key, 0)){
        kvdelete(key);
    }
    //concat target str
    FILE *fp;

    fp = fopen( "./db.txt" , "a+" );
    fprintf(fp, "%s,%s\n", key, value);

    fclose(fp);
    
}

int check_key_valid(char **commands){
    for (int i = 0; i < strlen(commands[1]);++i){
        if (!isdigit(commands[1][i])){
            return 0;
        }
    }
    return 1;
}

/*
 * process readed commands
 */
void process_command(char **commands){
    //printf("%i\n",commands[1]);
    if (commands[3] != NULL){
        printf("bad command\n");
    }
    else if (*commands[0]=='p' && commands[1] && commands[2] && check_key_valid(commands)){
        //printf("!!!!!doing put!!!!!\n\n");
        kvput(commands[1],commands[2]);
    }
    else if (*commands[0]=='g'&& commands[1]!=NULL && commands[2]==NULL && check_key_valid(commands)){
        //printf("!!!!!doing get!!!!!\n\n");
        kvget(commands[1], 1);
    }
    else if (*commands[0]=='d'&& commands[1]!=NULL && commands[2]==NULL && check_key_valid(commands)){
        //printf("!!!!!doing delete!!!!!!\n\n");
        kvdelete(commands[1]);
    }
    else if (*commands[0]=='c'&& commands[1]==NULL && commands[2]==NULL){
        //printf("!!!!!doing clear!!!!!!\n\n");
        kvclear();
    }
    else if (*commands[0]=='a'&& commands[1]==NULL && commands[2]==NULL){
        //printf("!!!!!doing All!!!!!!\n\n");
        kvall();
    }
    else{
        printf("bad command\n");
    }
}


int main(int argc, char *argv[]){
    if (argc<2 || argc > 3){
    }

    if( access( FILENAME, F_OK ) == 0 ) {
    // file exists
    } else {
        // file doesn't exist
        kvclear();
    }

    char *commands1[4];
    for (int i = 0; i < 4; ++i){
        commands1[i] = NULL;
    }

    if (argc >= 2){
        for (int j = 1; j < argc; ++j){
            char *token = strtok(argv[j], DELIM);
            int ci = 0;
            while (token != NULL){
                commands1[ci] = token;
                ci+=1;
                token = strtok(NULL, DELIM);
            }
            //group commands by 3    
            process_command(commands1);
            for (int i = 0; i < 4; ++i){
                commands1[i] = NULL;
            }
        }
        
    }
    return 0;
}