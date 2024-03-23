//#include <Windows.h>    
//#include <gl/GL.h>    
//#include <gl/GLU.h>
//
//GLfloat diffuseMaterial[4] = { 0.5, 0.5, 0.5, 1.0 };
//
//void init(void)
//{
//    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
//    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
//
//    glClearColor(0.0, 0.0, 0.0, 0.0);
//    glShadeModel(GL_SMOOTH);
//    glEnable(GL_DEPTH_TEST);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//    glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
//    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//
//    glColorMaterial(GL_FRONT, GL_DIFFUSE);
//    glEnable(GL_COLOR_MATERIAL);
//}
//
//void display(void)
//{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glutSolidSphere(1.0, 20, 16);
//    glFlush();
//}
//
////void reshape(int w, int h)
////{
////    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
////    glMatrixMode(GL_PROJECTION);
////    glLoadIdentity();
////    if (w <= h)
////        glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h / (GLfloat)w,
////            1.5 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
////    else
////        glOrtho(-1.5 * (GLfloat)w / (GLfloat)h,
////            1.5 * (GLfloat)w / (GLfloat)h, -1.5, 1.5, -10.0, 10.0);
////    glMatrixMode(GL_MODELVIEW);
////    glLoadIdentity();
////}