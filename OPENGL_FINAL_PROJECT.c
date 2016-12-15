#include<Windows.h>
#include<gl/GLUT.H>
#include<stdio.h>

#define max(a,b) ( ( (a)>(b) ) ? (a) : (b) )

//다중mesh를 위한 mesh클래스 선언
class Mesh
{
public:
   Mesh();

   float* vX;   float* vY;   float* vZ;
   int* vR;    int* vG;     int* vB;      int* vA;
   int* a;      int* b;      int* c;      int* d;      int *s;

   int numT;
   int numV;

   float trans_x = 0, trans_y = 0, trans_z = 0;      //translation
   float scale = 0.008;                          //scaling
   float rot_x = 0, rot_y = 0, rot_z = 0;         // rotation

   void Mesh::load(char *a);
   void Mesh::draw();
   void Mesh::translation();

};

Mesh::Mesh(){ };

void Mesh::load(char *filename)
{
   FILE *fp = fopen(filename, "r");

   char str[255];

   fscanf(fp, "%s", str);
   fscanf(fp, "%d %d %d", &numV, &numT, &str[10]);

   vX = new float[numV]; vY = new float[numV]; vZ = new float[numV];
   vR = new int[numV];   vG = new int[numV];   vB = new int[numV];   vA = new int[numV];
   s = new int[numT];
   a = new int[numT];   b = new int[numT];   c = new int[numT];   d = new int[numT];

   for (int i = 0; i < numV; i++){

      fscanf(fp, "%f %f %f %d %d %d %d", &vX[i], &vY[i], &vZ[i], &vR[i], &vG[i], &vB[i], &vA[i]);
   }

   for (int i = 0; i < numT; i++){

      fscanf(fp, "%d", &s[i]);
      //mesh파일 중 삼각형 mesh와 사각형 mesh가 섞여 있어서 나눠서 저장 > 삼각형이면 d[i]=0
      if (s[i] == 4)
         fscanf(fp, "%d %d %d %d", &a[i], &b[i], &c[i], &d[i]);
      else {
         fscanf(fp, "%d %d %d ", &a[i], &b[i], &c[i]);
         d[i] = 0;
      }
   }

}
void Mesh::translation(){

   glRotatef(rot_z, 0.0, 0.0, 1.0);
   glRotatef(rot_y, 0.0, 1.0, 0.0);
   glRotatef(rot_x, 1.0, 0.0, 0.0);

   glScalef(scale, scale, scale);
   glTranslatef(trans_x, -30 + trans_y, trans_z);

}
void Mesh::draw(){

   int Tindex = 0;

   //삼각형mesh를 먼저, 그 후에 사각형 mesh를 그림
   if (s[Tindex] == 3){
      glBegin(GL_TRIANGLES);

      while (s[Tindex] == 3){
         glVertex3f(vX[a[Tindex]], vY[a[Tindex]], vZ[a[Tindex]]); glColor3ub(vR[a[Tindex]], vG[a[Tindex]], vB[a[Tindex]]);
         glVertex3f(vX[b[Tindex]], vY[b[Tindex]], vZ[b[Tindex]]); glColor3ub(vR[b[Tindex]], vG[b[Tindex]], vB[b[Tindex]]);
         glVertex3f(vX[c[Tindex]], vY[c[Tindex]], vZ[c[Tindex]]); glColor3ub(vR[c[Tindex]], vG[c[Tindex]], vB[c[Tindex]]);
         Tindex++;
      }
      glEnd();
   }

   glBegin(GL_QUADS);
   while (Tindex < numT){
      glVertex3f(vX[a[Tindex]], vY[a[Tindex]], vZ[a[Tindex]]); glColor3ub(vR[a[Tindex]], vG[a[Tindex]], vB[a[Tindex]]);
      glVertex3f(vX[b[Tindex]], vY[b[Tindex]], vZ[b[Tindex]]); glColor3ub(vR[b[Tindex]], vG[b[Tindex]], vB[b[Tindex]]);
      glVertex3f(vX[c[Tindex]], vY[c[Tindex]], vZ[c[Tindex]]); glColor3ub(vR[c[Tindex]], vG[c[Tindex]], vB[c[Tindex]]);
      glVertex3f(vX[d[Tindex]], vY[d[Tindex]], vZ[d[Tindex]]); glColor3ub(vR[d[Tindex]], vG[d[Tindex]], vB[d[Tindex]]);
      Tindex++;
   }
   glEnd();
}


//mesh 생성 
Mesh castle;         //가장 중심의 성
Mesh castle_bar;      //회전 바      -> 이벤트 받으면 y축 90도 회전 anni
Mesh castle_bottom;      //midbox_under 성 아래의 바닥
Mesh midbox;         //성을 감싸는 midbox -> 위로 올라가는 anni
Mesh big_upper;         //큰 box  윗뚜껑 -> 위로 올라가는 anni
Mesh big_left;         //큰 box  왼쪽벽 -> z축으로 -90도 회전 anni
Mesh big_right;         //큰 box  오른쪽벽 -> z축으로 90도 회전 anni
Mesh big_front;         //큰 box  앞벽 -> x축으로 90도 회전 anni
Mesh big_back;         //큰 box  뒷벽 -> x축으로 -90도 회전 anni


//전역변수선언
GLubyte *pBytes;
BITMAPINFO *info;
GLuint texture[2];
float rot_L_x = -1.0, rot_L_y = 1.0, rot_L_z = 1.0;

///////////////////////////////////////////////////////////////////////////////////////
///////////////알맞은 주소를 입력하세요~~~변수 이름에 해당하는 파일 ㄱㄱㄱㄱㄱㄱㄱ//////////
///////////////////////////////////////////////////////////////////////////////////////
char *address_texture = "C:\\Users\\DS\\Desktop\\castle\\texture1024.bmp";
char *address_castle_realFin = "C:\\Users\\DS\\Desktop\\castle\\castle_realFin.off";
char *address_element_violet = "C:\\Users\\DS\\Desktop\\castle\\element_violet.off";
char *address_rotation = "C:\\Users\\DS\\Desktop\\castle\\rotation.off";
char *address_mid_box = "C:\\Users\\DS\\Desktop\\castle\\mid_box.off";
char *address_big_upper = "C:\\Users\\DS\\Desktop\\castle\\big_upper.off";
char *address_big_left4 = "C:\\Users\\DS\\Desktop\\castle\\big_left4.off";
char *address_big_right2 = "C:\\Users\\DS\\Desktop\\castle\\big_rigth2.off";
char *address_big_right1 = "C:\\Users\\DS\\Desktop\\castle\\big_front1.off";
char *address_big_right3 = "C:\\Users\\DS\\Desktop\\castle\\big_back3.off";

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

// 함수선언 

GLvoid Time(int sec)
{
   if (big_left.rot_z < 90)
   {
      big_upper.trans_y += 0.5;
      big_left.rot_z += 0.5;
      big_right.rot_z -= 0.5;
      big_front.rot_x += 0.5;
      big_back.rot_x -= 0.5;
      glutPostRedisplay();
      glutTimerFunc(8, Time, 1);
   }

   else if (castle.rot_y > -90)
   {
      big_upper.trans_y += 0.5;
      midbox.trans_y += 0.5;
      castle.rot_y -= 0.25;
      castle_bar.rot_y -= 0.25;
      glutPostRedisplay();
      glutTimerFunc(8, Time, 1);
   }

   //1.0 & 20 
   //0.5 & 10

}
GLvoid CastleBarTime(int sec)
{
   castle_bar.rot_y += 0.5;
   glutPostRedisplay();

   if ((int)castle_bar.rot_y % 90 != 0){
      glutTimerFunc(5, CastleBarTime, 1);
   }

}
GLvoid CameraRotCounterClockTime(int sec)
{
   //lookat 카메라 조정      default >  rot_L_x = -1.0, rot_L_y = 1.0, rot_L_z = 1.0;
   // 0.01   0.01   0.025
   
   if (rot_L_x < -1.0 + 0.07)
   {
      rot_L_x += 0.005;   rot_L_y += 0.005;   rot_L_z += 0.0125 ;
      glutPostRedisplay();
      glutTimerFunc(25, CameraRotCounterClockTime, 1);

   }

}
GLvoid CameraRotClockTime(int sec)
{
   if (rot_L_x > -1.0 - 0.07){
      rot_L_x -= 0.005;   rot_L_y -= 0.005;   rot_L_z -= 0.0125;
      glutPostRedisplay();
      glutTimerFunc(25, CameraRotClockTime, 1);

   }
}
GLvoid CameraRotCentTime(int sec)
{
   if (rot_L_x >= -1.0){
      if (rot_L_x = -1.0)   return;

      rot_L_x -= 0.005;   rot_L_y -= 0.005;   rot_L_z -= 0.0125;
      glutPostRedisplay();
      glutTimerFunc(25, CameraRotCentTime, 1);

   }
   else {   
      rot_L_x += 0.005;   rot_L_y += 0.005;   rot_L_z += 0.0125;
      glutPostRedisplay();
      glutTimerFunc(25, CameraRotCentTime, 1);

   }
}

GLubyte * LoadDIBMP(const char *filename, BITMAPINFO **info)
{
   FILE *fp;
   GLubyte *bits;
   int bitsize, infosize;
   BITMAPFILEHEADER header;

   if ((fp = fopen(filename, "rb")) == NULL)      return NULL;

   if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1)
   {
      fclose(fp);
      return NULL;
   }

   if (header.bfType != 'MB')
   {
      fclose(fp);
      return NULL;
   }

   infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);

   if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL)
   {
      fclose(fp);      exit(0);
      return NULL;
   }

   if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {

      free(*info);
      fclose(fp);

      return NULL;
   }

   if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
      bitsize = ((*info)->bmiHeader.biWidth *
      (*info)->bmiHeader.biBitCount + 7) / 8.0 *
      abs((*info)->bmiHeader.biHeight);

   if ((bits = (unsigned char *)malloc(bitsize)) == NULL)
   {
      free(*info);   fclose(fp);
      return NULL;
   }

   if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize)
   {
      free(*info); free(bits);
      fclose(fp);
      return NULL;
   }

   fclose(fp);

   return bits;

}

void initTextures(){

   glGenTextures(1, texture);
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   pBytes = LoadDIBMP(address_texture, &info);
   glTexImage2D(GL_TEXTURE_2D, 0, 3, 1024, 1024, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   glColor3f(1.0f, 1.0f, 1.0f); // 이것을 선언을 안해주면 object와 색깔이 겹쳐짐


}

void display(){

   glMatrixMode(GL_MODELVIEW);
   glEnable(GL_TEXTURE_2D);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_SMOOTH);

   initTextures();
   
   glPushMatrix();
   glBindTexture(GL_TEXTURE_2D, texture[0]);
   glBegin(GL_QUADS);
   {

      glTexCoord2f(0, 0);      glVertex3f(1.0f, -3.9f, -3.0f);
      glTexCoord2f(1, 0);      glVertex3f(3.0f, -3.9f, -1.0f);
      glTexCoord2f(1, 1);      glVertex3f(3.0f, -0.1f, -1.0f);
      glTexCoord2f(0, 1);      glVertex3f(1.0f, -0.1f, -3.0f);

   }
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glPopMatrix();

   glLoadIdentity();


   gluLookAt(rot_L_x, rot_L_y, rot_L_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);


   glPushMatrix();
   castle_bar.translation();      castle_bar.draw();
   glPopMatrix();

   glPushMatrix();
   castle_bottom.translation();   castle_bottom.draw();
   glPopMatrix();

   glPushMatrix();
   castle.translation();         castle.draw();
   glPopMatrix();

   glPushMatrix();
   midbox.translation();         midbox.draw();
   glPopMatrix();

   glPushMatrix();
   big_right.translation();      big_right.draw();
   glPopMatrix();

   glPushMatrix();
   big_front.translation();      big_front.draw();
   glPopMatrix();

   glPushMatrix();
   big_back.translation();         big_back.draw();
   glPopMatrix();

   glPushMatrix();
   big_upper.translation();      big_upper.draw();
   glPopMatrix();

   glPushMatrix();
   big_left.translation();         big_left.draw();
   glPopMatrix();

   glutSwapBuffers();

}

void reshape(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.0, 1.0, -1.0, 1.0, -50.0, 200.0);
   glutPostRedisplay();

}

void MouseButton(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON)
      glutTimerFunc(5, CastleBarTime, 1);
}

void myKeyboard(unsigned char key, int x, int y){
   switch (key)
   {
      //rotate_camera
   case 'a':   glutTimerFunc(5, CameraRotCounterClockTime, 1);      break;
   case 'd':   glutTimerFunc(5, CameraRotClockTime, 1);         break;
   case 's':   glutTimerFunc(5, CameraRotCentTime, 1);            break;

   default:
      break;
   }
   glutPostRedisplay();
}


void main(int argc, char ** argv)
{

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(1000, 1000);
   glutCreateWindow("example");
   glClearColor(1.0, 1.0, 1.0, 1.0);

   glutReshapeFunc(reshape);
   glutMouseFunc(MouseButton);
   glutKeyboardFunc(myKeyboard);
   glutDisplayFunc(display);
   glutTimerFunc(2000, Time, 1);

   castle.load(address_castle_realFin);
   castle_bottom.load(address_element_violet);
   castle_bar.load(address_rotation);
   midbox.load(address_mid_box);
   big_upper.load(address_big_upper);
   big_left.load(address_big_left4);
   big_right.load(address_big_right2);
   big_front.load(address_big_right1);
   big_back.load(address_big_right3);

   glutMainLoop();

}

