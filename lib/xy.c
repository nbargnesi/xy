/*
 * Copyright 2011-2012 Nick Bargnesi <nick@den-4.com>. All rights reserved.
 *
 * This file is part of xy.
 *
 * XY is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XY is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xy.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Portions of xy.c use code from dwm-6.0 (dwm.c). The dwm license is as
 * follows:
 *
 * MIT/X Consortium License
 *
 * © 2006-2011 Anselm R Garbe <anselm@garbe.us>
 * © 2007-2011 Peter Hartlich <sgkkr at hartlich dot com>
 * © 2010-2011 Connor Lane Smith <cls@lubutu.com>
 * © 2006-2009 Jukka Salmi <jukka at salmi dot ch>
 * © 2007-2009 Premysl Hruby <dfenze at gmail dot com>
 * © 2007-2009 Szabolcs Nagy <nszabolcs at gmail dot com>
 * © 2007-2009 Christof Musik <christof at sendfax dot de>
 * © 2009 Mate Nagy <mnagy at port70 dot net>
 * © 2007-2008 Enno Gottox Boland <gottox at s01 dot de>
 * © 2008 Martin Hurton <martin dot hurton at gmail dot com>
 * © 2008 Neale Pickett <neale dot woozle dot org>
 * © 2006-2007 Sander van Dijk <a dot h dot vandijk at gmail dot com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "xy.h"

//#define log log4c_category_log
#define FUNCTION_TRACE log_trace(globals->log, __FUNCTION__);

static void buttonpress(XEvent *e);
static void clientmessage(XEvent *e);
static void configurerequest(XEvent *e);
static void configurenotify(XEvent *e);
static void destroynotify(XEvent *e);
static void enternotify(XEvent *e);
static void _expose(XEvent *e);
static void focusin(XEvent *e);
static void keypress(XEvent *e);
static void mappingnotify(XEvent *e);
static void maprequest(XEvent *e);
static void motionnotify(XEvent *e);
static void propertynotify(XEvent *e);
static void unmapnotify(XEvent *e);
static void eventsink(XEvent *e);
static void _init();
static void (*handler[LASTEvent]) (XEvent *) = {
    [ButtonPress] = buttonpress,
    [ClientMessage] = clientmessage,
    [ConfigureRequest] = configurerequest,
    [ConfigureNotify] = configurenotify,
    [DestroyNotify] = destroynotify,
    [EnterNotify] = enternotify,
    [Expose] = _expose,
    [FocusIn] = focusin,
    [KeyPress] = keypress,
    [MappingNotify] = mappingnotify,
    [MapRequest] = maprequest,
    [MotionNotify] = motionnotify,
    [PropertyNotify] = propertynotify,
    [UnmapNotify] = unmapnotify,
    [FocusOut] = eventsink,
    [NoExpose] = eventsink,
    [CreateNotify] = eventsink,
    [MapNotify] = eventsink,
    [KeyRelease] = eventsink,
    [LeaveNotify] = eventsink,
    [ReparentNotify] = eventsink,
    [ButtonRelease] = eventsink,
    [KeymapNotify] = eventsink,
    [GraphicsExpose] = eventsink,
    [VisibilityNotify] = eventsink,
    [GravityNotify] = eventsink,
    [ResizeRequest] = eventsink,
    [CirculateNotify] = eventsink,
    [CirculateRequest] = eventsink,
    [SelectionClear] = eventsink,
    [SelectionRequest] = eventsink,
    [SelectionNotify] = eventsink,
    [ColormapNotify] = eventsink
};

void xy_init() {
    globals->dpy = open_display();
    if (!globals->dpy) {
        DIE_MSG("failed to open display");
    }
    globals->x_fd = ConnectionNumber(globals->dpy);
    globals->dflt_scrn = DefaultScreen(globals->dpy);
    configure(globals->cfg);
}

void xy_terminate() {
    free(globals);
}

void main_loop() {
    register_shutdown_hook(xy_cleanup);
    signal(SIGCHLD, SIG_IGN);
    ssize_t rslt;
    Display *d = globals->dpy;

    // XXX any >0 argument to epoll_create (see epoll_create man page)
    int epfd = epoll_create(1);

    if (epfd == -1) {
        perror("epoll_create()");
        fprintf(stderr, "(epoll_create returned %s)\n", strerror(errno));
        DIE;
    }

    EPOLL_EVENT epev_x, epev_ipc, epev_in, epev;
    memset(&epev_x, 0, sizeof(EPOLL_EVENT));
    memset(&epev_ipc, 0, sizeof(EPOLL_EVENT));
    memset(&epev_in, 0, sizeof(EPOLL_EVENT));
    memset(&epev, 0, sizeof(EPOLL_EVENT));

    epev_x.events = EPOLLIN;
    epev_x.data.fd = globals->x_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, globals->x_fd, &epev_x) == -1) {
        perror("epoll_ctl() globals->x_fd");
        fprintf(stderr, "(epoll_ctl returned %s)\n", strerror(errno));
        DIE;
    }

    epev_ipc.events = EPOLLIN;
    epev_ipc.data.fd = globals->ipc_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, globals->ipc_fd, &epev_ipc) == -1) {
        perror("epoll_ctl() globals->ipc_fd");
        fprintf(stderr, "(epoll_ctl returned %s)\n", strerror(errno));
        DIE;
    }

    epev_in.events = EPOLLIN;
    epev_in.data.fd = xy_in_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, xy_in_fd, &epev_in) == -1) {
        perror("epoll_ctl() xy_in_fd");
        fprintf(stderr, "(epoll_ctl returned %s)\n", strerror(errno));
        DIE;
    }

    XEvent xev;
    int nfds, i;
    char *ipc_buffer;

    _init();
    XSync(globals->dpy, True);

    while (1) {
        // Wait indefinitely for a maximum of one event
        nfds = epoll_wait(epfd, &epev, 1, -1);

        if (nfds == -1) {
            // Interrupted system call - continue
            if (errno == EINTR) continue;
            perror("epoll_wait()");
            fprintf(stderr, "(epoll_wait returned %d)\n", errno);
            DIE;
        }

        if (likely(epev.data.fd == globals->x_fd)) {
            while (XPending(globals->dpy)) {
                // X needs servicing, get the next event.
                XNextEvent(d, &xev);
                // Invoke a handler.
                handler[xev.type](&xev);
            }
        } else if (epev.data.fd == globals->ipc_fd) {
            // IPC needs servicing.
            log_debug(globals->log, "servicing IPC");
            ipc_buffer = malloc(MSG_LEN);
            memset(ipc_buffer, 0, MSG_LEN);
            rslt = read(globals->ipc_fd, ipc_buffer, MSG_LEN);
            process_ipc_buffer(ipc_buffer);
        } else if (epev.data.fd == xy_in_fd) {
            // inotify needs servicing.
            log_debug(globals->log, "servicing filesystem event");
            xy_inotify_read();
        }
    }
}

void ipc_quit() {
    transition(STATE_SHUTTING_DOWN);
}

void ipc_ping() {
    broadcast_send(PONG_MSG);
}

bool key_pressed(XKeyEvent *ev) {
    KeySym keysym = XKeycodeToKeysym(globals->dpy, ev->keycode, 0);
    if (is_ks_pressed(get_menu_shortcut(), ev)) {
        fprintf(stderr, "menu shortcut pressed\n");
    } else if (is_ks_pressed(get_terminal_shortcut(), ev)) {
        exec(get_terminal_command());
        return true;
    } else if (is_ks_pressed(get_quit_shortcut(), ev)) {
        transition(STATE_SHUTTING_DOWN);
    } else if (is_ks_pressed(get_restart_shortcut(), ev)) {
        transition(STATE_RESTARTING);
    }
    return false;
}

void key_released(XKeyEvent *ev) {
}

void configure_request(XConfigureRequestEvent *ev) {
}

void configure_notify(XConfigureEvent *ev) {
}

void destroy_notify(XDestroyWindowEvent *ev) {
}

void enter_notify(XCrossingEvent *ev) {
}

void expose(XExposeEvent *ev) {
}

void focus_event(XFocusChangeEvent *ev) {
}

void mapping_notify(XMappingEvent *ev) {
}

void map_request(XMapRequestEvent *ev) {
}

void property_notify(XPropertyEvent *ev) {
}

void unmap_notify(XUnmapEvent *ev) {
}

#define BUTTONMASK              (ButtonPressMask|ButtonReleaseMask)
#define CLEANMASK(mask)         (mask & ~(numlockmask|LockMask) & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))
#define INTERSECT(x,y,w,h,m)    (MAX(0, MIN((x)+(w),(m)->wx+(m)->ww) - MAX((x),(m)->wx)) \
                               * MAX(0, MIN((y)+(h),(m)->wy+(m)->wh) - MAX((y),(m)->wy)))
#define ISVISIBLE(C)            ((C->tags & C->mon->tagset[C->mon->seltags]))
#define LENGTH(X)               (sizeof X / sizeof X[0])
#define MAX(A, B)               ((A) > (B) ? (A) : (B))
#define MIN(A, B)               ((A) < (B) ? (A) : (B))
#define MOUSEMASK               (BUTTONMASK|PointerMotionMask)
#define WIDTH(X)                ((X)->w + 2 * (X)->bw)
#define HEIGHT(X)               ((X)->h + 2 * (X)->bw)
#define TAGMASK                 ((1 << LENGTH(tags)) - 1)
#define TEXTW(X)                (textnw(X, strlen(X)) + dc.font.height)

/* enums */
enum { CurNormal, CurResize, CurMove, CurLast };        /* cursor */
enum { ColBorder, ColFG, ColBG, ColLast };              /* color */
enum { NetSupported, NetWMName, NetWMState,
       NetWMFullscreen, NetActiveWindow, NetWMWindowType,
       NetWMWindowTypeDialog, NetLast };     /* EWMH atoms */
enum { WMProtocols, WMDelete, WMState, WMTakeFocus, WMLast }; /* default atoms */
enum { ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
       ClkClientWin, ClkRootWin, ClkLast };             /* clicks */

typedef union {
    int i;
    unsigned int ui;
    float f;
    const void *v;
} Arg;

typedef struct {
    unsigned int click;
    unsigned int mask;
    unsigned int button;
    void (*func)(const Arg *arg);
    const Arg arg;
} Button;

typedef struct _Monitor _Monitor;
typedef struct Client Client;
struct Client {
    char name[256];
    float mina, maxa;
    int x, y, w, h;
    int oldx, oldy, oldw, oldh;
    int basew, baseh, incw, inch, maxw, maxh, minw, minh;
    int bw, oldbw;
    unsigned int tags;
    Bool isfixed, isfloating, isurgent, neverfocus, oldstate, isfullscreen;
    Client *next;
    Client *snext;
    _Monitor *mon;
    Window win;
};

typedef struct {
    int x, y, w, h;
    unsigned long norm[ColLast];
    unsigned long sel[ColLast];
    Drawable drawable;
    GC gc;
    struct {
        int ascent;
        int descent;
        int height;
        XFontSet set;
        XFontStruct *xfont;
    } font;
} DC; /* draw context */

typedef struct {
    unsigned int mod;
    KeySym keysym;
    void (*func)(const Arg *);
    const Arg arg;
} Key;

typedef struct {
    const char *symbol;
    void (*arrange)(_Monitor *);
} Layout;

struct _Monitor {
    char ltsymbol[16];
    float mfact;
    int nmaster;
    int num;
    int by;               /* bar geometry */
    int mx, my, mw, mh;   /* screen size */
    int wx, wy, ww, wh;   /* window area  */
    unsigned int seltags;
    unsigned int sellt;
    unsigned int tagset[2];
    Bool showbar;
    Bool topbar;
    Client *clients;
    Client *sel;
    Client *stack;
    _Monitor *next;
    Window barwin;
    const Layout *lt[2];
};

typedef struct {
    const char *class;
    const char *instance;
    const char *title;
    unsigned int tags;
    Bool isfloating;
    int monitor;
} Rule;

/* function declarations */
static void _init();
static void applyrules(Client *c);
static Bool applysizehints(Client *c, int *x, int *y, int *w, int *h, Bool interact);
static void arrange(_Monitor *m);
static void arrangemon(_Monitor *m);
static void attach(Client *c);
static void attachstack(Client *c);
static void buttonpress(XEvent *e);
static void cleanupmon(_Monitor *mon);
static void clearurgent(Client *c);
static void _configure(Client *c);
static _Monitor *createmon(void);
static void detach(Client *c);
static void detachstack(Client *c);
static _Monitor *dirtomon(int dir);
static void drawbar(_Monitor *m);
static void drawbars(void);
static void drawsquare(Bool filled, Bool empty, Bool invert, unsigned long col[ColLast]);
static void drawtext(const char *text, unsigned long col[ColLast], Bool invert);
static void focus(Client *c);
static void focusmon(const Arg *arg);
static void focusstack(const Arg *arg);
static Bool getrootptr(int *x, int *y);
static long getstate(Window w);
static Bool gettextprop(Window w, Atom atom, char *text, unsigned int size);
static void grabbuttons(Client *c, Bool focused);
static void grabkeys(void);
static void incnmaster(const Arg *arg);
static void initfont(const char *fontstr);
static void killclient(const Arg *arg);
static void manage(Window w, XWindowAttributes *wa);
static void monocle(_Monitor *m);
static void movemouse(const Arg *arg);
static Client *nexttiled(Client *c);
static void pop(Client *);
static void quit(const Arg *arg);
static _Monitor *recttomon(int x, int y, int w, int h);
static void resize(Client *c, int x, int y, int w, int h, Bool interact);
static void resizeclient(Client *c, int x, int y, int w, int h);
static void resizemouse(const Arg *arg);
static void restack(_Monitor *m);
static void scan(void);
static Bool sendevent(Client *c, Atom proto);
static void sendmon(Client *c, _Monitor *m);
static void setclientstate(Client *c, long state);
static void setfocus(Client *c);
static void setfullscreen(Client *c, Bool fullscreen);
static void setlayout(const Arg *arg);
static void setmfact(const Arg *arg);
static void setup(void);
static void showhide(Client *c);
static void spawn(const Arg *arg);
static void tag(const Arg *arg);
static void tagmon(const Arg *arg);
static int textnw(const char *text, unsigned int len);
static void tile(_Monitor *);
static void togglebar(const Arg *arg);
static void togglefloating(const Arg *arg);
static void toggletag(const Arg *arg);
static void toggleview(const Arg *arg);
static void unfocus(Client *c, Bool setfocus);
static void unmanage(Client *c, Bool destroyed);
static Bool updategeom(void);
static void updatebarpos(_Monitor *m);
static void updatebars(void);
static void updatenumlockmask(void);
static void updatesizehints(Client *c);
static void updatestatus(void);
static void updatewindowtype(Client *c);
static void updatetitle(Client *c);
static void updatewmhints(Client *c);
static void view(const Arg *arg);
static void warp(const Client *c);
static Client *wintoclient(Window w);
static _Monitor *wintomon(Window w);
static int xerror(Display *, XErrorEvent *ee);
static int xerrordummy(Display *, XErrorEvent *ee);
static void zoom(const Arg *arg);

/* variables */
static const char broken[] = "broken";
static char stext[256];
static int screen;
static int sw, sh;           /* X display screen geometry width, height */
static int bh, blw = 0;      /* bar geometry */
static int (*xerrorxlib)(Display *, XErrorEvent *);
static unsigned int numlockmask = 0;
static Atom wmatom[WMLast], netatom[NetLast];
static Bool running = True;
static Cursor cursor[CurLast];
static DC dc;
static _Monitor *mons = NULL, *selmon = NULL;
static Window root;

static const char font[]            = "-lfp-bright-medium-r-normal--9-90-75-75-c-60-iso8859-1";
static const char normbordercolor[] = "#444444";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#005577";
static const char selbgcolor[]      = "#005577";
static const char selfgcolor[]      = "#eeeeee";
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* class      instance    title       tags mask     isfloating   monitor */
    { "Gimp",     NULL,       NULL,       0,            True,        -1 },
};

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = True; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* commands */
static const char *dmenucmd[] = { "njbdmenu", NULL };

static Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_r,      NULL,           {0} },
    { MODKEY,                       XK_Return, spawn,          {0} },
    { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
    { MODKEY,                       XK_b,      togglebar,      {0} },
    { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
    { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
    { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY,                       XK_space,  zoom,           {0} },
    { MODKEY,                       XK_Tab,    view,           {0} },
    { MODKEY,                       XK_c,      killclient,     {0} },
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
    { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
    { MODKEY,                       XK_End,    setlayout,      {0} },
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)
    { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

struct NumTags { char limitexceeded[LENGTH(tags) > 31 ? -1 : 1]; };

/* function implementations */
void
applyrules(Client *c) {
    const char *class, *instance;
    unsigned int i;
    const Rule *r;
    _Monitor *m;
    XClassHint ch = { NULL, NULL };

    /* rule matching */
    c->isfloating = c->tags = 0;
    XGetClassHint(globals->dpy, c->win, &ch);
    class    = ch.res_class ? ch.res_class : broken;
    instance = ch.res_name  ? ch.res_name  : broken;

    for (i = 0; i < LENGTH(rules); i++) {
        r = &rules[i];
        if ((!r->title || strstr(c->name, r->title))
        && (!r->class || strstr(class, r->class))
        && (!r->instance || strstr(instance, r->instance)))
        {
            c->isfloating = r->isfloating;
            c->tags |= r->tags;
            for (m = mons; m && m->num != r->monitor; m = m->next);
            if (m)
                c->mon = m;
        }
    }
    if (ch.res_class)
        XFree(ch.res_class);
    if (ch.res_name)
        XFree(ch.res_name);
    c->tags = c->tags & TAGMASK ? c->tags & TAGMASK : c->mon->tagset[c->mon->seltags];
}

Bool
applysizehints(Client *c, int *x, int *y, int *w, int *h, Bool interact) {
    Bool baseismin;
    _Monitor *m = c->mon;

    /* set minimum possible */
    *w = MAX(1, *w);
    *h = MAX(1, *h);
    if (interact) {
        if (*x > sw)
            *x = sw - WIDTH(c);
        if (*y > sh)
            *y = sh - HEIGHT(c);
        if (*x + *w + 2 * c->bw < 0)
            *x = 0;
        if (*y + *h + 2 * c->bw < 0)
            *y = 0;
    }
    else {
        if (*x >= m->wx + m->ww)
            *x = m->wx + m->ww - WIDTH(c);
        if (*y >= m->wy + m->wh)
            *y = m->wy + m->wh - HEIGHT(c);
        if (*x + *w + 2 * c->bw <= m->wx)
            *x = m->wx;
        if (*y + *h + 2 * c->bw <= m->wy)
            *y = m->wy;
    }
    if (*h < bh)
        *h = bh;
    if (*w < bh)
        *w = bh;
    if (resizehints || c->isfloating || !c->mon->lt[c->mon->sellt]->arrange) {
        /* see last two sentences in ICCCM 4.1.2.3 */
        baseismin = c->basew == c->minw && c->baseh == c->minh;
        if (!baseismin) { /* temporarily remove base dimensions */
            *w -= c->basew;
            *h -= c->baseh;
        }
        /* adjust for aspect limits */
        if (c->mina > 0 && c->maxa > 0) {
            if (c->maxa < (float)*w / *h)
                *w = *h * c->maxa + 0.5;
            else if (c->mina < (float)*h / *w)
                *h = *w * c->mina + 0.5;
        }
        if (baseismin) { /* increment calculation requires this */
            *w -= c->basew;
            *h -= c->baseh;
        }
        /* adjust for increment value */
        if (c->incw)
            *w -= *w % c->incw;
        if (c->inch)
            *h -= *h % c->inch;
        /* restore base dimensions */
        *w = MAX(*w + c->basew, c->minw);
        *h = MAX(*h + c->baseh, c->minh);
        if (c->maxw)
            *w = MIN(*w, c->maxw);
        if (c->maxh)
            *h = MIN(*h, c->maxh);
    }
    return *x != c->x || *y != c->y || *w != c->w || *h != c->h;
}

void
arrange(_Monitor *m) {
    if (m)
        showhide(m->stack);
    else for (m = mons; m; m = m->next)
        showhide(m->stack);
    if (m)
        arrangemon(m);
    else for (m = mons; m; m = m->next)
        arrangemon(m);
}

void
arrangemon(_Monitor *m) {
    strncpy(m->ltsymbol, m->lt[m->sellt]->symbol, sizeof m->ltsymbol);
    if (m->lt[m->sellt]->arrange)
        m->lt[m->sellt]->arrange(m);
    restack(m);
}

void
attach(Client *c) {
    c->next = c->mon->clients;
    c->mon->clients = c;
}

void
attachstack(Client *c) {
    c->snext = c->mon->stack;
    c->mon->stack = c;
}

void
buttonpress(XEvent *e) {
    unsigned int i, x, click;
    Arg arg = {0};
    Client *c;
    _Monitor *m;
    XButtonPressedEvent *ev = &e->xbutton;

    click = ClkRootWin;
    /* focus monitor if necessary */
    if ((m = wintomon(ev->window)) && m != selmon) {
        unfocus(selmon->sel, True);
        selmon = m;
        focus(NULL);
    }
    if (ev->window == selmon->barwin) {
        i = x = 0;
        do
            x += TEXTW(tags[i]);
        while ((unsigned int) ev->x >= x && ++i < LENGTH(tags));
        if (i < LENGTH(tags)) {
            click = ClkTagBar;
            arg.ui = 1 << i;
        }
        else if ((unsigned int) ev->x < x + blw)
            click = ClkLtSymbol;
        else if (ev->x > selmon->ww - TEXTW(stext))
            click = ClkStatusText;
        else
            click = ClkWinTitle;
    }
    else if ((c = wintoclient(ev->window))) {
        focus(c);
        click = ClkClientWin;
    }
    for (i = 0; i < LENGTH(buttons); i++)
        if (click == buttons[i].click && buttons[i].func && buttons[i].button == ev->button
        && CLEANMASK(buttons[i].mask) == CLEANMASK(ev->state))
            buttons[i].func(click == ClkTagBar && buttons[i].arg.i == 0 ? &arg : &buttons[i].arg);
}

void
xy_cleanup() {
    Arg a = {.ui = ~0};
    Layout foo = { "", NULL };
    _Monitor *m;

    view(&a);
    selmon->lt[selmon->sellt] = &foo;
    for (m = mons; m; m = m->next)
        while (m->stack) unmanage(m->stack, False);
    if (dc.font.set)
        XFreeFontSet(globals->dpy, dc.font.set);
    else
        XFreeFont(globals->dpy, dc.font.xfont);
    XUngrabKey(globals->dpy, AnyKey, AnyModifier, root);
    XFreePixmap(globals->dpy, dc.drawable);
    XFreeGC(globals->dpy, dc.gc);
    XFreeCursor(globals->dpy, cursor[CurNormal]);
    XFreeCursor(globals->dpy, cursor[CurResize]);
    XFreeCursor(globals->dpy, cursor[CurMove]);
    while (mons) cleanupmon(mons);
    XSync(globals->dpy, False);
    XSetInputFocus(globals->dpy, PointerRoot, RevertToPointerRoot, CurrentTime);
}

void
cleanupmon(_Monitor *mon) {
    _Monitor *m;

    if (mon == mons)
        mons = mons->next;
    else {
        for (m = mons; m && m->next != mon; m = m->next);
        m->next = mon->next;
    }
    XUnmapWindow(globals->dpy, mon->barwin);
    XDestroyWindow(globals->dpy, mon->barwin);
    free(mon);
}

void
clearurgent(Client *c) {
    XWMHints *wmh;

    c->isurgent = False;
    if (!(wmh = XGetWMHints(globals->dpy, c->win)))
        return;
    wmh->flags &= ~XUrgencyHint;
    XSetWMHints(globals->dpy, c->win, wmh);
    XFree(wmh);
}

void
clientmessage(XEvent *e) {
    XClientMessageEvent *cme = &e->xclient;
    Client *c = wintoclient(cme->window);

    if (!c)
        return;
    if (cme->message_type == netatom[NetWMState]) {
        if ((unsigned int) cme->data.l[1] == netatom[NetWMFullscreen] || (unsigned int) cme->data.l[2] == netatom[NetWMFullscreen])
            setfullscreen(c, (cme->data.l[0] == 1 /* _NET_WM_STATE_ADD    */
                          || (cme->data.l[0] == 2 /* _NET_WM_STATE_TOGGLE */ && !c->isfullscreen)));
    }
    else if (cme->message_type == netatom[NetActiveWindow]) {
        if (!ISVISIBLE(c)) {
            c->mon->seltags ^= 1;
            c->mon->tagset[c->mon->seltags] = c->tags;
        }
        pop(c);
    }
}

void
_configure(Client *c) {
    XConfigureEvent ce;

    ce.type = ConfigureNotify;
    ce.display = globals->dpy;
    ce.event = c->win;
    ce.window = c->win;
    ce.x = c->x;
    ce.y = c->y;
    ce.width = c->w;
    ce.height = c->h;
    ce.border_width = c->bw;
    ce.above = None;
    ce.override_redirect = False;
    XSendEvent(globals->dpy, c->win, False, StructureNotifyMask, (XEvent *)&ce);
}

void
configurenotify(XEvent *e) {
    _Monitor *m;
    XConfigureEvent *ev = &e->xconfigure;
    Bool dirty;

    if (ev->window == root) {
        dirty = (sw != ev->width);
        sw = ev->width;
        sh = ev->height;
        if (updategeom() || dirty) {
            if (dc.drawable != 0)
                XFreePixmap(globals->dpy, dc.drawable);
            dc.drawable = XCreatePixmap(globals->dpy, root, sw, bh, DefaultDepth(globals->dpy, screen));
            updatebars();
            for (m = mons; m; m = m->next)
                XMoveResizeWindow(globals->dpy, m->barwin, m->wx, m->by, m->ww, bh);
            focus(NULL);
            arrange(NULL);
        }
    }
}

void
configurerequest(XEvent *e) {
    Client *c;
    _Monitor *m;
    XConfigureRequestEvent *ev = &e->xconfigurerequest;
    XWindowChanges wc;

    if ((c = wintoclient(ev->window))) {
        if (ev->value_mask & CWBorderWidth)
            c->bw = ev->border_width;
        else if (c->isfloating || !selmon->lt[selmon->sellt]->arrange) {
            m = c->mon;
            if (ev->value_mask & CWX) {
                c->oldx = c->x;
                c->x = m->mx + ev->x;
            }
            if (ev->value_mask & CWY) {
                c->oldy = c->y;
                c->y = m->my + ev->y;
            }
            if (ev->value_mask & CWWidth) {
                c->oldw = c->w;
                c->w = ev->width;
            }
            if (ev->value_mask & CWHeight) {
                c->oldh = c->h;
                c->h = ev->height;
            }
            if ((c->x + c->w) > m->mx + m->mw && c->isfloating)
                c->x = m->mx + (m->mw / 2 - WIDTH(c) / 2); /* center in x direction */
            if ((c->y + c->h) > m->my + m->mh && c->isfloating)
                c->y = m->my + (m->mh / 2 - HEIGHT(c) / 2); /* center in y direction */
            if ((ev->value_mask & (CWX|CWY)) && !(ev->value_mask & (CWWidth|CWHeight)))
                _configure(c);
            if (ISVISIBLE(c))
                XMoveResizeWindow(globals->dpy, c->win, c->x, c->y, c->w, c->h);
        }
        else
            _configure(c);
    }
    else {
        wc.x = ev->x;
        wc.y = ev->y;
        wc.width = ev->width;
        wc.height = ev->height;
        wc.border_width = ev->border_width;
        wc.sibling = ev->above;
        wc.stack_mode = ev->detail;
        XConfigureWindow(globals->dpy, ev->window, ev->value_mask, &wc);
    }
    XSync(globals->dpy, False);
}

_Monitor *
createmon(void) {
    _Monitor *m;

    if (!(m = (_Monitor *)calloc(1, sizeof(_Monitor))))
        die("fatal: could not malloc() %u bytes\n", sizeof(_Monitor));
    m->tagset[0] = m->tagset[1] = 1;
    m->mfact = mfact;
    m->nmaster = nmaster;
    m->showbar = showbar;
    m->topbar = topbar;
    m->lt[0] = &layouts[0];
    m->lt[1] = &layouts[1 % LENGTH(layouts)];
    strncpy(m->ltsymbol, layouts[0].symbol, sizeof m->ltsymbol);
    return m;
}

void
destroynotify(XEvent *e) {
    Client *c;
    XDestroyWindowEvent *ev = &e->xdestroywindow;

    if ((c = wintoclient(ev->window)))
        unmanage(c, True);
}

void
detach(Client *c) {
    Client **tc;

    for (tc = &c->mon->clients; *tc && *tc != c; tc = &(*tc)->next);
    *tc = c->next;
}

void
detachstack(Client *c) {
    Client **tc, *t;

    for (tc = &c->mon->stack; *tc && *tc != c; tc = &(*tc)->snext);
    *tc = c->snext;

    if (c == c->mon->sel) {
        for (t = c->mon->stack; t && !ISVISIBLE(t); t = t->snext);
        c->mon->sel = t;
    }
}

void
die(const char *errstr, ...) {
    va_list ap;

    va_start(ap, errstr);
    vfprintf(stderr, errstr, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}

_Monitor *
dirtomon(int dir) {
    _Monitor *m = NULL;

    if (dir > 0) {
        if (!(m = selmon->next))
            m = mons;
    }
    else if (selmon == mons)
        for (m = mons; m->next; m = m->next);
    else
        for (m = mons; m->next != selmon; m = m->next);
    return m;
}

void
drawbar(_Monitor *m) {
    int x;
    unsigned int i, occ = 0, urg = 0;
    unsigned long *col;
    Client *c;

    for (c = m->clients; c; c = c->next) {
        occ |= c->tags;
        if (c->isurgent)
            urg |= c->tags;
    }
    dc.x = 0;
    for (i = 0; i < LENGTH(tags); i++) {
        dc.w = TEXTW(tags[i]);
        col = m->tagset[m->seltags] & 1 << i ? dc.sel : dc.norm;
        drawtext(tags[i], col, urg & 1 << i);
        drawsquare(m == selmon && selmon->sel && selmon->sel->tags & 1 << i,
                   occ & 1 << i, urg & 1 << i, col);
        dc.x += dc.w;
    }
    dc.w = blw = TEXTW(m->ltsymbol);
    drawtext(m->ltsymbol, dc.norm, False);
    dc.x += dc.w;
    x = dc.x;
    if (m == selmon) { /* status is only drawn on selected monitor */
        dc.w = TEXTW(stext);
        dc.x = m->ww - dc.w;
        if (dc.x < x) {
            dc.x = x;
            dc.w = m->ww - x;
        }
        drawtext(stext, dc.norm, False);
    }
    else
        dc.x = m->ww;
    if ((dc.w = dc.x - x) > bh) {
        dc.x = x;
        if (m->sel) {
            col = m == selmon ? dc.sel : dc.norm;
            drawtext(m->sel->name, col, False);
            drawsquare(m->sel->isfixed, m->sel->isfloating, False, col);
        }
        else
            drawtext(NULL, dc.norm, False);
    }
    XCopyArea(globals->dpy, dc.drawable, m->barwin, dc.gc, 0, 0, m->ww, bh, 0, 0);
    XSync(globals->dpy, False);
}

void
drawbars(void) {
    _Monitor *m;

    for (m = mons; m; m = m->next)
        drawbar(m);
}

void
drawsquare(Bool filled, Bool empty, Bool invert, unsigned long col[ColLast]) {
    int x;

    XSetForeground(globals->dpy, dc.gc, col[invert ? ColBG : ColFG]);
    x = (dc.font.ascent + dc.font.descent + 2) / 4;
    if (filled)
        XFillRectangle(globals->dpy, dc.drawable, dc.gc, dc.x+1, dc.y+1, x+1, x+1);
    else if (empty)
        XDrawRectangle(globals->dpy, dc.drawable, dc.gc, dc.x+1, dc.y+1, x, x);
}

void
drawtext(const char *text, unsigned long col[ColLast], Bool invert) {
    char buf[256];
    int x, y, h;
    unsigned int len, olen, i;

    XSetForeground(globals->dpy, dc.gc, col[invert ? ColFG : ColBG]);
    XFillRectangle(globals->dpy, dc.drawable, dc.gc, dc.x, dc.y, dc.w, dc.h);
    if (!text)
        return;
    olen = strlen(text);
    h = dc.font.ascent + dc.font.descent;
    y = dc.y + (dc.h / 2) - (h / 2) + dc.font.ascent;
    x = dc.x + (h / 2);
    /* shorten text if necessary */
    for (len = MIN(olen, sizeof buf); len && (unsigned int) textnw(text, len) > (unsigned int) dc.w - h; len--);
    if (!len)
        return;
    memcpy(buf, text, len);
    if (len < olen)
        for (i = len; i && i > len - 3; buf[--i] = '.');
    XSetForeground(globals->dpy, dc.gc, col[invert ? ColBG : ColFG]);
    if (dc.font.set)
        XmbDrawString(globals->dpy, dc.drawable, dc.font.set, dc.gc, x, y, buf, len);
    else
        XDrawString(globals->dpy, dc.drawable, dc.gc, x, y, buf, len);
}

void
enternotify(XEvent *e) {
    Client *c;
    _Monitor *m;
    XCrossingEvent *ev = &e->xcrossing;

    if ((ev->mode != NotifyNormal || ev->detail == NotifyInferior) && ev->window != root)
        return;
    c = wintoclient(ev->window);
    m = c ? c->mon : wintomon(ev->window);
    if (m != selmon) {
        unfocus(selmon->sel, True);
        selmon = m;
    }
    else if (!c || c == selmon->sel)
        return;
    focus(c);
}

void
_expose(XEvent *e) {
    _Monitor *m;
    XExposeEvent *ev = &e->xexpose;

    if (ev->count == 0 && (m = wintomon(ev->window)))
        drawbar(m);
}

void
focus(Client *c) {
    if (!c || !ISVISIBLE(c))
        for (c = selmon->stack; c && !ISVISIBLE(c); c = c->snext);
    /* was if (selmon->sel) */
    if (selmon->sel && selmon->sel != c)
        unfocus(selmon->sel, False);
    if (c) {
        if (c->mon != selmon)
            selmon = c->mon;
        if (c->isurgent)
            clearurgent(c);
        detachstack(c);
        attachstack(c);
        grabbuttons(c, True);
        XSetWindowBorder(globals->dpy, c->win, dc.sel[ColBorder]);
        setfocus(c);
    }
    else
        XSetInputFocus(globals->dpy, root, RevertToPointerRoot, CurrentTime);
    selmon->sel = c;
    drawbars();
}

void
focusin(XEvent *e) { /* there are some broken focus acquiring clients */
    XFocusChangeEvent *ev = &e->xfocus;

    if (selmon->sel && ev->window != selmon->sel->win)
        setfocus(selmon->sel);
}

void
focusmon(const Arg *arg) {
    _Monitor *m;

    if (!mons->next)
        return;
    if ((m = dirtomon(arg->i)) == selmon)
        return;
    unfocus(selmon->sel, True);
    selmon = m;
    focus(NULL);
    warp(selmon->sel);
}

void
focusstack(const Arg *arg) {
    Client *c = NULL, *i;

    if (!selmon->sel)
        return;
    if (arg->i > 0) {
        for (c = selmon->sel->next; c && !ISVISIBLE(c); c = c->next);
        if (!c)
            for (c = selmon->clients; c && !ISVISIBLE(c); c = c->next);
    }
    else {
        for (i = selmon->clients; i != selmon->sel; i = i->next)
            if (ISVISIBLE(i))
                c = i;
        if (!c)
            for (; i; i = i->next)
                if (ISVISIBLE(i))
                    c = i;
    }
    if (c) {
        focus(c);
        restack(selmon);
    }
}

Atom
getatomprop(Client *c, Atom prop) {
    int di;
    unsigned long dl;
    unsigned char *p = NULL;
    Atom da, atom = None;

    if (XGetWindowProperty(globals->dpy, c->win, prop, 0L, sizeof atom, False, XA_ATOM,
                          &da, &di, &dl, &dl, &p) == Success && p) {
        atom = *(Atom *)p;
        XFree(p);
    }
    return atom;
}

Bool
getrootptr(int *x, int *y) {
    int di;
    unsigned int dui;
    Window dummy;

    return XQueryPointer(globals->dpy, root, &dummy, &dummy, x, y, &di, &di, &dui);
}

long
getstate(Window w) {
    int format;
    long result = -1;
    unsigned char *p = NULL;
    unsigned long n, extra;
    Atom real;

    if (XGetWindowProperty(globals->dpy, w, wmatom[WMState], 0L, 2L, False, wmatom[WMState],
                          &real, &format, &n, &extra, (unsigned char **)&p) != Success)
        return -1;
    if (n != 0)
        result = *p;
    XFree(p);
    return result;
}

Bool
gettextprop(Window w, Atom atom, char *text, unsigned int size) {
    char **list = NULL;
    int n;
    XTextProperty name;

    if (!text || size == 0)
        return False;
    text[0] = '\0';
    XGetTextProperty(globals->dpy, w, &name, atom);
    if (!name.nitems)
        return False;
    if (name.encoding == XA_STRING)
        strncpy(text, (char *)name.value, size - 1);
    else {
        if (XmbTextPropertyToTextList(globals->dpy, &name, &list, &n) >= Success && n > 0 && *list) {
            strncpy(text, *list, size - 1);
            XFreeStringList(list);
        }
    }
    text[size - 1] = '\0';
    XFree(name.value);
    return True;
}

void
grabbuttons(Client *c, Bool focused) {
    updatenumlockmask();
    {
        unsigned int i, j;
        unsigned int modifiers[] = { 0, LockMask, numlockmask, numlockmask|LockMask };
        XUngrabButton(globals->dpy, AnyButton, AnyModifier, c->win);
        if (focused) {
            for (i = 0; i < LENGTH(buttons); i++)
                if (buttons[i].click == ClkClientWin)
                    for (j = 0; j < LENGTH(modifiers); j++)
                        XGrabButton(globals->dpy, buttons[i].button,
                                    buttons[i].mask | modifiers[j],
                                    c->win, False, BUTTONMASK,
                                    GrabModeAsync, GrabModeSync, None, None);
        }
        else
            XGrabButton(globals->dpy, AnyButton, AnyModifier, c->win, False,
                        BUTTONMASK, GrabModeAsync, GrabModeSync, None, None);
    }
}

void
grabkeys(void) {
    updatenumlockmask();
    {
        unsigned int i, j;
        unsigned int modifiers[] = { 0, LockMask, numlockmask, numlockmask|LockMask };
        KeyCode code;

        XUngrabKey(globals->dpy, AnyKey, AnyModifier, root);
        for (i = 0; i < LENGTH(keys); i++)
            if ((code = XKeysymToKeycode(globals->dpy, keys[i].keysym)))
                for (j = 0; j < LENGTH(modifiers); j++)
                    XGrabKey(globals->dpy, code, keys[i].mod | modifiers[j], root,
                         True, GrabModeAsync, GrabModeAsync);
    }
}

void
incnmaster(const Arg *arg) {
    selmon->nmaster = MAX(selmon->nmaster + arg->i, 0);
    arrange(selmon);
}

void
initfont(const char *fontstr) {
    char *def, **missing;
    int n;

    dc.font.set = XCreateFontSet(globals->dpy, fontstr, &missing, &n, &def);
    if (missing) {
        XFreeStringList(missing);
    }
    if (dc.font.set) {
        XFontStruct **xfonts;
        char **font_names;

        dc.font.ascent = dc.font.descent = 0;
        XExtentsOfFontSet(dc.font.set);
        n = XFontsOfFontSet(dc.font.set, &xfonts, &font_names);
        while (n--) {
            dc.font.ascent = MAX(dc.font.ascent, (*xfonts)->ascent);
            dc.font.descent = MAX(dc.font.descent,(*xfonts)->descent);
            xfonts++;
        }
    }
    else {
        if (!(dc.font.xfont = XLoadQueryFont(globals->dpy, fontstr))
        && !(dc.font.xfont = XLoadQueryFont(globals->dpy, "fixed")))
            die("error, cannot load font: '%s'\n", fontstr);
        dc.font.ascent = dc.font.xfont->ascent;
        dc.font.descent = dc.font.xfont->descent;
    }
    dc.font.height = dc.font.ascent + dc.font.descent;
}

static Bool
isuniquegeom(XineramaScreenInfo *unique, size_t n, XineramaScreenInfo *info) {
    while (n--)
        if (unique[n].x_org == info->x_org && unique[n].y_org == info->y_org
        && unique[n].width == info->width && unique[n].height == info->height)
            return False;
    return True;
}

inline void eventsink(XEvent *e) {
    // Ignore this event
}

void
keypress(XEvent *e) {
    XKeyEvent *ev = &e->xkey;
    if (key_pressed(ev)) return;
    unsigned int i;
    KeySym keysym;

    keysym = XKeycodeToKeysym(globals->dpy, (KeyCode)ev->keycode, 0);
    for (i = 0; i < LENGTH(keys); i++)
        if (keysym == keys[i].keysym
        && CLEANMASK(keys[i].mod) == CLEANMASK(ev->state)
        && keys[i].func)
            keys[i].func(&(keys[i].arg));
}

void
killclient(const Arg *arg) {
    if (!selmon->sel)
        return;
    if (!sendevent(selmon->sel, wmatom[WMDelete])) {
        XGrabServer(globals->dpy);
        XSetErrorHandler(xerrordummy);
        XSetCloseDownMode(globals->dpy, DestroyAll);
        XKillClient(globals->dpy, selmon->sel->win);
        XSync(globals->dpy, False);
        XSetErrorHandler(xerror);
        XUngrabServer(globals->dpy);
    }
}

void
manage(Window w, XWindowAttributes *wa) {
    Client *c, *t = NULL;
    Window trans = None;
    XWindowChanges wc;

    if (!(c = calloc(1, sizeof(Client))))
        die("fatal: could not malloc() %u bytes\n", sizeof(Client));
    c->win = w;
    updatetitle(c);
    if (XGetTransientForHint(globals->dpy, w, &trans) && (t = wintoclient(trans))) {
        c->mon = t->mon;
        c->tags = t->tags;
    }
    else {
        c->mon = selmon;
        applyrules(c);
    }
    /* geometry */
    c->x = c->oldx = wa->x;
    c->y = c->oldy = wa->y;
    c->w = c->oldw = wa->width;
    c->h = c->oldh = wa->height;
    c->oldbw = wa->border_width;

    if (c->x + WIDTH(c) > c->mon->mx + c->mon->mw)
        c->x = c->mon->mx + c->mon->mw - WIDTH(c);
    if (c->y + HEIGHT(c) > c->mon->my + c->mon->mh)
        c->y = c->mon->my + c->mon->mh - HEIGHT(c);
    c->x = MAX(c->x, c->mon->mx);
    /* only fix client y-offset, if the client center might cover the bar */
    c->y = MAX(c->y, ((c->mon->by == c->mon->my) && (c->x + (c->w / 2) >= c->mon->wx)
               && (c->x + (c->w / 2) < c->mon->wx + c->mon->ww)) ? bh : c->mon->my);
    c->bw = borderpx;

    wc.border_width = c->bw;
    XConfigureWindow(globals->dpy, w, CWBorderWidth, &wc);
    XSetWindowBorder(globals->dpy, w, dc.norm[ColBorder]);
    _configure(c); /* propagates border_width, if size doesn't change */
    updatewindowtype(c);
    updatesizehints(c);
    updatewmhints(c);
    XSelectInput(globals->dpy, w, EnterWindowMask|FocusChangeMask|PropertyChangeMask|StructureNotifyMask);
    grabbuttons(c, False);
    if (!c->isfloating)
        c->isfloating = c->oldstate = trans != None || c->isfixed;
    if (c->isfloating)
        XRaiseWindow(globals->dpy, c->win);
    attach(c);
    attachstack(c);
    XMoveResizeWindow(globals->dpy, c->win, c->x + 2 * sw, c->y, c->w, c->h); /* some windows require this */
    setclientstate(c, NormalState);
    if (c->mon == selmon)
        unfocus(selmon->sel, False);
    c->mon->sel = c;
    arrange(c->mon);
    XMapWindow(globals->dpy, c->win);
    focus(NULL);
}

void
mappingnotify(XEvent *e) {
    XMappingEvent *ev = &e->xmapping;

    XRefreshKeyboardMapping(ev);
    if (ev->request == MappingKeyboard)
        grabkeys();
}

void
maprequest(XEvent *e) {
    static XWindowAttributes wa;
    XMapRequestEvent *ev = &e->xmaprequest;

    if (!XGetWindowAttributes(globals->dpy, ev->window, &wa))
        return;
    if (wa.override_redirect)
        return;
    if (!wintoclient(ev->window))
        manage(ev->window, &wa);
}

void
monocle(_Monitor *m) {
    unsigned int n = 0;
    Client *c;

    for (c = m->clients; c; c = c->next)
        if (ISVISIBLE(c))
            n++;
    if (n > 0) /* override layout symbol */
        snprintf(m->ltsymbol, sizeof m->ltsymbol, "[%d]", n);
    for (c = nexttiled(m->clients); c; c = nexttiled(c->next))
        resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, False);
}

void
motionnotify(XEvent *e) {
    static _Monitor *mon = NULL;
    _Monitor *m;
    XMotionEvent *ev = &e->xmotion;

    if (ev->window != root)
        return;
    if ((m = recttomon(ev->x_root, ev->y_root, 1, 1)) != mon && mon) {
        selmon = m;
        focus(NULL);
    }
    mon = m;
}

void
movemouse(const Arg *arg) {
    int x, y, ocx, ocy, nx, ny;
    Client *c;
    _Monitor *m;
    XEvent ev;

    if (!(c = selmon->sel))
        return;
    restack(selmon);
    ocx = c->x;
    ocy = c->y;
    if (XGrabPointer(globals->dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
    None, cursor[CurMove], CurrentTime) != GrabSuccess)
        return;
    if (!getrootptr(&x, &y))
        return;
    do {
        XMaskEvent(globals->dpy, MOUSEMASK|ExposureMask|SubstructureRedirectMask, &ev);
        switch(ev.type) {
        case ConfigureRequest:
        case Expose:
        case MapRequest:
            handler[ev.type](&ev);
            break;
        case MotionNotify:
            nx = ocx + (ev.xmotion.x - x);
            ny = ocy + (ev.xmotion.y - y);
            if (nx >= selmon->wx && nx <= selmon->wx + selmon->ww
            && ny >= selmon->wy && ny <= selmon->wy + selmon->wh) {
                if ((unsigned int) abs(selmon->wx - nx) < (unsigned int) snap)
                    nx = selmon->wx;
                else if ((unsigned int) abs((selmon->wx + selmon->ww) - (nx + WIDTH(c))) < (unsigned int) snap)
                    nx = selmon->wx + selmon->ww - WIDTH(c);
                if ((unsigned int) abs(selmon->wy - ny) < (unsigned int) snap)
                    ny = selmon->wy;
                else if ((unsigned int) abs((selmon->wy + selmon->wh) - (ny + HEIGHT(c))) < (unsigned int) snap)
                    ny = selmon->wy + selmon->wh - HEIGHT(c);
                if (!c->isfloating && selmon->lt[selmon->sellt]->arrange
                && ((unsigned int) abs(nx - c->x) > (unsigned int) snap || (unsigned int) abs(ny - c->y) > (unsigned int) snap))
                    togglefloating(NULL);
            }
            if (!selmon->lt[selmon->sellt]->arrange || c->isfloating)
                resize(c, nx, ny, c->w, c->h, True);
            break;
        }
    } while (ev.type != ButtonRelease);
    XUngrabPointer(globals->dpy, CurrentTime);
    if ((m = recttomon(c->x, c->y, c->w, c->h)) != selmon) {
        sendmon(c, m);
        selmon = m;
        focus(NULL);
    }
}

Client *
nexttiled(Client *c) {
    for (; c && (c->isfloating || !ISVISIBLE(c)); c = c->next);
    return c;
}

void
pop(Client *c) {
    detach(c);
    attach(c);
    focus(c);
    arrange(c->mon);
}

void
propertynotify(XEvent *e) {
    Client *c;
    Window trans;
    XPropertyEvent *ev = &e->xproperty;

    if ((ev->window == root) && (ev->atom == XA_WM_NAME))
        updatestatus();
    else if (ev->state == PropertyDelete)
        return; /* ignore */
    else if ((c = wintoclient(ev->window))) {
        switch(ev->atom) {
        default: break;
        case XA_WM_TRANSIENT_FOR:
            if (!c->isfloating && (XGetTransientForHint(globals->dpy, c->win, &trans)) &&
               (c->isfloating = (wintoclient(trans)) != NULL))
                arrange(c->mon);
            break;
        case XA_WM_NORMAL_HINTS:
            updatesizehints(c);
            break;
        case XA_WM_HINTS:
            updatewmhints(c);
            drawbars();
            break;
        }
        if (ev->atom == XA_WM_NAME || ev->atom == netatom[NetWMName]) {
            updatetitle(c);
            if (c == c->mon->sel)
                drawbar(c->mon);
        }
        if (ev->atom == netatom[NetWMWindowType])
            updatewindowtype(c);
    }
}

void
quit(const Arg *arg) {
    transition(STATE_SHUTTING_DOWN);
}

_Monitor *
recttomon(int x, int y, int w, int h) {
    _Monitor *m, *r = selmon;
    int a, area = 0;

    for (m = mons; m; m = m->next)
        if ((a = INTERSECT(x, y, w, h, m)) > area) {
            area = a;
            r = m;
        }
    return r;
}

void
resize(Client *c, int x, int y, int w, int h, Bool interact) {
    if (applysizehints(c, &x, &y, &w, &h, interact))
        resizeclient(c, x, y, w, h);
}

void
resizeclient(Client *c, int x, int y, int w, int h) {
    XWindowChanges wc;

    c->oldx = c->x; c->x = wc.x = x;
    c->oldy = c->y; c->y = wc.y = y;
    c->oldw = c->w; c->w = wc.width = w;
    c->oldh = c->h; c->h = wc.height = h;
    wc.border_width = c->bw;
    XConfigureWindow(globals->dpy, c->win, CWX|CWY|CWWidth|CWHeight|CWBorderWidth, &wc);
    _configure(c);
    XSync(globals->dpy, False);
}

void
resizemouse(const Arg *arg) {
    int ocx, ocy;
    int nw, nh;
    Client *c;
    _Monitor *m;
    XEvent ev;

    if (!(c = selmon->sel))
        return;
    restack(selmon);
    ocx = c->x;
    ocy = c->y;
    if (XGrabPointer(globals->dpy, root, False, MOUSEMASK, GrabModeAsync, GrabModeAsync,
                    None, cursor[CurResize], CurrentTime) != GrabSuccess)
        return;
    XWarpPointer(globals->dpy, None, c->win, 0, 0, 0, 0, c->w + c->bw - 1, c->h + c->bw - 1);
    do {
        XMaskEvent(globals->dpy, MOUSEMASK|ExposureMask|SubstructureRedirectMask, &ev);
        switch(ev.type) {
        case ConfigureRequest:
        case Expose:
        case MapRequest:
            handler[ev.type](&ev);
            break;
        case MotionNotify:
            nw = MAX(ev.xmotion.x - ocx - 2 * c->bw + 1, 1);
            nh = MAX(ev.xmotion.y - ocy - 2 * c->bw + 1, 1);
            if (c->mon->wx + nw >= selmon->wx && c->mon->wx + nw <= selmon->wx + selmon->ww
            && c->mon->wy + nh >= selmon->wy && c->mon->wy + nh <= selmon->wy + selmon->wh)
            {
                if (!c->isfloating && selmon->lt[selmon->sellt]->arrange
                && ((unsigned int) abs(nw - c->w) > (unsigned int) snap || (unsigned int) abs(nh - c->h) > (unsigned int) snap))
                    togglefloating(NULL);
            }
            if (!selmon->lt[selmon->sellt]->arrange || c->isfloating)
                resize(c, c->x, c->y, nw, nh, True);
            break;
        }
    } while (ev.type != ButtonRelease);
    XWarpPointer(globals->dpy, None, c->win, 0, 0, 0, 0, c->w + c->bw - 1, c->h + c->bw - 1);
    XUngrabPointer(globals->dpy, CurrentTime);
    while (XCheckMaskEvent(globals->dpy, EnterWindowMask, &ev));
    if ((m = recttomon(c->x, c->y, c->w, c->h)) != selmon) {
        sendmon(c, m);
        selmon = m;
        focus(NULL);
    }
}

void
restack(_Monitor *m) {
    Client *c;
    XEvent ev;
    XWindowChanges wc;

    drawbar(m);
    if (!m->sel)
        return;
    if (m->sel->isfloating || !m->lt[m->sellt]->arrange)
        XRaiseWindow(globals->dpy, m->sel->win);
    if (m->lt[m->sellt]->arrange) {
        wc.stack_mode = Below;
        wc.sibling = m->barwin;
        for (c = m->stack; c; c = c->snext)
            if (!c->isfloating && ISVISIBLE(c)) {
                XConfigureWindow(globals->dpy, c->win, CWSibling|CWStackMode, &wc);
                wc.sibling = c->win;
            }
    }
    XSync(globals->dpy, False);
    while (XCheckMaskEvent(globals->dpy, EnterWindowMask, &ev));
    if (m == selmon && (m->tagset[m->seltags] & m->sel->tags))
        warp(m->sel);
}

/*
void
run(void) {
    XEvent ev;
    XSync(globals->dpy, False);
    while (running && !XNextEvent(globals->dpy, &ev))
        if (handler[ev.type])
            handler[ev.type](&ev);
}
*/

void
scan(void) {
    unsigned int i, num;
    Window d1, d2, *wins = NULL;
    XWindowAttributes wa;

    if (XQueryTree(globals->dpy, root, &d1, &d2, &wins, &num)) {
        for (i = 0; i < num; i++) {
            if (!XGetWindowAttributes(globals->dpy, wins[i], &wa)
            || wa.override_redirect || XGetTransientForHint(globals->dpy, wins[i], &d1))
                continue;
            if (wa.map_state == IsViewable || getstate(wins[i]) == IconicState)
                manage(wins[i], &wa);
        }
        for (i = 0; i < num; i++) { /* now the transients */
            if (!XGetWindowAttributes(globals->dpy, wins[i], &wa))
                continue;
            if (XGetTransientForHint(globals->dpy, wins[i], &d1)
            && (wa.map_state == IsViewable || getstate(wins[i]) == IconicState))
                manage(wins[i], &wa);
        }
        if (wins)
            XFree(wins);
    }
}

void
sendmon(Client *c, _Monitor *m) {
    if (c->mon == m)
        return;
    unfocus(c, True);
    detach(c);
    detachstack(c);
    c->mon = m;
    c->tags = m->tagset[m->seltags]; /* assign tags of target monitor */
    attach(c);
    attachstack(c);
    focus(NULL);
    arrange(NULL);
}

void
setclientstate(Client *c, long state) {
    long data[] = { state, None };

    XChangeProperty(globals->dpy, c->win, wmatom[WMState], wmatom[WMState], 32,
            PropModeReplace, (unsigned char *)data, 2);
}

Bool
sendevent(Client *c, Atom proto) {
    int n;
    Atom *protocols;
    Bool exists = False;
    XEvent ev;

    if (XGetWMProtocols(globals->dpy, c->win, &protocols, &n)) {
        while (!exists && n--)
            exists = protocols[n] == proto;
        XFree(protocols);
    }
    if (exists) {
        ev.type = ClientMessage;
        ev.xclient.window = c->win;
        ev.xclient.message_type = wmatom[WMProtocols];
        ev.xclient.format = 32;
        ev.xclient.data.l[0] = proto;
        ev.xclient.data.l[1] = CurrentTime;
        XSendEvent(globals->dpy, c->win, False, NoEventMask, &ev);
    }
    return exists;
}

void
setfocus(Client *c) {
    if (!c->neverfocus)
        XSetInputFocus(globals->dpy, c->win, RevertToPointerRoot, CurrentTime);
    sendevent(c, wmatom[WMTakeFocus]);
}

void
setfullscreen(Client *c, Bool fullscreen) {
    if (fullscreen) {
        XChangeProperty(globals->dpy, c->win, netatom[NetWMState], XA_ATOM, 32,
                        PropModeReplace, (unsigned char*)&netatom[NetWMFullscreen], 1);
        c->isfullscreen = True;
        c->oldstate = c->isfloating;
        c->oldbw = c->bw;
        c->bw = 0;
        c->isfloating = True;
        resizeclient(c, c->mon->mx, c->mon->my, c->mon->mw, c->mon->mh);
        XRaiseWindow(globals->dpy, c->win);
    }
    else {
        XChangeProperty(globals->dpy, c->win, netatom[NetWMState], XA_ATOM, 32,
                        PropModeReplace, (unsigned char*)0, 0);
        c->isfullscreen = False;
        c->isfloating = c->oldstate;
        c->bw = c->oldbw;
        c->x = c->oldx;
        c->y = c->oldy;
        c->w = c->oldw;
        c->h = c->oldh;
        resizeclient(c, c->x, c->y, c->w, c->h);
        arrange(c->mon);
    }
}

void
setlayout(const Arg *arg) {
    if (!arg || !arg->v || arg->v != selmon->lt[selmon->sellt])
        selmon->sellt ^= 1;
    if (arg && arg->v)
        selmon->lt[selmon->sellt] = (Layout *)arg->v;
    strncpy(selmon->ltsymbol, selmon->lt[selmon->sellt]->symbol, sizeof selmon->ltsymbol);
    if (selmon->sel)
        arrange(selmon);
    else
        drawbar(selmon);
}

/* arg > 1.0 will set mfact absolutly */
void
setmfact(const Arg *arg) {
    float f;

    if (!arg || !selmon->lt[selmon->sellt]->arrange)
        return;
    f = arg->f < 1.0 ? arg->f + selmon->mfact : arg->f - 1.0;
    if (f < 0.1 || f > 0.9)
        return;
    selmon->mfact = f;
    arrange(selmon);
}

void
setup(void) {
    XSetWindowAttributes wa;

    signal(SIGCHLD, SIG_IGN);
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = SA_RESTART | SA_NOCLDWAIT;
    sigaction(SIGCHLD, &sa, NULL);

    /* init screen */
    screen = DefaultScreen(globals->dpy);
    root = RootWindow(globals->dpy, screen);
    initfont(font);
    sw = DisplayWidth(globals->dpy, screen);
    sh = DisplayHeight(globals->dpy, screen);
    bh = dc.h = dc.font.height + 2;
    updategeom();
    /* init atoms */
    wmatom[WMProtocols] = XInternAtom(globals->dpy, "WM_PROTOCOLS", False);
    wmatom[WMDelete] = XInternAtom(globals->dpy, "WM_DELETE_WINDOW", False);
    wmatom[WMState] = XInternAtom(globals->dpy, "WM_STATE", False);
    wmatom[WMTakeFocus] = XInternAtom(globals->dpy, "WM_TAKE_FOCUS", False);
    netatom[NetActiveWindow] = XInternAtom(globals->dpy, "_NET_ACTIVE_WINDOW", False);
    netatom[NetSupported] = XInternAtom(globals->dpy, "_NET_SUPPORTED", False);
    netatom[NetWMName] = XInternAtom(globals->dpy, "_NET_WM_NAME", False);
    netatom[NetWMState] = XInternAtom(globals->dpy, "_NET_WM_STATE", False);
    netatom[NetWMFullscreen] = XInternAtom(globals->dpy, "_NET_WM_STATE_FULLSCREEN", False);
    netatom[NetWMWindowType] = XInternAtom(globals->dpy, "_NET_WM_WINDOW_TYPE", False);
    netatom[NetWMWindowTypeDialog] = XInternAtom(globals->dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False);
    /* init cursors */
    cursor[CurNormal] = XCreateFontCursor(globals->dpy, XC_left_ptr);
    cursor[CurResize] = XCreateFontCursor(globals->dpy, XC_sizing);
    cursor[CurMove] = XCreateFontCursor(globals->dpy, XC_fleur);
    /* init appearance */
    dc.norm[ColBorder] = get_color(normbordercolor, globals->dpy, NULL);
    dc.norm[ColBG] = get_color(normbgcolor, globals->dpy, NULL);
    dc.norm[ColFG] = get_color(normfgcolor, globals->dpy, NULL);
    dc.sel[ColBorder] = get_color(selbordercolor, globals->dpy, NULL);
    dc.sel[ColBG] = get_color(selbgcolor, globals->dpy, NULL);
    dc.sel[ColFG] = get_color(selfgcolor, globals->dpy, NULL);
    dc.drawable = XCreatePixmap(globals->dpy, root, DisplayWidth(globals->dpy, screen), bh, DefaultDepth(globals->dpy, screen));
    dc.gc = XCreateGC(globals->dpy, root, 0, NULL);
    XSetLineAttributes(globals->dpy, dc.gc, 1, LineSolid, CapButt, JoinMiter);
    if (!dc.font.set)
        XSetFont(globals->dpy, dc.gc, dc.font.xfont->fid);
    /* init bars */
    updatebars();
    updatestatus();
    /* EWMH support per view */
    XChangeProperty(globals->dpy, root, netatom[NetSupported], XA_ATOM, 32,
            PropModeReplace, (unsigned char *) netatom, NetLast);
    /* select for events */
    wa.cursor = cursor[CurNormal];
    wa.event_mask = SubstructureRedirectMask|SubstructureNotifyMask|ButtonPressMask|PointerMotionMask
                    |EnterWindowMask|LeaveWindowMask|StructureNotifyMask|PropertyChangeMask;
    XChangeWindowAttributes(globals->dpy, root, CWEventMask|CWCursor, &wa);
    XSelectInput(globals->dpy, root, wa.event_mask);
    grabkeys();
}

void
showhide(Client *c) {
    if (!c)
        return;
    if (ISVISIBLE(c)) { /* show clients top down */
        XMoveWindow(globals->dpy, c->win, c->x, c->y);
        if ((!c->mon->lt[c->mon->sellt]->arrange || c->isfloating) && !c->isfullscreen)
            resize(c, c->x, c->y, c->w, c->h, False);
        showhide(c->snext);
    }
    else { /* hide clients bottom up */
        showhide(c->snext);
        XMoveWindow(globals->dpy, c->win, WIDTH(c) * -2, c->y);
    }
}

void
spawn(const Arg *arg) {
    if (fork() == 0) {
        if (globals->dpy)
            close(ConnectionNumber(globals->dpy));
        setsid();
        execvp(((char **)arg->v)[0], (char **)arg->v);
        fprintf(stderr, "execvp %s", ((char **)arg->v)[0]);
        perror(" failed");
        exit(EXIT_SUCCESS);
    }
}

void
tag(const Arg *arg) {
    if (selmon->sel && arg->ui & TAGMASK) {
        selmon->sel->tags = arg->ui & TAGMASK;
        focus(NULL);
        arrange(selmon);
    }
}

void
tagmon(const Arg *arg) {
    if (!selmon->sel || !mons->next)
        return;
    sendmon(selmon->sel, dirtomon(arg->i));
}

int
textnw(const char *text, unsigned int len) {
    XRectangle r;

    if (dc.font.set) {
        XmbTextExtents(dc.font.set, text, len, NULL, &r);
        return r.width;
    }
    return XTextWidth(dc.font.xfont, text, len);
}

void
tile(_Monitor *m) {
    unsigned int i, n, h, mw, my, ty;
    Client *c;

    for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
    if (n == 0)
        return;

    if (n > (unsigned int) m->nmaster)
        mw = m->nmaster ? m->ww * m->mfact : 0;
    else
        mw = m->ww;
    for (i = my = ty = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
        if (i < (unsigned int) m->nmaster) {
            h = (m->wh - my) / (MIN(n, (unsigned int) m->nmaster) - i);
            resize(c, m->wx, m->wy + my, mw - (2*c->bw), h - (2*c->bw), False);
            my += HEIGHT(c);
        }
        else {
            h = (m->wh - ty) / (n - i);
            resize(c, m->wx + mw, m->wy + ty, m->ww - mw - (2*c->bw), h - (2*c->bw), False);
            ty += HEIGHT(c);
        }
}

void
togglebar(const Arg *arg) {
    selmon->showbar = !selmon->showbar;
    updatebarpos(selmon);
    XMoveResizeWindow(globals->dpy, selmon->barwin, selmon->wx, selmon->by, selmon->ww, bh);
    arrange(selmon);
}

void
togglefloating(const Arg *arg) {
    if (!selmon->sel)
        return;
    selmon->sel->isfloating = !selmon->sel->isfloating || selmon->sel->isfixed;
    if (selmon->sel->isfloating)
        resize(selmon->sel, selmon->sel->x, selmon->sel->y,
               selmon->sel->w, selmon->sel->h, False);
    arrange(selmon);
}

void
toggletag(const Arg *arg) {
    unsigned int newtags;

    if (!selmon->sel)
        return;
    newtags = selmon->sel->tags ^ (arg->ui & TAGMASK);
    if (newtags) {
        selmon->sel->tags = newtags;
        focus(NULL);
        arrange(selmon);
    }
}

void
toggleview(const Arg *arg) {
    unsigned int newtagset = selmon->tagset[selmon->seltags] ^ (arg->ui & TAGMASK);

    if (newtagset) {
        selmon->tagset[selmon->seltags] = newtagset;
        focus(NULL);
        arrange(selmon);
    }
}

void
unfocus(Client *c, Bool setfocus) {
    if (!c)
        return;
    grabbuttons(c, False);
    XSetWindowBorder(globals->dpy, c->win, dc.norm[ColBorder]);
    if (setfocus)
        XSetInputFocus(globals->dpy, root, RevertToPointerRoot, CurrentTime);
}

void
unmanage(Client *c, Bool destroyed) {
    _Monitor *m = c->mon;
    XWindowChanges wc;

    /* The server grab construct avoids race conditions. */
    detach(c);
    detachstack(c);
    if (!destroyed) {
        wc.border_width = c->oldbw;
        XGrabServer(globals->dpy);
        XSetErrorHandler(xerrordummy);
        XConfigureWindow(globals->dpy, c->win, CWBorderWidth, &wc); /* restore border */
        XUngrabButton(globals->dpy, AnyButton, AnyModifier, c->win);
        setclientstate(c, WithdrawnState);
        XSync(globals->dpy, False);
        XSetErrorHandler(xerror);
        XUngrabServer(globals->dpy);
    }
    free(c);
    focus(NULL);
    arrange(m);
}

void
unmapnotify(XEvent *e) {
    Client *c;
    XUnmapEvent *ev = &e->xunmap;

    if ((c = wintoclient(ev->window))) {
        if (ev->send_event)
            setclientstate(c, WithdrawnState);
        else
            unmanage(c, False);
    }
}

void
updatebars(void) {
    _Monitor *m;
    XSetWindowAttributes wa = {
        .override_redirect = True,
        .background_pixmap = ParentRelative,
        .event_mask = ButtonPressMask|ExposureMask
    };
    for (m = mons; m; m = m->next) {
        m->barwin = XCreateWindow(globals->dpy, root, m->wx, m->by, m->ww, bh, 0, DefaultDepth(globals->dpy, screen),
                                  CopyFromParent, DefaultVisual(globals->dpy, screen),
                                  CWOverrideRedirect|CWBackPixmap|CWEventMask, &wa);
        XDefineCursor(globals->dpy, m->barwin, cursor[CurNormal]);
        XMapRaised(globals->dpy, m->barwin);
    }
}

void
updatebarpos(_Monitor *m) {
    m->wy = m->my;
    m->wh = m->mh;
    if (m->showbar) {
        m->wh -= bh;
        m->by = m->topbar ? m->wy : m->wy + m->wh;
        m->wy = m->topbar ? m->wy + bh : m->wy;
    }
    else
        m->by = -bh;
}

Bool
updategeom(void) {
    Bool dirty = False;

    if (is_xinerama_active(globals->dpy)) {
        int i, j, n, nn;
        Client *c;
        _Monitor *m;
        XineramaScreenInfo *info = get_xinerama_screens(globals->dpy, &nn);
        XineramaScreenInfo *unique = NULL;

        for (n = 0, m = mons; m; m = m->next, n++);
        /* only consider unique geometries as separate screens */
        if (!(unique = (XineramaScreenInfo *)malloc(sizeof(XineramaScreenInfo) * nn)))
            die("fatal: could not malloc() %u bytes\n", sizeof(XineramaScreenInfo) * nn);
        for (i = 0, j = 0; i < nn; i++)
            if (isuniquegeom(unique, j, &info[i]))
                memcpy(&unique[j++], &info[i], sizeof(XineramaScreenInfo));
        XFree(info);
        nn = j;
        if (n <= nn) {
            for (i = 0; i < (nn - n); i++) { /* new monitors available */
                for (m = mons; m && m->next; m = m->next);
                if (m)
                    m->next = createmon();
                else
                    mons = createmon();
            }
            for (i = 0, m = mons; i < nn && m; m = m->next, i++)
                if (i >= n
                || (unique[i].x_org != m->mx || unique[i].y_org != m->my
                    || unique[i].width != m->mw || unique[i].height != m->mh))
                {
                    dirty = True;
                    m->num = i;
                    m->mx = m->wx = unique[i].x_org;
                    m->my = m->wy = unique[i].y_org;
                    m->mw = m->ww = unique[i].width;
                    m->mh = m->wh = unique[i].height;
                    updatebarpos(m);
                }
        }
        else { /* less monitors available nn < n */
            for (i = nn; i < n; i++) {
                for (m = mons; m && m->next; m = m->next);
                while (m->clients) {
                    dirty = True;
                    c = m->clients;
                    m->clients = c->next;
                    detachstack(c);
                    c->mon = mons;
                    attach(c);
                    attachstack(c);
                }
                if (m == selmon)
                    selmon = mons;
                cleanupmon(m);
            }
        }
        free(unique);
    }
    else
    /* default monitor setup */
    {
        if (!mons)
            mons = createmon();
        if (mons->mw != sw || mons->mh != sh) {
            dirty = True;
            mons->mw = mons->ww = sw;
            mons->mh = mons->wh = sh;
            updatebarpos(mons);
        }
    }
    if (dirty) {
        selmon = mons;
        selmon = wintomon(root);
    }
    return dirty;
}

void
updatenumlockmask(void) {
    unsigned int i, j;
    XModifierKeymap *modmap;

    numlockmask = 0;
    modmap = XGetModifierMapping(globals->dpy);
    for (i = 0; i < 8; i++)
        for (j = 0; j < (unsigned int) modmap->max_keypermod; j++)
            if (modmap->modifiermap[i * modmap->max_keypermod + j]
               == XKeysymToKeycode(globals->dpy, XK_Num_Lock))
                numlockmask = (1 << i);
    XFreeModifiermap(modmap);
}

void
updatesizehints(Client *c) {
    long msize;
    XSizeHints size;

    if (!XGetWMNormalHints(globals->dpy, c->win, &size, &msize))
        /* size is uninitialized, ensure that size.flags aren't used */
        size.flags = PSize;
    if (size.flags & PBaseSize) {
        c->basew = size.base_width;
        c->baseh = size.base_height;
    }
    else if (size.flags & PMinSize) {
        c->basew = size.min_width;
        c->baseh = size.min_height;
    }
    else
        c->basew = c->baseh = 0;
    if (size.flags & PResizeInc) {
        c->incw = size.width_inc;
        c->inch = size.height_inc;
    }
    else
        c->incw = c->inch = 0;
    if (size.flags & PMaxSize) {
        c->maxw = size.max_width;
        c->maxh = size.max_height;
    }
    else
        c->maxw = c->maxh = 0;
    if (size.flags & PMinSize) {
        c->minw = size.min_width;
        c->minh = size.min_height;
    }
    else if (size.flags & PBaseSize) {
        c->minw = size.base_width;
        c->minh = size.base_height;
    }
    else
        c->minw = c->minh = 0;
    if (size.flags & PAspect) {
        c->mina = (float)size.min_aspect.y / size.min_aspect.x;
        c->maxa = (float)size.max_aspect.x / size.max_aspect.y;
    }
    else
        c->maxa = c->mina = 0.0;
    c->isfixed = (c->maxw && c->minw && c->maxh && c->minh
                 && c->maxw == c->minw && c->maxh == c->minh);
}

void
updatetitle(Client *c) {
    if (!gettextprop(c->win, netatom[NetWMName], c->name, sizeof c->name))
        gettextprop(c->win, XA_WM_NAME, c->name, sizeof c->name);
    if (c->name[0] == '\0') /* hack to mark broken clients */
        strcpy(c->name, broken);
}

void
updatestatus(void) {
    gettextprop(root, XA_WM_NAME, stext, sizeof(stext));
    drawbar(selmon);
}

void
updatewindowtype(Client *c) {
    Atom state = getatomprop(c, netatom[NetWMState]);
    Atom wtype = getatomprop(c, netatom[NetWMWindowType]);

    if (state == netatom[NetWMFullscreen])
        setfullscreen(c, True);

    if (wtype == netatom[NetWMWindowTypeDialog])
        c->isfloating = True;
}

void
updatewmhints(Client *c) {
    XWMHints *wmh;

    if ((wmh = XGetWMHints(globals->dpy, c->win))) {
        if (c == selmon->sel && wmh->flags & XUrgencyHint) {
            wmh->flags &= ~XUrgencyHint;
            XSetWMHints(globals->dpy, c->win, wmh);
        }
        else
            c->isurgent = (wmh->flags & XUrgencyHint) ? True : False;
        if (wmh->flags & InputHint)
            c->neverfocus = !wmh->input;
        else
            c->neverfocus = False;
        XFree(wmh);
    }
}

void
view(const Arg *arg) {
    if ((arg->ui & TAGMASK) == selmon->tagset[selmon->seltags])
        return;
    selmon->seltags ^= 1; /* toggle sel tagset */
    if (arg->ui & TAGMASK)
        selmon->tagset[selmon->seltags] = arg->ui & TAGMASK;
    focus(NULL);
    arrange(selmon);
}

void
warp(const Client *c) {
    Window dummy;
    int x, y, di;
    unsigned int dui;

    if (!c) {
        XWarpPointer(globals->dpy, None, root, 0, 0, 0, 0, selmon->wx + selmon->ww / 2, selmon->wy + selmon->wh/2);
        return;
    }

    XQueryPointer(globals->dpy, root, &dummy, &dummy, &x, &y, &di, &di, &dui);

    if ((x > c->x && y > c->y && x < c->x + c->w && y < c->y + c->h) ||
        (y > c->mon->by && y < c->mon->by + bh))
        return;

    XWarpPointer(globals->dpy, None, c->win, 0, 0, 0, 0, c->w / 2, c->h / 2);
}

Client *
wintoclient(Window w) {
    Client *c;
    _Monitor *m;

    for (m = mons; m; m = m->next)
        for (c = m->clients; c; c = c->next)
            if (c->win == w)
                return c;
    return NULL;
}

_Monitor *
wintomon(Window w) {
    int x, y;
    Client *c;
    _Monitor *m;

    if (w == root && getrootptr(&x, &y))
        return recttomon(x, y, 1, 1);
    for (m = mons; m; m = m->next)
        if (w == m->barwin)
            return m;
    if ((c = wintoclient(w)))
        return c->mon;
    return selmon;
}

/* There's no way to check accesses to destroyed windows, thus those cases are
 * ignored (especially on UnmapNotify's).  Other types of errors call Xlibs
 * default error handler, which may call exit.  */
int
xerror(Display *dpy, XErrorEvent *ee) {
    if (ee->error_code == BadWindow
    || (ee->request_code == X_SetInputFocus && ee->error_code == BadMatch)
    || (ee->request_code == X_PolyText8 && ee->error_code == BadDrawable)
    || (ee->request_code == X_PolyFillRectangle && ee->error_code == BadDrawable)
    || (ee->request_code == X_PolySegment && ee->error_code == BadDrawable)
    || (ee->request_code == X_ConfigureWindow && ee->error_code == BadMatch)
    || (ee->request_code == X_GrabButton && ee->error_code == BadAccess)
    || (ee->request_code == X_GrabKey && ee->error_code == BadAccess)
    || (ee->request_code == X_CopyArea && ee->error_code == BadDrawable))
        return 0;
    fprintf(stderr, "fatal error: request code=%d, error code=%d\n",
            ee->request_code, ee->error_code);
    return xerrorxlib(globals->dpy, ee); /* may call exit */
}

int
xerrordummy(Display *dpy, XErrorEvent *ee) {
    return 0;
}

void
zoom(const Arg *arg) {
    Client *c = selmon->sel;

    if (!selmon->lt[selmon->sellt]->arrange
    || (selmon->sel && selmon->sel->isfloating))
        return;
    if (c == nexttiled(selmon->clients))
        if (!c || !(c = nexttiled(c->next)))
            return;
    pop(c);
}

void _init() {
    if (!setlocale(LC_CTYPE, "") || !XSupportsLocale())
        fputs("warning: no locale support\n", stderr);
    setup();
    scan();
}

