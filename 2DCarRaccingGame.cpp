#include<GL/glut.h>
#include<iostream>//for strlen
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

 
int i,q;
int score = 0;//for score counting
int screen = 0;
bool collide = false;//check if car collide to make game over
char buffer[10];
bool menuValue=true,gameQuit=false,instructionsGame=false,mButtonPressed=false,gameover=false,startgame=false;
int vehicleX = 200, vehicleY = 70;   
int ovehicleX[4], ovehicleY[4];
int divx = 250, divy = 4, movd;
float mouseX ,mouseY ;
GLint m_viewport[4];
char uName[40],username[40];
char highScore[100],ch,ch1;	
//char arrow[10]="--->";
//char copyUsername[10];


void displayRasterText(float x ,float y ,float z ,char *stringToDisplay) {
	int length;
	glRasterPos3f(x, y, z);
		length = strlen(stringToDisplay);

	for(int i=0 ;i<length ;i++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 ,stringToDisplay[i]); 
	}
}
 
void drawText(char ch[],int xpos, int ypos)//draw the text for score and game over
{
    int numofchar = strlen(ch);
    glLoadIdentity ();
    glRasterPos2f( xpos , ypos);
    for (i = 0; i <= numofchar - 1; i++)
    {
     
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ch[i]);//font used here, may use other font also
    }
}
 
 
void drawTextNum(char ch[],int numtext,int xpos, int ypos)//counting the score
{
    int len;
    int k;
    k = 0;
    len = numtext - sizeof (ch);
    glLoadIdentity ();
    glRasterPos2f( xpos , ypos);
    for (i = 0; i <=numtext - 1; i++)
    {
    if ( i < len )
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'0');
    else
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ch[k]);
        k++;
    }
     
    }
}
 
void passiveMotionFunc(int x,int y) {

	//when mouse not clicked
	mouseX = float(x)/(m_viewport[2]/1200.0)-600.0;  //converting screen resolution to ortho 2d spec
	mouseY = -(float(y)/(m_viewport[3]/700.0)-350.0);

}

void readFromFile() {

	FILE *fp = fopen("HighScoreFile.txt" ,"r");
	int i=0;
	if(fp!= NULL){
		while(fread(&ch,sizeof(char),1 ,fp)){
			highScore[i++] = ch;
		}
		highScore[i] = '\0';
	} 
	fclose(fp);
}
void readFromUserFile(){
	FILE *fp = fopen("UserName.txt","r");
	int i=0;
	if(fp!= NULL){
		while(fread(&ch1,sizeof(char),1,fp)){
			username[i++] = ch1;
		}
		username[i] = '\0';
	}
	fclose(fp);
}

void writeIntoFile() {						//To write high score on to file
	FILE *fp = fopen("HighScoreFile.txt" ,"w");
	int i=0;
	char temp[40];
	if(fp!= NULL){
		int n= score;
		while(n){
			ch = (n%10)+ '0';
			n/=10;
			temp[i++] = ch;
		}
		temp[i] = '\0';
		strrev(temp);		
		//puts(temp);
		if(temp[0] == '\0')
			temp[i++] = '0' ,temp[i++] = '\0';
		//strcat(uName,sign);
		//strcat(uName,temp);
		//fprintf(fptr,"%s", sentence);
		fwrite(temp ,sizeof(char)*i ,i ,fp);
	}
 fclose(fp);
}

void writeIntoUnameFile() {						//To write high score on to file
	FILE * fp;
   int i;
   /* open the file for writing*/
   fp = fopen ("UserName.txt","w");
 
   /* write 10 lines of text into the file stream*/
       fwrite(uName, sizeof(uName),1,fp);
 
   /* close the file*/  
   fclose (fp);

}


void ovpos()
{
    glClearColor(0,0,1,0);
for(i = 0; i < 4; i++)
{
    if(rand() % 2 == 0)
    {
        ovehicleX[i] = 200;
    }
    else
    {
        ovehicleX[i] = 300;
    }  
    ovehicleY[i] = 1000 - i * 160;
}
}
 
 
void drawRoad()
{
    glBegin(GL_QUADS); 
        glColor3f(0.5,0.5,0.5);
        glVertex2f(250 - 100, 500);
        glVertex2f(250 - 100, 0);
        glVertex2f(250 + 100, 0);
        glVertex2f(250 + 100, 500);
    glEnd();
}
 
 
void drawDivider()//black patch drawn in middle of road
{
    glLoadIdentity();
    glTranslatef(0, movd, 0);
    for(i = 1; i <= 10; i++)
    {
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
        glVertex2f(divx  - 5, divy * 15 * i + 18);
        glVertex2f(divx  - 5, divy * 15 * i - 18);
        glVertex2f(divx  + 5, divy * 15 * i - 18);
        glVertex2f(divx  + 5, divy * 15 * i + 18);
    glEnd();
    }
    glLoadIdentity();
}
     
 
void drawVehicle()//car for racing
{
    glPointSize(10.0);
    glBegin(GL_POINTS);//tire
        glColor3f(0,0,0);                     
        glVertex2f(vehicleX - 25, vehicleY + 16); 
        glVertex2f(vehicleX + 25, vehicleY + 16); 
        glVertex2f(vehicleX - 25, vehicleY - 16); 
        glVertex2f(vehicleX + 25, vehicleY - 16);
    glEnd();
     
    glBegin(GL_QUADS);  
        glColor3f(1,0,0);//middle body
        glVertex2f(vehicleX - 25, vehicleY + 20);
        glVertex2f(vehicleX - 25, vehicleY - 20);
        glVertex2f(vehicleX + 25, vehicleY - 20);
        glVertex2f(vehicleX + 25, vehicleY + 20);
    glEnd();
 
    glBegin(GL_QUADS);//up body
        glColor3f(0,0,1);
        glVertex2f(vehicleX - 23, vehicleY + 20);
        glVertex2f(vehicleX - 19, vehicleY + 40);
        glVertex2f(vehicleX + 19, vehicleY + 40);
        glVertex2f(vehicleX + 23, vehicleY + 20);
    glEnd();
 
    glBegin(GL_QUADS);//down body
        glColor3f(0,0,1);
        glVertex2f(vehicleX - 23, vehicleY - 20);
        glVertex2f(vehicleX - 19, vehicleY - 35);
        glVertex2f(vehicleX + 19, vehicleY - 35);
        glVertex2f(vehicleX + 23, vehicleY - 20);
    glEnd();
 } 
  
void drawOVehicle()//other cars
{
     
    for(i = 0; i < 4; i++)
    {
    glPointSize(10.0);
    glBegin(GL_POINTS);//tire
        glColor3f(0,0,0);                     
        glVertex2f(ovehicleX[i] - 25, ovehicleY[i] + 16); 
        glVertex2f(ovehicleX[i] + 25, ovehicleY[i] + 16); 
        glVertex2f(ovehicleX[i] - 25, ovehicleY[i] - 16); 
        glVertex2f(ovehicleX[i] + 25, ovehicleY[i] - 16);
    glEnd();
     
    glBegin(GL_QUADS);  
        glColor3f(0.99609, 0.83984, 0);//middle body
        glVertex2f(ovehicleX[i] - 25, ovehicleY[i] + 20);
        glVertex2f(ovehicleX[i] - 25, ovehicleY[i] - 20);
        glVertex2f(ovehicleX[i] + 25, ovehicleY[i] - 20);
        glVertex2f(ovehicleX[i] + 25, ovehicleY[i] + 20);
    glEnd();
 
    glBegin(GL_QUADS);//up body
        glColor3f(0,1,0);
        glVertex2f(ovehicleX[i] - 23, ovehicleY[i] + 20);
        glVertex2f(ovehicleX[i] - 19, ovehicleY[i] + 40);
        glVertex2f(ovehicleX[i] + 19, ovehicleY[i] + 40);
        glVertex2f(ovehicleX[i] + 23, ovehicleY[i] + 20);
    glEnd();
 
    glBegin(GL_QUADS);//down body
        glColor3f(0,1,0);
        glVertex2f(ovehicleX[i] - 23, ovehicleY[i] - 20);
        glVertex2f(ovehicleX[i] - 19, ovehicleY[i] - 35);
        glVertex2f(ovehicleX[i] + 19, ovehicleY[i] - 35);
        glVertex2f(ovehicleX[i] + 23, ovehicleY[i] - 20);
    glEnd();
     
    ovehicleY[i] = ovehicleY[i] - 8; 
     
    if(ovehicleY[i] > vehicleY - 25 - 25 && ovehicleY[i] < vehicleY + 25 + 25 && ovehicleX[i] == vehicleX)
    {
    	//printf("%d\t%d\t\t%d\t%d\t\t%d\t%d\n",ovehicleY[i],vehicleY - 25 - 25,ovehicleY[i],vehicleY + 25 + 25,ovehicleX[i],vehicleX);
        collide = true;
    
		startgame=false;
	}
	if(ovehicleY[i] < -25)
    { 
        if(rand() % 2 == 0)
        {
            ovehicleX[i] = 200;
        }
        else
        {
            ovehicleX[i] = 300;
        }
        ovehicleY[i] = 600; 
    } 
    
    
	}
}


void Specialkey(int key, int x, int y)//allow to use navigation key for movement of car
{
switch(key)
{
case GLUT_KEY_UP: 
	for(i = 0; i <4; i++)
             {
              ovehicleY[i] = ovehicleY[i] - 10;  
             }
             movd = movd - 20;
             break;	
case GLUT_KEY_DOWN: 
	for(i = 0; i <4; i++)
             {
              ovehicleY[i] = ovehicleY[i] + 10;  
             }
             movd = movd + 20;
             break;
case GLUT_KEY_LEFT:vehicleX = 200;
             break;
case GLUT_KEY_RIGHT:vehicleX = 300;
             break;
			 
}

glutPostRedisplay();
}

void Normalkey(unsigned char key, int x, int y)
{
    switch(key)
    {
    /*case '1':if(screen == 0)
             screen=1;
             break;
    case '2':if(screen == 1)
             screen=2;
             break;*/
    case 27:exit(0);
    		break;
    case 'q': //glClearColor(0,0,0,1);
			  menuValue=true;
    		  gameover=false;
    		  startgame=false;
    		  instructionsGame=false;
			break;
    }
} 
 
void init()
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(0, 500, 0, 500);
glMatrixMode(GL_MODELVIEW);
}
void InstructionsScreenDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,1);
	//glEnable(GL_DEPTH_TEST);
	//glLoadIdentity();
	//SetDisplayMode(MENU_SCREEN);
	//colorBackground();
	glColor3f(1, 1, 1);
	displayRasterText(100 ,400 ,0.4 ,"> Press Left arrow key to move left.");
	displayRasterText(100 ,350 ,0.4 ,"> Press Right arrow key to move right.");
	displayRasterText(100 ,300 ,0.4 ,"> Press Up arrow key to increase the speed.");
	displayRasterText(100 ,250 ,0.4 ,"> Press Down arrow key to decrease the speed.");
	
	//displayRasterText(-900 ,-100 ,0.4 ,"The packet can be placed only when 's' is pressed before.");
	displayRasterText(100 ,200 ,0.4 ,"> You Get 1 point for the unit distance travelled.");
	displayRasterText(100, 150,0.4,"> The Objective is to score maximum points");
	displayRasterText(100, 100,0.4,"> Press 'Q' to go back to main menu.");	
	displayRasterText(100, 50,0.4,"> Press escape key to go quit.");
	//backButton();
	//if(previousScreen)
		//nextScreen = false ,previousScreen = false; //as set by backButton()


}
 void menuScreen()
 {
 		glLineWidth(1);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0 , 0, 1);
	glColor3f(0,1,0);
	glBegin(GL_POLYGON);				//START GAME PLOYGON
		glVertex3f(250 - 100, 300,0.5);
        glVertex3f(250 - 100, 400,0.5);
        glVertex3f(250 + 100, 400,0.5);
        glVertex3f(250 + 100, 300,0.5);
	glEnd();
	
	glColor3f(0,0,1);
	glBegin(GL_POLYGON);				//INSTRUCTIONS POLYGON
		glVertex3f(250 - 100, 175,0.5);
        glVertex3f(250 - 100, 275,0.5);
        glVertex3f(250 + 100, 275,0.5);
        glVertex3f(250 + 100, 175,0.5);
	glEnd();
	
	glColor3f(1,0,0);
	glBegin(GL_POLYGON);				//QUIT POLYGON
		glVertex3f(250 - 100, 50,0.5);
        glVertex3f(250 - 100, 150,0.5);
        glVertex3f(250 + 100, 150,0.5);
        glVertex3f(250 + 100, 50,0.5);
	glEnd();

	if(mouseX>=-250 && mouseX<=250 && mouseY>=90 && mouseY<=200){
		glColor3f(1 ,1 ,1) ;
		if(mButtonPressed){
			//startGame();
			startgame = true ;
			gameover = false;
			mButtonPressed = false;
			menuValue=false;
		}
	} else
		glColor3f(1 , 0, 0);

	displayRasterText(220 ,340 ,0.4 ,"Start Game");
	
	if(mouseX>=-250 && mouseX<=250 && mouseY>=-100 && mouseY<=40) {
		glColor3f(1 ,1 ,1);
		if(mButtonPressed){
			startgame = false;
			menuValue=false;
			instructionsGame = true ;
			mButtonPressed = false;
		}
	} else
		glColor3f(0 , 0, 0);
	displayRasterText(220 ,210 ,0.4 ,"Instructions");
	
	if(mouseX>=-250 && mouseX<=250  &&  mouseY>=-270 && mouseY<=-140){
		glColor3f(1 ,1 ,1);
		if(mButtonPressed){
			//printf("%f",mouseY);
			//InstructionsScreenDisplay();
			gameQuit = true ;
			menuValue=false;
			mButtonPressed = false;
			startgame = false;
		}
	}
	else
		glColor3f(0 , 0, 0);
	displayRasterText(220 ,90 ,0.4 ,"    Quit");
 }
 
 void GameOver()	//Display Game Over Screen
 {
 	startgame=false;
 	menuValue=false;
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 	glClearColor(1, 0 , 0, 1);
 	char temp[40];
	char c[1000];
    FILE *fptr = fopen("HighScoreFile.txt", "r");

    if (fptr == NULL)
    {
        printf("Error! opening file1");
        // Program exits if file pointer returns NULL.
        exit(1);         
    }

    // reads text until newline 
    fscanf(fptr,"%s", c);
	fclose(fptr);
	sprintf(temp,"Score : %d",score);
	displayRasterText(-100 ,340 ,0.4 ,temp);
	readFromFile();            //read high score from the file*
	readFromUserFile();
	char temp2[40];
	if(atoi(highScore) < score){         //set new high score*
		writeIntoFile();
		writeIntoUnameFile();           //write into user name file
		sprintf(temp2 ,"Highest Score :%d" ,score);
		printf("%d",score);
	} else 
		sprintf(temp2 ,"Highest Score :%s ---> %s",username,highScore);

	displayRasterText(-250 ,400 ,0.4 ,temp2);

     
    
 	//glEnable(GL_DEPTH_TEST);
	//glLoadIdentity();
	glColor3f(255,0,0);
	drawText("High Score : ",200,200);					//Display highscore after game over
	drawText(username,270,200);
	drawText(" ---> ",(275+strlen(username)+10),200);
	drawText(highScore,320,200);
	//itoa (highScore, buffer, 10);
    //drawTextNum(buffer, 6, 420,455);
	drawText("Game Over", 200,150);
	glFlush();
	glutSwapBuffers();
 }
 
 void playGame()		//Start the game
 {
 	glClear(GL_COLOR_BUFFER_BIT);
    drawRoad(); 
    drawDivider();
    drawVehicle();
    drawOVehicle();
    movd = movd - 16;
    if(movd < - 60)
    {
        movd = 0;
    }
     
    score = score + 1;
    glColor3f(1,1,1);
    drawText("Score:", 360,455);
    itoa (score, buffer, 10);
    drawTextNum(buffer, 6, 420,455);
    glutSwapBuffers(); 
    for(q = 0; q<= 10000000; q++){;}
    if(collide == true)
    {
    	//printf("Game Over");
    	//gameover= true;
        //menuValue=false;
		//startgame=false;
		//glColor3f(0,0,0);
    	//drawText("Game Over", 200,250);
		GameOver();
		//glutSwapBuffers();
        //gets();
        //exit(0);
    }
 }
 
void display()
{
    /*if(screen == 0)
    {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,1,1);
    drawText("WELCOME", 170, 290);
    drawText("PLAYER", 170, 250);
    drawText("Press 1 to continue", 170, 150);
    glutSwapBuffers();
    }
    else if(s == 1)
    {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,1,1);
    drawText("Use w to accelerate", 170, 290);
    drawText("Use a to move left", 170, 250);
    drawText("Use d to move right", 170, 210);
    drawText("Press 2 to continue", 170, 150);
    glutSwapBuffers();
    }
    else
    {*/
    //glLineWidth(50);
	//SetDisplayMode(MENU_SCREEN);

	/*glColor3f(0.3764,0.3764,0.3764);
	glBegin(GL_LINE_LOOP);               //Border
		glVertex3f(-750 ,-500 ,0.5);
		glVertex3f(-750 ,550 ,0.5);
		glVertex3f(750 ,550 ,0.5);
		glVertex3f(750 ,-500, 0.5);
	glEnd();*/	
	if(menuValue)
	{	
		startgame = false;
		menuScreen();
	}
	else if(startgame)
	{
		menuValue=false;
		playGame();
	}
	else if(instructionsGame)
	{
		startgame = false;
		menuValue=false;
		InstructionsScreenDisplay();
	}
	else if(gameQuit)
	{
		exit(0);
	}
	else if(gameover)
	{
		GameOver();
	}
    glFlush();  
	glLoadIdentity();
	glutSwapBuffers();
    //}
}
 
void mouseClick(int buttonPressed ,int state ,int x, int y) {
	
	if(buttonPressed == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		mButtonPressed = true;
	else 
		mButtonPressed = false;
	//display();
}
 
int main(int argc, char **argv)
{
 	printf("Enter the name:");
 	gets(uName);
	
	 FILE *fp1 = fopen("HighScoreFile.txt" ,"r") ;      //check if HighScoreFile.txt exist if not create             
	 if(fp1!=NULL)
		fclose(fp1);
	 else
		 writeIntoFile(); 
		 
	 FILE *fp2 = fopen("UserName.txt" ,"r") ;      //check if UserName.txt exist if not create             
	 if(fp2!=NULL)
		fclose(fp2);
	 else
	 	printf("Hello");
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,500);
    glutCreateWindow("2D Car Racing game");
    ovpos();
    init();
    glutDisplayFunc(display);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutMouseFunc(mouseClick);
    glGetIntegerv(GL_VIEWPORT ,m_viewport);
	glutSpecialFunc(Specialkey);
    glutKeyboardFunc(Normalkey);
    glutIdleFunc(display);
    glutMainLoop();
    return 0;
}
