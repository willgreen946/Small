#include <stdio.h>
#include <stdlib.h>

#include <GL/glx.h>
#include <GL/gl.h>

#include <X11/X.h>
#include <X11/keysym.h>

typedef struct {
	XEvent event; 
	XVisualInfo *visual_info;
	XSetWindowAttributes window_attributes; 
	Display *display;
	Window window;
	Colormap color_map; 
	GLXContext glx_context;
}SCR;

int dummy;
GLfloat xAngle = 42.0, yAngle = 42.0, zAngle = 42.0;
GLboolean needRedraw = GL_FALSE, recalcModelView = GL_TRUE;
GLboolean doubleBuffer = GL_TRUE;

static int single_gl_buf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
static int double_gl_buf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

SCR *scr;

void redraw (void)
{
  static GLboolean   displayListInited = GL_FALSE;

  if (displayListInited)
  {
    /* if display list already exists, just execute it */
    glCallList(1);
  }
  else
  {
    /* otherwise compile and execute to create the display list */
    glNewList(1, GL_COMPILE_AND_EXECUTE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* front face */
    glBegin(GL_QUADS);
      glColor3f(0.0, 0.7, 0.1);  /* green */
      glVertex3f(-1.0, 2.5, 1.0);
      glVertex3f(1.0, 2.5, 1.0);
      glVertex3f(1.0, -2.5, 1.0);
      glVertex3f(-1.0, -5.5, 1.0);

      /* back face */
      glColor3f(0.9, 4.5, 6.0);  /* yellow */
      glVertex3f(-2.0, 2.5, -1.0);
      glVertex3f(1.0, 2.5, -1.0);
      glVertex3f(1.0, -2.5, -1.0);
      glVertex3f(-6.0, -2.5, -1.0);

      /* top side face */
      glColor3f(0.2, 0.25, 1.0);  /* blue */
      glVertex3f(-2.0, 2.5, 1.0);
      glVertex3f(1.0, 2.5, 1.0);
      glVertex3f(1.0, 2.5, -1.0);
      glVertex3f(-1.0, 2.5, -1.0);

      /* bottom side face */
      glColor3f(0.7, 0.0, 0.1);  /* red */
      glVertex3f(-2.0, -2.5, 1.0);
      glVertex3f(1.0, -2.5, 1.0);
      glVertex3f(1.0, -2.5, -1.0);
      glVertex3f(-1.0, -2.5, -1.0);
    glEnd();
    glEndList();
    displayListInited = GL_TRUE;
  }
  if (doubleBuffer)
    glXSwapBuffers(scr->display, scr->window);/* buffer swap does implicit glFlush */
  else
    glFlush(); 
}

int create_x_window (int argc, char *argv[])
{
	/* Open connection to X server */
	scr->display = XOpenDisplay(NULL);
	if (scr->display == NULL) {
		fprintf(stderr, "ERROR: Couldn't connect to X server\n");
		return -1;
	}

	/* Check glx extension is supported */
	if (!glXQueryExtension(scr->display, &dummy, &dummy)) {
		fprintf(stderr, "ERROR: X server has no GLX extension\n");
		return -1;
	}

	/* Find an OpenGL capable visual */
	scr->visual_info = glXChooseVisual(
			scr->display,
			DefaultScreen(scr->display),
			double_gl_buf);

	if (scr->visual_info == NULL) {
		scr->visual_info = glXChooseVisual(
			scr->display,
			DefaultScreen(scr->display),
			single_gl_buf);

		if (scr->visual_info == NULL) {
			fprintf(stderr, "ERROR: No RGB visual with depth buffer\n");
			return -1;
		}
	}

	if (scr->visual_info->class != TrueColor) {
		fprintf(stderr, "ERROR: True color required for this program\n");
		return -1;
	}

	/* Create OpenGL rendering context */
	scr->glx_context = glXCreateContext(
			scr->display,
			scr->visual_info,
			None,
			GL_TRUE);
	
	if (scr->glx_context == NULL) {
		fprintf(stderr, "ERROR: Couldn't create rendering context\n");
		return -1;
	}

	/* Create X window with current context 
	 * Create an X colormap */
	scr->color_map = XCreateColormap(
			scr->display,
			RootWindow(scr->display, scr->visual_info->screen),
			scr->visual_info->visual,
			AllocNone);

	scr->window_attributes.colormap = scr->color_map;
	scr->window_attributes.border_pixel = 0;
	scr->window_attributes.event_mask = 
		KeyPressMask | ExposureMask | ButtonPressMask | StructureNotifyMask; 

	/* Create the window */
	scr->window = XCreateWindow(
			scr->display,
			RootWindow(scr->display, scr->visual_info->screen),
			0, 0, 800, 800, 0,
			scr->visual_info->depth,
			InputOutput,
			scr->visual_info->visual,
			CWBorderPixel | CWColormap | CWEventMask,
			&scr->window_attributes);

	XSetStandardProperties(
			scr->display,
			scr->window,
			"main", "main",
			None,
			argv, argc,
			NULL);

	/* Bind the rendering context to the current window */
	glXMakeCurrent(scr->display, scr->window, scr->glx_context);

	/* show window on the screen */
	XMapWindow(scr->display, scr->window);

	/* Configure OpenGL for context rendering */
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearDepth(1.0);

	/* Black frame buffer clears */
	glClearColor(0.0, 0.0, 0.0, 0.0);

	/* setup projection transform */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);

	/* Set initial veiwpoint */
	glViewport(0, 0, 300, 300);
	return 0;
}

void event_loop (void)
{
	char buf[1];
	XKeyEvent *key_event;
	KeySym keysym;

	for (;;) {
		do {
			XNextEvent(scr->display, &scr->event);
			switch (scr->event.type) {
				case KeyPress:
					/* check for escape key */
					key_event = (XKeyEvent*) &scr->event;
					if ((XLookupString((XKeyEvent*) &scr->event, buf, 1, &keysym, NULL)) == 1
								&& (keysym == (KeySym)XK_Escape))
							exit(0);
					break;
				case ButtonPress:
					recalcModelView	= GL_TRUE;
					switch (scr->event.xbutton.button) {
						case 1:
							xAngle += 10.0;
							break;
						case 2:
							yAngle += 10.0;
							break;
						case 3:
							zAngle += 10.0;
							break;
					}
					break;
				case ConfigureNotify:
					glViewport(0, 0, scr->event.xconfigure.width, scr->event.xconfigure.height);
					/* Fallthrough */
				case Expose:
					needRedraw = GL_TRUE; 
					break;
			}
		} while (XPending(scr->display)); /* loop to compress events */

		if (recalcModelView) {
			glMatrixMode(GL_MODELVIEW);
			
			glLoadIdentity();

			/* Move camera back 3 units */ 
			glTranslatef(0.0, 0.0, -3.0);

			/* Rotate */
			glRotatef(xAngle, 0.1, 0.0, 0.0);
			glRotatef(yAngle, 0.0, 0.1, 0.0);
			glRotatef(zAngle, 0.0, 0.0, 0.1);

			recalcModelView = GL_FALSE; 
			needRedraw = GL_TRUE;
		}

		if (needRedraw) {
			redraw();
			needRedraw = GL_FALSE;
		}
	}
}

int global_setup (int argc, char *argv[])
{
	scr = (SCR*)malloc(sizeof(*scr));
	if (scr == NULL) {
		fprintf(stderr, "ERROR: allocating memory for SCR\n");
		return -1;
	}
	
	if (create_x_window(argc,argv) != 0)
		return -1;

	event_loop();

	free(scr);
	return 0;
}

int main (int argc, char *argv[])
{
	global_setup(argc,argv);
	return 0;
}
