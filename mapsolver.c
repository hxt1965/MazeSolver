/**
+ * name: mapsolver.c
+ * author: Harsh Tagotra
+ * date: 11/3/2018
+ *
+ * reads a maze from a given input file or stdin and processes it
+ * for various display options like pretty print, show path and 
+ * calculate shortest distance to get to the end of the maze/
+ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "queue.h"

/**
 * a struct representing one location or one point in the maze
 * rCord = row coordinate
 * cCord = column coordinate
 * value = 0 or 1 (open or blocked)
 * rep = character representation '.', '+', or '#'
 */
struct Location 
{
    int rCord;
    int cCord;
    int value;
    char rep;
};

//1d array representing the maze in a 2d form
struct Location* maze;
//no. of rows in the maze
size_t rows;
//no. of columns in the maze
size_t cols;
//constant defined for use in bfs
int min_dist = INT_MAX;
//queue containing all the visited cells of the maze
queue *q;
//row and cols defined coordinates of all the neighbouring 
//cells wrt a certain cell
int rowPts[] = {1, 0, 0, -1};
int colPts[] = {0, 1, -1, 0};

//implicit declaration of the function
int getIdx(size_t i, size_t j);

/*
 * checks if a given row and column is within the maze or not
 */
bool isValidLoc(int r, int c)
{
    return r<(int)rows && c<(int)cols && r>=0 && 
        c>=0 ? true : false;
}

/**
 * prints the maze
 * @prettyFlag: flag that decides whether to show the path or not
 */
void printMaze(int prettyFlag, char outFile[])
{
    FILE *fp;
    int fileFlag = 0;
    if(strcmp(outFile, "")!=0)
    {
        fp = fopen(outFile, "w");
        fileFlag = 1;
        if(fp == NULL)
        {
            fprintf(stderr, "Error opening file!");
            exit(1);
        }    
    }

    //printing top border
    for(size_t i = 0; i<cols*2+1; i++)
       fprintf(fileFlag==1 ? fp : stdout, 
               i==0 ? "\n|-" : i==cols*2 ? "-|\n" : "-"); 
    
    for(size_t i=0; i<rows; i++)
    {
        fprintf(fileFlag==1 ? fp : stdout, 
                i==0 ? "  " : "| ");
        for(size_t j=0; j<cols; j++)
        {
            if(prettyFlag==1)
                fprintf(fileFlag==1 ? fp : stdout,
                        maze[getIdx(i, j)].value==0 ? ". " : "# ");
            else
                fprintf(fileFlag==1 ? fp : stdout,
                        "%c ", maze[getIdx(i, j)].rep);
        }
        fprintf(fileFlag==1 ? fp : stdout, 
                i==rows-1 ? " \n" : "|\n");
    }
    
    //printing bottom border
    for(size_t i = 0; i<cols*2+1; i++)
       fprintf(fileFlag==1 ? fp : stdout, 
               i==0 ? "|-" : i==cols*2 ? "-|\n" : "-");
    if(fileFlag==1)
        fclose(fp);
}

/**
 * reads the input from a given file 'inpFile'
 */
void read( char* inpFile )
{
    char *ptr, buf[5000], *ch;
    FILE *fp;
    size_t cntr=0, cntc, cntele=0;
    fp = NULL;
    //no. of rows and cols assumed at the beggining
    //these will eventually get updated to the 
    rows = 2;
    cols = 2;
    int fileFlag = 0;

    maze = malloc(rows * cols * sizeof(struct 
                Location));
 
    if(!strcmp(inpFile, "")==0) {
        fp = fopen(inpFile, "r");
        fileFlag = 1;
    }
    if(!strcmp(inpFile, "")==0 && fp==NULL)
    {
        perror( inpFile );
        exit ( EXIT_FAILURE );
    }
    else 
    {       
        while( (ptr = fgets(buf, 5000, fp==NULL ? stdin : fp)) != NULL )
        {
            //if set no. of rows are less than actual number of rows
            if(strcmp(buf, "")==0)
                break;
            if(cntr >= rows)
            {
                struct Location *mazeTemp = realloc(maze, ((2*rows*cols) 
                            * sizeof(struct Location)));

                if(mazeTemp==NULL)
                {
                    printf("Cannot allocate more memory.\n");
                    exit(1);
                }
                else
                    maze = mazeTemp;
                rows = rows*2;
            }

            ch = strtok(buf, " ");
            cntc=0;
            while(ch!=NULL && (*ch=='1' || *ch=='0'))
            {
                if(cntc > cols)
                {
                    struct Location *mazeTemp =  realloc(maze, 
                            ((2*rows*cols) * sizeof(struct Location)));
                    if(mazeTemp==NULL)
                    {
                        printf("Cannot allocate more memory.\n");
                        exit(1);
                    }
                    else
                        maze = mazeTemp;
                    cols = cols*2;
                }
                //each element is initialized in the 1d array
                maze[cntele].rCord = cntr;
                maze[cntele].cCord = cntc;
                maze[cntele].rep = *ch=='1' ? '#' : '.';
                maze[cntele++].value = (int)*ch-'0';
                //cntc is constantly incremented to keep track
                //of supposed cols
                cntc++;  
                ch = strtok(NULL, " ");
            }
            //similarly, cntr is updated 
            cntr++;
        }

        //global variables rows and cols are updated 
        if(rows>cntr)
            rows = cntr;
        if(cols>cntc)
            cols = cntc;
    }
    if(fileFlag==1)
        fclose(fp);
}

/*
 * returns the index of the cell in the 1d array according
 * to a row and column index
 */
int getIdx(size_t i, size_t j)
{
    return i*cols + j;
}

/**
 * creates a path from start to finish cell and updates 
 * the character representation for the cells in the path
 */
bool createPath(int x, int y, bool visited[rows][cols])
{
    
    /**if(!isValidLoc(x, y))
        return false;*/
    if((size_t) x==rows-1 && (size_t) y==cols-1)
        return true;
    //if(maze[getIdx(x, y)].value==1)
    //    return false;
    maze[getIdx(x, y)].rep = '+';
   visited[x][y] = true;

    if(isValidLoc(x+1, y) && maze[getIdx(x+1, y)].value==0)
        if( createPath(x+1, y, visited)==true)
            return true;
    if(isValidLoc(x, y+1) && maze[getIdx(x, y+1)].value==0)
        if(createPath(x, y+1, visited)==true)
            return true;
    if(isValidLoc(x-1, y) && maze[getIdx(x-1, y)].value==0) 
        if(createPath(x-1, y, visited)==true)
            return true;
    if(isValidLoc(x, y-1) && maze[getIdx(x, y-1)].value==0)
        if(createPath(x, y-1, visited)==true)
            return true;

    maze[getIdx(x, y)].rep = '.';
    return false;
    /**
    node* curr = back(qvisited);
    struct Location l = *(struct Location *)curr->ele;
    
    maze[getIdx(0, 0)].rep = '+';
    maze[getIdx(rows-1, cols-1)].rep = '+';
    
    while(( l.cCord + l.rCord ) != 1)
    {
        //maze[getIdx(l.rCord, l.cCord)].rep = '+';
        //curr = popLast(qvisited);
        Node* loc = getLastNode(qvisited);
        l = *(struct Location *) popLast(qvisited);
        maze[getIdx(l.rCord, l.cCord)].rep = '+';
        //printf("%d %d\n", (int)l.rCord, (int)l.cCord);
    }*/
}

/**
 * uses breadth-first-search to get the shortest distance 
 * between start and finish cell
 */
int Bfs()
{
    queue *qvisited;
    //check if start and finish cells are blocked
    if(maze[getIdx(0, 0)].value!=0 || maze[getIdx(rows-1, cols-1)].value!=0)
        return -1;

    //setting all visited cells to false after creating
    bool visited[rows][cols];
    memset(visited, false, sizeof(visited));

    visited[0][0] = true;

    q = create();
    qvisited = create();
    
    //first element is pushed and distance is updated
    if(push(q, (eleP) &maze[getIdx(0, 0)], 1)!=1 && 
            push(qvisited, (eleP) &maze[getIdx(0, 0)], 1)!=1)
        fprintf(stderr, "push error!\n");

    while(isempty(q)!=1)
    {
        node* curr = top(q);
        struct Location l = *(struct Location *) curr->ele;

        //if destination reached
        if(l.rCord == (int) rows-1 && l.cCord == (int)cols-1) 
        {
            //function called to create the pat
            memset(visited, false, sizeof(visited));
            createPath(0, 0, visited);
            destroy(q);
            destroy(qvisited);
            return curr->dist;
        }

        pop(q);

        //for all neighbouring cells..
        for(int i = 0; i<4; i++)
        {
            int rr = l.rCord + rowPts[i];
            int cc = l.cCord + colPts[i];

            //validity checked and added to visited queue
            if(isValidLoc(rr, cc) && maze[getIdx((size_t)rr, 
                        (size_t)cc)].value==0 && !visited[rr][cc])
            {
                visited[rr][cc] = true;
                push(q, (eleP) &maze[getIdx((size_t)rr, (size_t)cc)], 
                    curr->dist + 1);
                push(qvisited, (eleP) &maze[getIdx((size_t)rr, (size_t)cc)],
                    curr->dist + 1);

           }
        }
    }
    //if path not found, -1 returned
   return -1;
}

/*
 * command line arguments are read through and respective actions
 * are performed
 */
int main ( int argc, char * argv[] )
{
    int flagh=0, flagd=0, flags=0, flagp=0;
    char *inpFile="", *outFile="";
    char fl=' ';

    for(size_t i = 1; i<(size_t)argc; i++)
    {
        if(argv[i][0]=='-')
            fl = argv[i][1];
        switch(fl)
        {
            case 'i':
            i++;
            if(i<(size_t)argc && argv[i][0]!='-')
                inpFile = argv[i];
            else 
                fprintf(stdout, "%s%s","USAGE:\nmapsolver [-hdsp] [-i INFILE]",
                   " [-o OUTFILE]\n");
            break;

            case 'o':
            i++;
            if(i<(size_t)argc && argv[i][0]!='-')
                outFile = argv[i];
            else
                fprintf(stdout, "%s%s",
                        "USAGE:\nmapsolver [-hdsp] [-i INFILE]",
                   " [-o OUTFILE]\n");
            break;

            default:
            ; size_t j=1;
            while((j)<strlen(argv[i]))
            {
                printf("i");
                char cc = argv[i][j];
                if(cc=='h')
                    flagh = 1;
                else if(cc=='d')
                    flagd = 1;
                else if (cc=='s')
                    flags = 1;
                else if (cc=='p')
                    flagp = 1;
                else
                {
                    fprintf(stderr, "%s%s", 
                                "USAGE:\nmapsolver [-hdsp] [-i INFILE]", 
                                " [-o OUTFILE]\n");
                    //return 0;
                }
                j++;
            }
        }
    }

    read(inpFile);

    if(flagh==1)
    {
        printf("USAGE:\nmopsolver [-hdsp] [-i INFILE] [-o OUTFILE]\n\n");
        printf("%s%s","Options:\n\t-h\tPrint this helpful message to ", 
                "stdout and exit");
        printf("%s%s", 
                "\n\t-d\tPretty print (display) the maze after reading.",
                "\tDefault: off)");
        printf("\n\t-s\tPrint shortest solution steps\t\t(Default: off)");
        printf("\n\t-p\tPrint an optimal path.\t\t\t(Default: off)");
        printf("\n\t-i INFILE\tRead maze from INFILE\t\t(Default: stdin)");
        printf("%s%s", "\n\t-o OUTFILE\tWrite all output to OUTFILE\t\t", 
                "(Default: stdout)");
    }
    if(flagd==1)
        printMaze(0, outFile);
    if(flags==1)
        printf("Solution in %d steps.", Bfs());
    if(flagp==1)
        printMaze(1, outFile);
}
