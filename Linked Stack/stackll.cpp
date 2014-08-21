#include<iostream>
#include<GL/glut.h>
using namespace std;


static GLfloat nodeHt = 30;

static GLfloat arrow_width=2;
static GLfloat arrow_height=.5*nodeHt;

static GLfloat node_color[3]={1,.6,0};
static GLfloat arrow_color[3]={0,0,0};
static GLfloat clear_color[4]={1,.7,0,0};
static GLfloat base_color[3]={0,0,0};
static GLfloat text_color[3]={0,0,0};

static GLfloat xleft=0;
static GLfloat xright=400;
static GLfloat ybottom=0;
static GLfloat ytop=400;

static GLfloat topX_base = xleft+nodeHt*2;
static GLfloat topY_base = ybottom+nodeHt;

static GLfloat topX = topX_base;     
static GLfloat topY = topY_base;



enum Action_State
{
     PUSH,POP,OVERF,UNDERF,NONE
};

Action_State state=NONE;

static GLfloat msg_box_color[5][3]={{0.1,0.7,0.1},{.7,0.1,0.1},{1,1,0},{1,1,0},{1,1,1}};
static GLfloat msg_box_node_color[5][3]={{0.4,1.0,0.4},{1.0,0.4,0.4},{1,1,.4},{1,1,.4},{1,1,1}};


static GLfloat msg_box_width=(xright-xleft)/3;
static GLfloat msg_box_height=2*(nodeHt+arrow_height);

static GLfloat msgX=xright-(msg_box_width+nodeHt);
static GLfloat msgY=ytop-(msg_box_height+nodeHt*2);

static char msg_list[5][20]={"Element Pushed!","Element Popped!","Stack Overflow!","Stack Underflow!"," "};

static const int STACK_MAX=(int)((ytop-ybottom-nodeHt*3)/(nodeHt+arrow_height));


struct stack_element
{
       int data;
       struct stack_element* down;
	   struct stack_element* up;
};

typedef struct stack_element* StackElement;


StackElement msg_box_node=NULL;

class stack
{
      StackElement head,top;
      
      public:
             stack()
             {
                    top=head=newNode(0,NULL,NULL);
                 //head.data=0;
                 //head.down=NULL; 
             }
             stack(int a[], int n)
             {
                       top=head=newNode(0,NULL,NULL);
                       for(int i=0; i<n ; i++)
                       {
                               push(a[i]);
                       }
             }
             
             StackElement newNode(int data, StackElement down, StackElement up)
             {
                          StackElement new_element;
                          new_element= new struct stack_element;
                          new_element->data=data;
                          new_element->down=down;
						  new_element->up=up;
                          return new_element;
             }
             
             void push(int data)
             {
                  if((head->data)>=STACK_MAX)
                  {
                                             state=OVERF;
                                             return;
                  }
                  (head->data)++;
                  StackElement element=newNode(data,top,NULL);
                  top->up=element;
                  top=element;
                  msg_box_node=element;
                  state=PUSH;
                  //glutPostRedisplay();
             }
             
             void pop()
             {
                          if(head->data<=0)
                          {
                                           state=UNDERF;
                                           return;
                          }
                          head->data--;
                          StackElement temp=top;
                          top=top->down;
                          //glutPostRedisplay();
                          msg_box_node=temp;
                          state=POP;
                          //return temp;
             }
             
             void drawStackDown()
             {
                  StackElement curr=top;
                  topY=topY_base;
                  topY+=(head->data)*(nodeHt+(arrow_height+nodeHt/4));
                  
				  while(curr!=head)
                  {
                                  
                                  draw_node_down(curr);
								  
                                  curr=curr->down;
                  }
				                  draw_base();
             }
			 
			 
             friend void print_num(int num, GLfloat scale,GLfloat x, GLfloat y, GLfloat z);
             friend void draw_node_down(StackElement n);  
             friend void draw_node_down(StackElement n, GLfloat x, GLfloat y);
			 friend void draw_arrow_down();
			 friend void draw_base();
          
};

int stack_init[]={1,2,3,4,5};
stack s;


int itoa(int n, char s[])
{
    int i, sign,len;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
	len=strlen(s);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    strrev(s);
	return len;
}



void print_num(int num, GLfloat scale=1,GLfloat x=0, GLfloat y=0, GLfloat z=0)
{
              char s[20];
              itoa(num,s);
               glPushMatrix();
                              glColor3fv(text_color);
                              glTranslatef(x,y,z);
                              glScalef(scale,scale,1);
                              for( int i=0 ; s[i]!='\0' ; i++ )
                              {
                                   glutStrokeCharacter(/*GLUT_STROKE_ROMAN*/GLUT_STROKE_MONO_ROMAN, s[i]);
                              }
               glPopMatrix();
}

void print_line(char s[], GLfloat scale=1,GLfloat x=0, GLfloat y=0, GLfloat z=0)
{
     glPushMatrix();
                              glColor3fv(text_color);
                              glTranslatef(x,y,z);
                              glScalef(scale,scale,1);
                              for( int i=0 ; s[i]!='\0' ; i++ )
                              {
                                   glutStrokeCharacter(/*GLUT_STROKE_ROMAN*/GLUT_STROKE_MONO_ROMAN, s[i]);
                              }
     glPopMatrix();
}     


void draw_square_down(GLfloat x, GLfloat y, GLfloat l)
{
     
     glBegin(GL_QUADS);
                       glVertex2f(x,y);
                       glVertex2f(x+l,y);
                       glVertex2f(x+l,y-l);
                       glVertex2f(x,y-l);
     glEnd();
     
}

void draw_msg_box()
{
     char *print_string=msg_list[state];
     if(state==NONE) return;
          glColor3fv(msg_box_color[state]);
          glBegin(GL_QUADS);
                            glVertex2f(msgX,msgY);
                            glVertex2f(msgX+msg_box_width,msgY);
                            glVertex2f(msgX+msg_box_width,msgY+msg_box_height);
                            glVertex2f(msgX,msgY+msg_box_height);
          glEnd();
          
          GLfloat scale= (nodeHt/119.05)*.3;
          print_line(print_string,scale,msgX + nodeHt/8 , msgY + nodeHt/4 );
     
     if(state==PUSH || state==POP)
          draw_node_down(msg_box_node, msgX+nodeHt*1.75, msgY+2*nodeHt);
                      
          
}
void draw_node_down(StackElement n)
{
     //ht=119.05 wd= 104.76
      GLfloat scale= (nodeHt/119.05)*.5;
      //int num=0;
      int num=n->data;//s.getval(n);
	  //draw_arrow();
	  glColor3fv(node_color);
      draw_square_down(topX,topY, nodeHt);
      print_num(num,scale,topX+(nodeHt/4),(topY-(nodeHt-nodeHt/4)));  
                                             
      topY-=(nodeHt);
      draw_arrow_down();
      
}

void draw_node_down(StackElement n, GLfloat x, GLfloat y)
{
     //ht=119.05 wd= 104.76
      GLfloat scale= (nodeHt/119.05)*.5;
      //int num=0;
      int num=n->data;//s.getval(n);
	  //draw_arrow();
	  glColor3fv(msg_box_node_color[state]);
      draw_square_down(x,y, nodeHt);
      print_num(num,scale,x+(nodeHt/4),(y-(nodeHt-nodeHt/4)));  
                                             
            
}

void draw_arrow_down()
{
    glColor3fv(arrow_color);
	GLfloat half1=(nodeHt/2);
	GLfloat half2=half1/2;
	GLfloat half3=half2/2;
	topX+=half1;
	
	glBegin(GL_QUADS);
		glVertex2f(topX-half3,topY);
		glVertex2f(topX+half3,topY);
		glVertex2f(topX+half3,topY-(arrow_height));
		glVertex2f(topX-half3,topY-(arrow_height));
	glEnd();
	
	topY-=(arrow_height);
	
	glBegin(GL_TRIANGLES);
		glVertex2f(topX,topY-half2);
		glVertex2f(topX+half2,topY);
		glVertex2f(topX-half2,topY);
	glEnd();
	
	topY-=(half2);
	topX-=half1;
	
	
	
	
}		

void draw_base()
{
     glColor3fv(base_color);
	glBegin(GL_QUADS);
		glVertex2f(topX-nodeHt,topY);
		glVertex2f(topX+nodeHt*2,topY);
		glVertex2f(topX+nodeHt*2,topY-(.5*nodeHt));
		glVertex2f(topX-nodeHt,topY-(.5*nodeHt));
	glEnd();
}


void myinit()
{
     glClearColor(.7,.4,0,0);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluOrtho2D(xleft,xright,ybottom,ytop);
     glMatrixMode(GL_MODELVIEW);
}


void myReshape(int w, int h)
{
     GLfloat fAspect;

    
    if(h == 0) h = 1;

    glViewport(0, 0, w, h);

    fAspect = (GLfloat)w/(GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    
    gluPerspective(35.0f, fAspect, 1.0, 40.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}


     

void display()
{
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     
   
     s.drawStackDown();
     draw_msg_box();
    
     
     glFlush();
}

void menu(int item)
{
     StackElement a;
        switch (item)
        {
        case 0:
                      s.pop();
                      break;
        
        }

        glutPostRedisplay();
        return;
}

void push_menu(int item)
{
    
        switch (item)
        {
        case 0:
               s.push(0);
               break;
        case 1:
               s.push(1);
               break;
        case 2:
               s.push(2);
               break;
        case 3:
               s.push(3);
               break;
        case 4:
               s.push(4);
               break;
        case 5:
               s.push(5);
               break;
        case 6:
               s.push(6);
               break;
        case 7:
               s.push(7);
               break;
        case 8:
               s.push(8);
               break;
        case 9:
               s.push(9);
               break;
               
        }

        glutPostRedisplay();
        return;
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Stack");
    //glutReshapeFunc(myReshape);
    
    int submenu = glutCreateMenu(push_menu);
	glutAddMenuEntry("0",0);
	glutAddMenuEntry("1",1);
	glutAddMenuEntry("2",2);
	glutAddMenuEntry("3",3);
	glutAddMenuEntry("4",4);
	glutAddMenuEntry("5",5);
	glutAddMenuEntry("6",6);
	glutAddMenuEntry("7",7);
	glutAddMenuEntry("8",8);
	glutAddMenuEntry("9",9);
	
	glutCreateMenu(menu);
	glutAddSubMenu("Push",submenu);
    glutAddMenuEntry("Pop", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutDisplayFunc(display);
    myinit();
    glutMainLoop();
    
    return 0;
}
