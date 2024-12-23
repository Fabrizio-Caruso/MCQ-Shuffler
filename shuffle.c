
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define QUESTION_DASH    0
#define QUESTION_NUMBER  1

#define ANSWER_DASH      1
#define ANSWER_NUMBER    0

// Range: 1-5
#define VERSIONS         5

static uint8_t shuffle[5][5] =
{
    {0, 1, 2, 3, 4},
    {1, 0, 3, 4, 2},
    {2, 4, 1, 0, 3},
    {4, 3, 0, 2, 1},
    {3, 2, 4, 1, 0},
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

    fclose(fp);
    printf("mcq file read");
    return 0;
}


void write_shuffled_mcqs(void)
{
    uint8_t version;
    
    uint8_t question;
    
    uint8_t answer;
    
    
    char filename[30];
    
    strcpy(filename,"./versions/version?.txt");

    for(version=0;version<VERSIONS;++version)
    {
        
        filename[18] = 48 + version + 1;
        FILE *fp = fopen(filename, "w");

        printf("Writing version: %d\n", version+1);
        
        printf("File name: %s\n", filename);
        
        for(question=0;question<5;++question)
        {
            if(QUESTION_NUMBER)
            {
                fprintf(fp,"\n%d. %s\n",question+1, questions[shuffle[question][version]]);
            }
            else if(QUESTION_DASH)
            {
                fprintf(fp,"\n%s\n",questions[shuffle[question][version]]);
            }
            else
            {
                fprintf(fp,"\n- %s\n",questions[shuffle[question][version]]);
            }
            printf("%s\n", questions[shuffle[question][version]]);

            for(answer=0;answer<5; ++answer)
            {
                if(ANSWER_NUMBER)
                {
                    fprintf(fp,"%d. %s\n", answer+1, answers[shuffle[question][version]][shuffle[answer][version]]);
                }
                else if(ANSWER_DASH)
                {
                    fprintf(fp,"- %s\n", answers[shuffle[question][version]][shuffle[answer][version]]);
                }
                else
                {
                    fprintf(fp,"%s\n", answers[shuffle[question][version]][shuffle[answer][version]]);
                }
                printf("%s\n", answers[shuffle[question][version]][shuffle[answer][version]]);
            }
        }
        fclose(fp);

    }

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
    // while(1){};
    return 0;
}