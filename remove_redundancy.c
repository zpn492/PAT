#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** ******* *************************************************** */
/** STRUCTS *************************************************** */
/** ******* *************************************************** */
/* File & flow handling */
struct Controlflow {
    int patternm;
    int pattern;
} controller;

struct Buffer {
    /* The current instruction line */
    char line[256];
    char param_set[5][32];

    /* Some list to split
     * an instruction line
     * into single paramters */
    char param_sets[5][5][32];

    /* Previous instruction lines */
    char previous_lines[5][256];
} buffer;

struct Filehandler {
    FILE *input_file;
    FILE *output_file;
} filehandler;
/** ******* *************************************************** */
/** ******* *************************************************** */

/** ******* *************************************************** */
/* functions */
/** ******* *************************************************** */
int apply();
int memory_exchange();
int special_if_statements();

/** ******* *************************************************** */
/* File & flow handling */
/** ******* *************************************************** */
int initialize(const char *file);
int getinst();
char* getparam(int idx);
int saveline(int idx);
int setPreviousLines();

/** ******* *************************************************** */
/** main */
/** ******* *************************************************** */
int main(int arg, char *args[])
    {
    int line=0; /* start w/ line 2 of the file */
    
    /* load in the instruction memory */
    filehandler.input_file=fopen(args[1], "r");
    if(!filehandler.input_file) {
        printf("%s: Unable to open input file %s.\n","",args[1]);
        exit(1);
    }
    fclose(filehandler.input_file);

    /* memory exchange */
    apply(1, args[1]);  
    apply(2, "reduced.pal");

    return 0;
    }

int apply(int method, const char *file) {   
    initialize(file);
    
    /* loop to load instruction memory */
    while(getinst()) {        
        int action;

        /* check for memory exchange pattern 
         * if none just write the instruction */
        if(method == 1)
            action = memory_exchange();
        else
            /* check for special if-statements */
            action = special_if_statements();
        if(action == 0)    
            fputs(buffer.line, filehandler.output_file);
    }
    fclose(filehandler.input_file);
    fclose(filehandler.output_file);
}

int memory_exchange() {
    int action = 0;

    /* reset on an add */
    if( !strcasecmp(getparam(0), "ADD") ) {
        for(int i = 0; i < controller.pattern; i++)
            fputs(buffer.previous_lines[i], 
                filehandler.output_file);
        controller.pattern = 0;   
    }
    
    /** memory exchange start
      * save lines **/
    if( !strcasecmp(getparam(0), "ADD") || controller.pattern > 0) {
        saveline(controller.pattern);
        controller.pattern += 1; 
        action = 1;  
    }            

    /* memory exchange end 
        * */
    if( !strcasecmp(getparam(0), "SUB") && controller.pattern == 5) {
        char buf[256];

        /* Place previous lines in 
         * param_sets[0..4] 
         *
         * EXCH within param_sets[2]
         * ADDI within param_sets[1]
         * SUB within param_sets[4]
         **/
        setPreviousLines();
        
        if( !strcasecmp(buffer.param_sets[2][0], "EXCH") ) {
            strcpy(buffer.param_sets[2][4]
                , "                         ");
            sprintf(buf,"%s%s  %s %s %s\n",buffer.param_sets[2][4]
                ,"EXCHI"
                ,buffer.param_sets[2][1]
                ,buffer.param_sets[4][2]
                ,buffer.param_sets[1][2]);
            printf("%s", buf);

            fputs(buf, filehandler.output_file);
            controller.pattern = 0; 
            action = 1;
        }
    }

    /* memory exchange final
        * if no pattern was regconized 
        * print all five lines */
    if( controller.pattern == 5) {
        for(int i = 0; i < controller.pattern; i++)
            fputs(buffer.previous_lines[i], filehandler.output_file);
        controller.pattern = 0;
        action = 1;
    }     

    return action; 
}

int special_if_statements() {
    int action = 0;

    if( strstr(getparam(0), "cmp") != NULL && controller.patternm == 0) {
        saveline(0); /* cmp */
        printf("%s%s\n", "cmp: ", buffer.previous_lines[0]);
        controller.patternm = 1;
    }

    if( !strcasecmp(getparam(0), "XOR") && controller.patternm == 1) {
        saveline(1); /* xor */
        printf("%s%s\n", "xor: ", buffer.previous_lines[1]);
    }

    if( getparam(0)[strlen(getparam(0))-2]=='i' && strstr(getparam(0), "cmp") != NULL 
        &&  controller.patternm == 1) {
        char buf[256];
        char cls[32];
        setPreviousLines();

        strcpy(buffer.param_set[4], "                         ");
        strcpy(buffer.param_set[2], "               ");
        
        char *pp1 = strndup(buffer.param_sets[0][0], strlen(buffer.param_sets[0][0])-1);
        char *pp2 = strndup(getparam(0), strlen(getparam(0))-3);
        char *pp3 = strndup(buffer.param_sets[0][4], strlen(buffer.param_sets[0][4]));
        printf("%s %s\n", "pp1: ", pp1);
        printf("%s %s\n", "pp2: ", pp2);

        if( !strcasecmp(pp1, pp2) ) {
            /* if */
            sprintf(buf,"%s%s%s    %s %s %s\n", 
                buffer.param_sets[0][0], 
                getparam(2), 
                buffer.param_sets[0][1], 
                buffer.param_sets[0][2], 
                buffer.param_sets[0][3], 
                buffer.param_sets[0][4]);
            fputs(buf, filehandler.output_file);

            /* xori */
            sprintf(buf,"%s%s   %s %s\n",getparam(4), "XORI", 
                buffer.param_sets[1][1], "1");
            fputs(buf, filehandler.output_file);

            /* fi */
            sprintf(buf,"%s:%s%s    %s %s %s\n", pp3, getparam(2), 
                buffer.param_sets[0][1], 
                buffer.param_sets[0][2], 
                buffer.param_sets[0][3], 
                pp1);
            fputs(buf, filehandler.output_file);

            controller.patternm = 0;
        }
        action = 1;
    }

    return action+controller.patternm;
}

int initialize(const char *file) {
    /* copy input file to 
     * tmp file */
    filehandler.input_file=fopen(file, "r");
    filehandler.output_file=fopen("tmp.pal", "w");
    while(getinst()) {
        fputs(buffer.line, filehandler.output_file);
    }
    fclose(filehandler.output_file);
    fclose(filehandler.input_file);
    
    filehandler.input_file = fopen("tmp.pal", "r");
    filehandler.output_file = fopen("reduced.pal", "w");
    
    /* make sure file is in valid pendulum format */
    fgets(buffer.line,256,filehandler.input_file); /* get first line */
    if( strncmp(buffer.line, ";; pendulum pal file", 20) ) { 
        /* compare with known header */
        printf("Input file not in in Pendulum pal format.\n");
        exit(1);
    }    
    /* Initialize new file */
    sprintf(buffer.line, ";; pendulum pal file\n");
    fputs(buffer.line, filehandler.output_file);
    return 0;
};

int getinst() {
    int r = fgets(buffer.line,256,filehandler.input_file); /* get first line */

    /*  */
    int fields=sscanf(buffer.line,"%s%s%s%s%s",
        buffer.param_set[0],
        buffer.param_set[1],
        buffer.param_set[2],
        buffer.param_set[3],
        buffer.param_set[4]);
    
    if( fields==0 || fields==EOF ) return 0;

    return r;
};

char* getparam(int idx) {
    return buffer.param_set[idx];
}

int saveline(int idx) {
    strcpy(buffer.previous_lines[idx], buffer.line);
    return 0;
};

int setPreviousLines() {
    for(int i = 0; i < 5; i++)
        {
        sscanf(buffer.previous_lines[i],"%s%s%s%s%s",
        buffer.param_sets[i][0],
        buffer.param_sets[i][1],
        buffer.param_sets[i][2],
        buffer.param_sets[i][3],
        buffer.param_sets[i][4]);
        }
    return 0;
}

