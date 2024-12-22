
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint8_t shuffle[5][5] =
{
    {0, 1, 2, 3, 4},
    {1, 0, 3, 4, 2},
    {2, 4, 1, 0, 3},
    {3, 2, 4, 1, 0},
    {4, 3, 0, 2, 1}
};

#define MAX_CHARS 1000

char buffer[MAX_CHARS];


char questions[5][MAX_CHARS] = 
    {
        "Question1",
        "Question2",
        "Question3",
        "Question4",
        "Question5"
    };


char answers[5][5][MAX_CHARS] = 
{
    {
        "Answer1toQuestion1",
        "Answer1toQuestion2",
        "Answer1toQuestion3",
        "Answer1toQuestion4",
        "Answer1toQuestion5",
    },
    {
        "Answer2toQuestion1",
        "Answer2toQuestion2",
        "Answer2toQuestion3",
        "Answer2toQuestion4",
        "Answer2toQuestion5",
    },
    {
        "Answer3toQuestion1",
        "Answer3toQuestion2",
        "Answer3toQuestion3",
        "Answer3toQuestion4",
        "Answer3toQuestion5",
    },
    {
        "Answer4toQuestion1",
        "Answer4toQuestion2",
        "Answer4toQuestion3",
        "Answer4toQuestion4",
        "Answer4toQuestion5",
    },
    {
        "Answer5toQuestion1",
        "Answer5toQuestion2",
        "Answer5toQuestion3",
        "Answer5toQuestion4",
        "Answer5toQuestion5",
    }
};


int read_mcq(void)
{
    char *filename = "mcq.txt";
    FILE *fp = fopen(filename, "r");
    
    uint8_t line;
    uint8_t question;
    // uint8_t answer;


    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        return 1;
    }

    // reading line by line, max 256 bytes
    // const unsigned MAX_LENGTH = 256;

    line = 0;
    question = 0;
    
    while (fgets(buffer, MAX_CHARS, fp) && question<5)
    {
        printf("%d\n", line);
        printf("%s\n", buffer);
        if(!line)
        {
            strcpy(questions[question],buffer);
        }
        else
        {
            strcpy(answers[question][line-1],buffer);
        }
        ++line;
        if(line==6)
        {
            line=0;
            ++question;
        }
    }

        
        // ++answer;
        // if(answer==6)
        // {
            // answer = 0;
            // ++question;
        // }
        // else
        // {
            // strcpy(answers[question][answer],buffer);
        // }
        // if(question==5)
            // break;
        

    
    // close the file
    fclose(fp);
    printf("mcq file read");
    return 0;
}


void write_shuffled_mcqs(void)
{
    uint8_t version;
    
    uint8_t question;
    
    uint8_t answer;
    
    
    char *filename = "versions.txt";
    
    FILE *fp = fopen(filename, "w");
        
    for(version=0;version<5;++version)
    {
        

        
        printf("Writing version: %d\n", version);
        // filename[7] = version+48;
        
        printf("File name: %s\n", filename);
        
        for(question=0;question<5;++question)
        {
            // printf("Wrting question: %d\n", question);
            // fprintf(fp,"Question %d: %s\n",question,"Hello world!");
            fprintf(fp,"Question %d: %s\n",question,questions[shuffle[question][version]]);
            // fprintf(fp,"%s", shuffle[question][version]);
            printf("Question: %s\n", questions[shuffle[question][version]]);

            for(answer=0;answer<5; ++answer)
            {
                // fprintf(fp,"%s",answers[shuffle[question][version]][shuffle[answer][version]]);
                fprintf(fp,"Answer %d: %s\n", answer, answers[shuffle[question][version]][shuffle[answer][version]]);

                printf("Answer: %s\n", answers[shuffle[question][version]][shuffle[answer][version]]);
            }
        }
        
    }
    fclose(fp);

}


int main(void)
{
    uint8_t version;
    uint8_t question;
    uint8_t answer;
    
    
    read_mcq();
    
    for(version=0;version<5;++version)
    {
        for(question=0;question<5;++question)
        {
            printf("Question: %s\n", questions[shuffle[question][version]]);
            for(answer=0;answer<5;++answer)
            {
                printf("Answer: %s\n", answers[shuffle[question][version]][shuffle[answer][version]]);
            }
            printf("-----\n");
        }
        printf("-----------------------------------------\n");
    }
    write_shuffled_mcqs();
    while(1){};
    return 0;
}