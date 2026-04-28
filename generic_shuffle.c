
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define QUESTION_DASH    0
#define QUESTION_NUMBER  1

#define ANSWER_DASH      1
#define ANSWER_NUMBER    0

// TODO: Generalize this program to several question/answer values.

// Range: 1-5
#define VERSIONS      5

// QUESTIONS >= VERSIONS
#define QUESTIONS     5

#define QUESTION_LINE ((QUESTIONS)+1)

// 5x5 non-cyclic Latin Square


uint8_t (*shuffle)[5]; 

static uint8_t shuffle4[4][4] =
{
    {0,1,2,3},
    {1,0,3,2},
    {2,3,1,0},
    {3,2,0,1}
};

static uint8_t shuffle5[5][5] =
{
    {0,1,2,3,4},
    {1,2,4,0,3},
    {2,4,3,1,0},
    {3,0,1,4,2},
    {4,3,0,2,1}
};

static uint8_t shuffle6[6][6] =
{
    {0,1,2,3,4,5},
    {5,3,1,2,0,4},
    {1,5,3,4,2,0},
    {2,0,4,5,1,3},
    {4,2,5,0,3,1},
    // {3,4,0,1,5,2}
};


static uint8_t shuffle7[7][7] =
{
    {0,1,2,3,4,5,6},
    {1,0,4,5,6,2,3},
    {4,3,5,0,1,6,2},
    {5,6,1,2,3,0,4},
    {2,4,6,1,0,3,5},
    // {3,2,0,6,5,4,1},
    // {6,5,3,4,2,1,0},
};


static uint8_t shuffle8[8][8] =
{
    {0,1,2,3,4,5,6,7},
    {5,4,3,2,6,0,7,1},
    {6,5,4,1,7,2,0,3},
    {2,6,0,7,5,1,3,4},
    {7,3,1,4,2,6,5,0},
    {4,0,7,6,1,3,2,5},
    // {3,7,6,5,0,4,1,2},
    // {1,2,5,0,3,7,4,6}
};


static uint8_t shuffle9[9][9] =
{
    {0,1,2,3,4,5,6,7,8},
    {6,4,0,8,7,1,2,5,3},
    {2,8,5,1,0,7,3,6,4},
    {4,7,6,5,1,3,0,8,2},
    {1,6,4,2,3,8,5,0,7},
    {8,3,1,4,6,0,7,2,5},
    // {3,2,7,6,5,4,8,1,0},
    // {7,5,3,0,8,2,1,4,6},
    // {5,0,8,7,2,6,4,3,1}
};


static uint8_t shuffle10[10][10] =
{
    {0,1,2,3,4,5,6,7,8,9},
    {4,7,0,6,9,2,3,8,1,5},
    {3,4,7,9,2,6,5,1,0,8},
    {9,3,1,8,7,4,2,5,6,0},
    {5,6,8,2,1,9,4,0,7,3},
    // {1,5,3,4,8,7,0,9,2,6},
    // {6,0,5,7,3,8,1,4,9,2},
    // {8,2,9,1,6,0,7,3,5,4},
    // {2,8,4,5,0,1,9,6,3,7},
    // {7,9,6,0,5,3,8,2,4,1}
};


#define MAX_TITLE 900
#define MAX_CHARS 9000


char buffer[MAX_CHARS];


char title[MAX_TITLE];

char questions[5][MAX_CHARS];

char answers[5][5][MAX_CHARS];


int read_mcq(void)
{
    char *filename = "mcq.txt";
    FILE *fp = fopen(filename, "r");
    
    uint8_t line;
    uint8_t question;
    size_t offset;
    size_t buffer_size;
    uint8_t empty_line;

    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        return 1;
    }

    offset = 0;

    while (fgets(buffer, MAX_TITLE, fp))
    {
        if(strcmp(buffer,"\n"))
        {
            buffer_size = strlen(buffer);
            strcpy(title+offset,buffer);
            offset+=buffer_size;
        }
        else
        {
            break;
        }
    }
    // fgets(buffer, MAX_TITLE, fp);
    printf("Title: %s\n-------------------", title);
    // strcpy(title, buffer);

    line = 0;
    question = 0;
    buffer_size = 0;
    offset = 0;
    empty_line = 1;
    
    while (fgets(buffer, MAX_CHARS, fp) && question<5)
    {
        buffer_size = strlen(buffer);
        if(strcmp(buffer,"\n"))
        {
            
            printf("Line   : %d\n", line);
            printf("Buffer : %s\n", buffer);
            if(!line)
            {
                // strcpy(questions[question]+offset," ");
                strcpy(questions[question]+offset,buffer);

            }
            else
            {
                // strcpy(answers[question][line-1]," ");
                strcpy(answers[question][line-1]+offset,buffer);
            }
            offset+=buffer_size;
            empty_line = 0;
        }
        else
        {
            printf("Empty line\n");
            
            if(!empty_line)
            {
                offset = 0;
                ++line;
                
                if(line==QUESTION_LINE)
                {
                    line=0;
                    ++question;
                }
                empty_line=1;
            }
        }
    
    }

    fclose(fp);
    printf("mcq file read\n");
    return 0;
}


void write_ASCII_shuffled_mcqs(void)
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
        
        fprintf(fp,"%s\n", title);
        
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


void printHTMLText(FILE *f, const char *text)
{
  for (; *text; ++text) {
    switch (*text) {
      case '<': fprintf(f, "&lt;"); break;
      case '>': fprintf(f, "&gt;"); break;
      case '&': fprintf(f, "&amp;"); break;
      case '"': fprintf(f, "&quot;"); break;
      case '\'': fprintf(f, "&apos;"); break;
      default: putc(*text, f);
    }
  }
}

void printQuestion(FILE *f, const char *text)
{
    fprintf(f, "<h3><br>");
    printHTMLText(f,text);
    fprintf(f,"</h3>");
}


void printAnswer(FILE *f, const char *text)
{
    fprintf(f,
    "<tr><!-- start of table row -->\n"
    "<td>%s</td><!-- number -->\n"
    "<td>",
    "&#x25A1;");
    printHTMLText(f, text);
    fprintf(f,
    "</td><!-- Title -->\n"
    "</tr><!-- end of table row -->\n");
}



void write_HTML_shuffled_mcqs(void)
{
    uint8_t version;
    uint8_t question;
    uint8_t answer;
    char filename[30];

    
    strcpy(filename,"./versions/version?.html");

    for(version=0;version<VERSIONS;++version)
    {
        
        filename[18] = 48 + version + 1;
        FILE *fp = fopen(filename, "w");
        
        fprintf(fp,
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "<title>");
        printHTMLText(fp, title);
        fprintf(fp,
        "</title>\n"
        "</head>\n");


        fprintf(fp,
        "<body>\n");
        fprintf(fp, "<h1>\n");
        fprintf(fp, title);
        fprintf(fp, "</h1>\n");
        
        for(question=0;question<5;++question)
        {
            printQuestion(fp, questions[shuffle[question][version]]);
            
            fprintf(fp,
            "<table><!-- start of table -->\n"
            "<tr><!-- start of table head row -->\n"
            "</tr><!-- end of table head row -->\n");
            
            for(answer=0;answer<5;++answer)
            {
                printAnswer(fp, answers[shuffle[question][version]][shuffle[answer][version]]);
            }
            fprintf(fp,
            "</table><!-- end of table -->\n");        
        }

        fprintf(fp,
        "</body>\n"
        "</html>\n");
        fclose(fp);
    }


}


int main(void)
{
    uint8_t version;
    uint8_t question;
    uint8_t answer;
    
    if(QUESTIONS==5)
    {
        shuffle = shuffle5;
    }
    read_mcq();
    
    for(version=0;version<VERSIONS;++version)
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
    write_ASCII_shuffled_mcqs();
    
    write_HTML_shuffled_mcqs(); 

    
    return 0;
}