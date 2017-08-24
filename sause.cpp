
#if !defined(Linux)
#include <windows.h>           //Not Linux must be windows
#endif
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <tuple>
using namespace std;


//the global structure
// THIS IS WHERE ALL GLOBAL VARIABLES WILL BE HELD
typedef struct {
    double mutationrate;
    double crossRate;
    int popSize; //population size
    int nPoints;// number of points
    int maxGen;// max generations
    int min_el; // elitism
    int ** point_coord; // 2d array of coordinates of points
    int **weightMatrix;
    int **tempBuffer; // this will hold a temperary buffer
    int **CurrentPopulation; // have the current populatoin
    int x, y; // window size
    int count;

} glob;
glob global;

//initializes wieght matrix that contains the distances
void initialize_weight_matrix() {
    global.weightMatrix = new int*[global.nPoints];
    for (int i = 0; i < global.nPoints; i++)
        global.weightMatrix[i] = new int[global.nPoints];// x and y // matrix

}

// initialize dynamic 2D array that contains points
void initialize_coordinate_system() {
    global.point_coord = new int*[global.nPoints];
    for (int i = 0; i < global.nPoints; i++)
        global.point_coord[i] = new int[2];// x and y
    //makes weight mtrix too


}





//this boolean will check if the coordinates are unique;
boolean UniqueCheck(int x, int y) {
    boolean unique = true;
    //iterates through all current cooradinates and compares
    for (int i = 0; i < global.nPoints; i++) {

        if ((global.point_coord[i][0] = !x) || (global.point_coord[i][1] != y)) {
            unique = false;
            break;
        }

    }
    return unique;
}

//creates N uniques points
void initialize_points() {
    int x;
    int y;
    boolean exists = true; // used to finding unique points
    for (int i = 0; i < global.nPoints; i++) {
        while (exists != false) {
            x = rand() % global.x;
            y = rand() % global.y;
            exists = UniqueCheck(x, y);
        }
        global.point_coord[i][0] = x;
        global.point_coord[i][1] = y;
        exists = true;//resets boolean
    }
}


void Display_coordinates(void) {// will display current coordinates on screen
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(1); // increase pixel size
    glColor3f(1.0, 1.0, 1.0); // makes points white
    glBegin(GL_POINTS);

    int x = 20;
    int y = 20;
    for (int i = 0; i < global.nPoints; i++) {
        x = global.point_coord[i][0];
        y = global.y - global.point_coord[i][1];
        glVertex2i(x, y);
    }
    glEnd();
    glFlush();
}



// finds the euclean distance of 2 points
int EucleanDistance(int x0, int x1, int y0, int y1) {
    int distance;
    //euclean distance
    distance = ((x1 - x0)*(x1 - x0)) + ((y1 - y0)*(y1 - y0));

    return distance;
}


//this will fill the matix with weights
void initiateMatrixArray() {
    int n = global.nPoints; // number of points
    int x, x1, y, y1; // initial;ize ints here
    int distance;
    for (int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {

            x =global.point_coord[i][0] ;
            y = global.point_coord[i][1];
            x1 = global.point_coord[j][0];
            y1 = global.point_coord[j][1];
            distance = EucleanDistance(x, x1, y, y1);

            global.weightMatrix[i][j] = distance; // put distance to matrix
            global.weightMatrix[j][i] = distance; // since it is identical , replicate the other half

        }
    }



}



void ClearBuffer() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    int p1; //point 1
    int p2;// point 2;
    //index 0 is reserves
    int x, x1, y, y1;
    for (int i = 1; i < global.nPoints; i++) {
        //do stuff here
        p1 = global.CurrentPopulation[global.min_el][i];
        p2 = global.CurrentPopulation[global.min_el][i - 1];
        x = global.point_coord[p1][0];
        x1 = global.point_coord[p2][0];
        y = global.y - global.point_coord[p1][1];
        y1 = global.y - global.point_coord[p2][1];
        glVertex2i(x, y);
        glVertex2i(x1, y1);

    }
    //print last vtex;

    p2 = global.CurrentPopulation[global.min_el][0];
    x1 = global.point_coord[p2][0];
    y1 = global.y - global.point_coord[p2][1];
    glVertex2i(x, y);
    glVertex2i(x1, y1);


    glEnd();
    glFlush();

}




//will display the best chromosomes in the population
void display_print() {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    double min = global.CurrentPopulation[0][global.nPoints];
    int max_index = 0;
    int sum = 0;
    //this will traverse through the population and find the best
    for (int i = 0; i < global.popSize; i++) {
        if (global.CurrentPopulation[i][global.nPoints] < min) {
            //change max index
            min = global.CurrentPopulation[i][global.nPoints];
            max_index = i;
            //change new minimum

        }

    }

    global.min_el = max_index;
    int p1; //point 1
    int p2;// point 2;
    //index 0 is reserves
    int x, x1, y, y1;
    for (int i = 1; i < global.nPoints; i++) {
        //do stuff here
        p1 = global.CurrentPopulation[max_index][i];
        p2 = global.CurrentPopulation[max_index][i - 1];
        x = global.point_coord[p1][0];
        x1 = global.point_coord[p2][0];
        y = global.y - global.point_coord[p1][1];
        y1 = global.y - global.point_coord[p2][1];
        glVertex2i(x, y);
        glVertex2i(x1, y1);

    }
    //print last vtex;

    p2 = global.CurrentPopulation[max_index][0];
    x1 = global.point_coord[p2][0];
    y1 = global.y - global.point_coord[p2][1];
    glVertex2i(x, y);
    glVertex2i(x1, y1);

    glEnd();
    glFlush();
//	printf("gen best");
    //for (int i = 0; i < global.nPoints; i++) {
    //	printf("%d ",global.CurrentPopulation[max_index][i]);
    //}
//	printf(" \n");
}









//allocates memory for temop buffer;
void allocateTempBuffer() {

    global.tempBuffer = new int*[global.popSize];
    for (int i = 0; i < global.popSize; i++)
        global.tempBuffer[i] = new int[global.nPoints];// x and y // matrix


}
//allocates memory for current Population;
void allocateCurrentPop() {
    //n is popsize
    global.CurrentPopulation = new int*[global.popSize];
    for (int i = 0; i < global.popSize; i++)
        global.CurrentPopulation[i] = new int[global.nPoints+1];// x and y // matrix


}


//genetic algorithm evaluation function
void EvaluateDistance() {
    int sum = 0;
    int c = 0;;
    int index1;
    int index2;
    for (int i = 0; i < global.popSize; i++) {
        for (int j = 0; j < global.nPoints; j++) {
            if (j == (global.nPoints - 1)) {

                // just to make things eve
            }
            else {
                c = sum;
                index1 = global.CurrentPopulation[i][j];
                index2 = global.CurrentPopulation[i][j+1];

                sum = c + global.weightMatrix[index1][index2];
            }

        }
        //adds the fists and last distance because thats the traveling saleman problem
        global.CurrentPopulation[i][global.nPoints] = sum + global.weightMatrix[0][global.nPoints - 1];
        sum = 0; //resets sum
    }


}


// this renaomizes chromosomes
void raondomize_chromes(int **array) {
    // takes in 2d array
    for (int i = 0; i < global.nPoints; i++) {
        double rr = (double)rand() / (double)RAND_MAX;
        int random = i + (int)(rr * (global.nPoints - i));

        int rand = array[0][random];
        array[0][random] = array[0][i];
        array[0][i] = rand;
    }
}

void GenerateInitialPopulation() {
    allocateCurrentPop(); // makes emory space for current population

    // set current population;
    int **unsorted = new int*[0];//array size
    unsorted[0] = new int[global.nPoints]; // array length

    for (int i = 0; i < global.nPoints; i++) {
        unsorted[0][i] = i; // set chromosomes in order
    }


    // set initial chromosomes for the population at random
    for (int i = 0; i < global.popSize; i++) {
        //randomize chromosomes
        raondomize_chromes(unsorted);
        for (int j = 0; j < global.nPoints; j++) {
            global.CurrentPopulation[i][j] = unsorted[0][j]; // set to population
        }

    }

}


int find_valid(int index, int index2, int **cmatrix) {
    int rval = 0; //return value;
    boolean found = false;
    int count = 0; //for while loop
    int t;
    for (int i = 0; i < global.nPoints; i++) {
        t = cmatrix[index][i];
        count = 0;
        while (count < global.nPoints) {
            if (t == cmatrix[index2][count]) {
                break;
            }
            if ((count == global.nPoints - 1)&&cmatrix[index2][count]!=t) {

                found = true;
            }
            count++;
        }// end of while loop

        if (found==true) {//if true
            rval = t;
            break; //break from loop
        }

    }

    return rval;
}


//moves elites to new population of genertion
void move_elites(int number) {

    for (int i = 0; i < global.nPoints; i++) {
        global.tempBuffer[number][i] = global.CurrentPopulation[global.min_el][i];
    }
}


void temp_to_current() {
//	printf("curretn POP\n");
    int move;
    for (int i = 0; i < global.popSize; i++) {
        for (int j = 0; j < global.nPoints; j++) {

            move = global.tempBuffer[i][j];
            if (move == global.nPoints + 1) {
                move = 0;
            }
            global.CurrentPopulation[i][j] = move;
            //	printf(" %d", global.CurrentPopulation[i][j]);
        }
        //	printf("----------\n");
    }

}
//this will preform the actual corssover based on the bit mast(arithmic)
void AcrossOver(int p1, int p2, int pl1, int pl2) {

    int **cmatrx = new int*[5];
    //set 5 levels
    int Irandom;//int random
    double dRandom;//double random
    for (int i = 0; i < 5; i++)
        cmatrx[i] = new int[global.nPoints];

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < global.nPoints; j++) {

            switch (i) {
            case 0:
                if (global.CurrentPopulation[p1][j] == 0) {
                    cmatrx[i][j] = global.nPoints + 1;
                }
                else {
                    cmatrx[i][j] = global.CurrentPopulation[p1][j];
                }
                break;
            case 1:
                if (global.CurrentPopulation[p2][j] == 0) {
                    cmatrx[i][j] = global.nPoints + 1;
                }
                else {
                    cmatrx[i][j] = global.CurrentPopulation[p2][j];
                }
                break;
            case 2:
                Irandom = (int)rand() % 100;
                if (Irandom % 2 == 1) {
                    Irandom = 0;
                }
                else {
                    Irandom = 1;
                    cmatrx[3][j] = cmatrx[0][j];
                    cmatrx[4][j] = cmatrx[1][j];
                }
                cmatrx[i][j] = Irandom; //bit mask
                break;

            }// end switch


        }// end of for loop(jj)
    }// end of for loop(i)

    int temp;
    //fill missing values
    for (int i = 0; i < global.nPoints; i++) {
        if (cmatrx[2][i] == 0) {
            temp = find_valid(0, 4, cmatrx);
            cmatrx[4][i] = temp;
            temp = find_valid(1, 3, cmatrx);
            cmatrx[3][i] = temp;
        }
    }

    //traverse through matrix
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < global.nPoints; j++) {
            if (cmatrx[i][j] > global.nPoints + 1) {
                //chnage to 0
                cmatrx[i][j] = 0;
            }

        }
    }


    //muations time
    dRandom = (double)rand() / (double)RAND_MAX;
    int Irandom2;
    if (dRandom < global.mutationrate) {
        //swap 2 random variables
        Irandom = (int)rand() % global.nPoints;
        Irandom2 = (int)rand() % global.nPoints;
        temp = cmatrx[3][Irandom];
        cmatrx[3][Irandom] = cmatrx[3][Irandom2];
        cmatrx[3][Irandom2] = temp;


    }
    //swap 2 more for cinsistency
    dRandom = (double)rand() / (double)RAND_MAX;
    if (dRandom < global.mutationrate) {
        //swap 2 random variables
        Irandom = (int)rand() % global.nPoints;
        Irandom2 = (int)rand() % global.nPoints;
        temp = cmatrx[4][Irandom];
        cmatrx[4][Irandom] = cmatrx[4][Irandom2];
        cmatrx[4][Irandom2] = temp;


    }

    //cross overs

    dRandom = (double)rand() / (double)RAND_MAX;
    double dRandom2 = (double)rand() / (double)RAND_MAX;
    if (pl1 == global.nPoints - 1) {
        if (dRandom > global.crossRate) {
            for (int i = 0; i < global.nPoints; i++) {
                global.tempBuffer[pl1][i] = cmatrx[0][i];//0
            }
        }
        else {
            for (int i = 0; i < global.nPoints; i++) {
                global.tempBuffer[pl1][i] = cmatrx[3][i];//3
            }
        }//end of else
    }
    else {
        if (dRandom > global.crossRate) {
            for (int i = 0; i < global.nPoints; i++) {
                global.tempBuffer[pl1][i] = cmatrx[0][i];//0
            }
        }
        else {
            for (int i = 0; i < global.nPoints; i++) {
                global.tempBuffer[pl1][i] = cmatrx[3][i];//3
            }

        }

        if (dRandom2 > global.crossRate) {
            for (int i = 0; i < global.nPoints; i++) {
                global.tempBuffer[pl2][i] = cmatrx[1][i];//1
            }
        }
        else {
            for (int i = 0; i < global.nPoints; i++) {
                global.tempBuffer[pl2][i] = cmatrx[4][i];//4
            }

        }

    }//end of else






}



void Roullete(int i1, int i2) {

    int par1, par2;
    int rand1, rand2;

    rand1 = (double)rand() / (double)RAND_MAX;
    rand2 = (double)rand() / (double)RAND_MAX;

    if (rand1<.3) {
        par1 = global.min_el;
    }
    else {

        par1 = (int)(rand2*(global.popSize));

        if (par1 == 0) {
            par1 = 1;
        }
        if (par1 == global.popSize) {
            par1 = global.popSize - 1;
        }
    }
    //parent_2
    if (rand2<.3) {
        par2 = global.min_el;
    }
    else {

        par2 = (int)(rand2*(global.popSize));
        if (par2 == 0) {
            par2 = 1;
        }
        if (par2 == global.popSize) {
            par2 = global.popSize-1;
        }
    }
    if (i2 == global.popSize) {
        i2 = i2 - 1;
    }
    AcrossOver(par1, par2, i1, i2);
}


//this will preform a crossover of the chromosomes
void crossOver() {

    double elitism = 0.005;
    //selects the perecentage of population going into new generation of the best
    int Eselect = (int)(global.popSize*elitism);
    if (Eselect % 2 == 1) {
        // to make things even
        Eselect = Eselect - 1;
    }

    for (int i = 0; i < Eselect; i++) {
        //move to new population
        move_elites(i);
    }

    for (int j = Eselect; j < global.popSize; j++) {

        if (j % 2 == 0) {
            //roulette selection
            Roullete(j, j + 1);
        }
    }

}

void Genetic_Algorith() {
    // add some parameters
    allocateTempBuffer(); // allocates memory for a tmeop buffer
    GenerateInitialPopulation(); // genetates initial population
    EvaluateDistance();
    display_print();


    int count = 1;
    while (count != global.maxGen) {

        crossOver();

        temp_to_current();
        EvaluateDistance();
        ClearBuffer();
        display_print();
        //	Sleep();
        count++;
    }

}
void finalImage() {

    //draws background
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    glColor3ub(255, 238, 173);
    glVertex2i(0, global.y);
    glColor3ub(255, 238, 173);
    glVertex2i(global.x, global.y);
    glColor3ub(255, 238, 173);
    glVertex2i(global.x, 0);
    glColor3ub(255, 238, 173);
    glVertex2i(0, 0);
    glEnd();



    glColor3ub(4, 70, 176);
    glBegin(GL_LINES);
    int p1; //point 1
    int p2;// point 2;
    //index 0 is reserves
    int x, x1, y, y1;
    for (int i = 1; i < global.nPoints; i++) {
        //do stuff here
        p1 = global.CurrentPopulation[global.min_el][i];
        p2 = global.CurrentPopulation[global.min_el][i - 1];
        x = global.point_coord[p1][0];
        x1 = global.point_coord[p2][0];
        y = global.y - global.point_coord[p1][1];
        y1 = global.y - global.point_coord[p2][1];
        glVertex2i(x, y);
        glVertex2i(x1, y1);

    }
    //print last vtex;

    p2 = global.CurrentPopulation[global.min_el][0];
    x1 = global.point_coord[p2][0];
    y1 = global.y - global.point_coord[p2][1];
    glVertex2i(x, y);
    glVertex2i(x1, y1);


    glEnd();
    //dra points
    glColor3ub(238, 29, 0);//red
    glBegin(GL_POINTS);


    for (int i = 0; i < global.nPoints; i++) {
        x = global.point_coord[i][0];
        y = global.y - global.point_coord[i][1];
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                glVertex2i(x+i, y+j);
            }
        }


    }
    glEnd();


    //draw roads
    glFlush();
}
// Keyboard functions
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 0x1B:
    case'q':
    case 'Q':
        exit(0);

        break;
    case 't':
    case 'T':
        Genetic_Algorith();
        printf("algorithm ended \n");
        finalImage();
        break;

    }

}




void show_keys()
{
    printf("Key functions\nQ:Quit\nT:Begin Algorithm\n");
}


int main(int argc, char **argv)
{



    //Initialize points
    global.count = 0;

    int choice;


    printf("How many Cities would you like?\n ");
    printf("max cities is 150?\n ");
    scanf_s("%d", &global.nPoints);
    //crash proof
    if (global.nPoints % 2 == 1) {
        global.nPoints = global.nPoints + 1;
    }

    global.popSize = 251;

    global.maxGen = 1500;

    global.x = 600; // window size
    global.y = 600;//window size
    initialize_coordinate_system(); // allocates memory based on number of points
    initialize_points();//creates n unique points random
    initialize_weight_matrix();
    initiateMatrixArray();

    global.mutationrate = 0.15;
    global.crossRate = 0.9;

    glutInit(&argc, argv);
    glutInitWindowSize(global.x, global.y); // adding 20 px to fit pixels in screen

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Triangulate");

    glutDisplayFunc(Display_coordinates);// will make screen black

    glClearColor(0.0, 0.0, 0.0, 1.0); // black background
    glColor3f(1.0, 0.0, 0.0);
    glutKeyboardFunc(keyboard);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0.0, global.x, 0.0, global.y, 0.0, 1.0);
    show_keys();
    glutMainLoop();

    return 0;
}