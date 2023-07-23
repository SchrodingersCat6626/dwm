/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#646C74"; /* orig: "#005577"; I didn't change name so that i wouldn't have to change too much more code. this adds the grey to status bar..etc. */
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
/* #646C74 */
/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5","6" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "librewolf", NULL,     NULL,         1 << 8,    0,        0,           -1,        -1 },
	{ "alacritty",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "alacritty-256color", NULL, NULL,	       0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.60; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

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

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
/* other keys */
#include <X11/XF86keysym.h> 

/*defining various function keys */
#define XF86XK_AudioLowerVolume	 0x1008FF11   /* Volume control down        */
#define XF86XK_AudioMute	     0x1008FF12   /* Mute sound from the system */
#define XF86XK_AudioRaiseVolume	 0x1008FF13   /* Volume control up          */
#define XF86AudioMicMute 	     0x1008ffb2  /* microphone control */
#define XF86WLAN                 0x1008ff95 /* wlan toggle (F8) */
#define Print                    0xff61 /*prtsc key */ 
#define XF86Favorites            0x1008ff30 /* favourite function key (F12) */
#define XF86Tools                0x1008ff81 /* settings function key */
#define XF86Display              0x1008ff59 /* display function key */


/* commands */
/* Note: since arrays will be passed into execvp, I cannot use path variables like $HOME or operators like & or &&. In the cases where those are required, it is better to use the SHCMD macro defined earlier in this file rather than a pointer to array. To keep it clean it is better to define the command as string literals and pass them into SHCMD in the keybinding section.  */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
/* custom commands; these are all executed in SHCMD() function seen above */
static const char *browser[] = { "librewolf", NULL };
/* coding stuff */
static const char *pythonshell[] = { "alacritty", "-e", "python3", NULL };
static const char *Rshell[] = { "alacritty", "-e", "R", NULL };
static const char *Rstudio[] = { "rstudio", NULL };
static const char *slockonly[] = { "slock", NULL };
static const char suspend[] = "slock & systemctl suspend";
static const char volumeup[] = "amixer sset Master 5%+ && $HOME/src/dwm/scripts/./volumelevels.sh";
static const char volumedown[] = "amixer sset Master 5%- && $HOME/src/dwm/scripts/./volumelevels.sh";  
static const char togglemutecmd[] = "amixer sset Master toggle && $HOME/src/dwm/scripts/./volumelevels.sh"; 
static const char *togglemic[] = { "amixer", "sset", "Capture", "toggle", NULL };
/* mail */
static const char mutt[]  = "alacritty -e neomutt & mbsync -a";
/* file manager */
/* static const char *fileman[] = { "alacritty", "-e", "ranger",  NULL }; */
static const char *fileman[] = { "pcmanfm",  NULL };
/* text editing */
static const char *vim[] = { "alacritty", "-e", "nvim", NULL };
/* bluetooth is controlled at the hardware level by bluetooth function key. This runs a script which tells me bluetooth status everytime it is pressed */
static const char bltstatus[] = "$HOME/src/dwm/scripts/./bluetoothcontrol.sh";
/* prints wifi status in status bar when wifi switch is pressed (f8) */
static const char wifistatus[] = "$HOME/src/dwm/scripts/./wifistatus.sh";
/* brightness control */
static const char light_up[] = "/usr/bin/brightnessctl s 5%+ && $HOME/src/dwm/scripts/./brightnessstatus.sh";
static const char light_down[] = "/usr/bin/brightnessctl s 5%- && $HOME/src/dwm/scripts/./brightnessstatus.sh";
/* taking screenshot with flameshot */
static const char *screenshot[] = { "flameshot", "gui", NULL };
static const char fullscreenshot[] = "$HOME/src/dwm/scripts/./fullscreenflameshot.sh"; /* fullscreen screenshot saved to ~/Pictures/screenshots/ */
/* setting power profile and displaying it in status bar */
static const char togglepower[] = "$HOME/src/dwm/scripts/./powerprofilecontrol.sh";
/* opening amixer with tool button (F9) */
static const char *amixer[] = { "alacritty", "-e", "/usr/bin/alsamixer", NULL };
/*opening htop */
static const char *htop[] = { "alacritty", "-e", "htop", NULL };
/* shortcut to opening calendar application */
static const char calcurse[] = "alacritty -e calcurse";
/* for opening signal messaging application */
static const char *signalapp[] = { "signal-desktop", NULL };


/*including function from shiftview.c which allows cycling through tags */
#include "shiftview.c"


static const Key keys[] = {
	/* modifier                     key        function        argument */
	/* { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } }, */
	{ MODKEY,                       XK_p,      spawn,          SHCMD( "rofi -theme dmenu -show run" ) },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_w,      spawn,          {.v = browser } },
	{ MODKEY|ShiftMask,             XK_r, 	   spawn,          {.v = Rstudio } },
	{ MODKEY,                       XK_r, 	   spawn,          {.v = Rshell } },
	{ MODKEY|ShiftMask,             XK_p, 	   spawn,          {.v = pythonshell } },
    { MODKEY,                       XK_v, 	   spawn,          {.v = vim } },
	{ MODKEY|ShiftMask,             XK_s, 	   spawn,          SHCMD( suspend ) },
	{ MODKEY,                       XK_s, 	   spawn,          { .v = signalapp } },
	{ MODKEY|ShiftMask,             XK_f, 	   spawn,          { .v = fileman } },
	{ MODKEY|ShiftMask,        		XK_l, 	   spawn,          {.v = slockonly } },
	{ MODKEY|ShiftMask,	            XK_m, 	   spawn,          SHCMD( mutt ) },
    { MODKEY|ShiftMask,             XK_h,      spawn,          {.v = htop } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
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
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ 0,                            XF86XK_AudioMute,         spawn, SHCMD( togglemutecmd ) },
    { 0,                            XF86XK_AudioRaiseVolume,  spawn, SHCMD( volumeup ) },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn, SHCMD( volumedown ) },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn, { .v = volumedown } },
    { 0,		                    XF86AudioMicMute, 	      spawn, { .v = togglemic  } },
	{ 0,		                    0x1008ff94,               spawn, SHCMD( bltstatus  ) },
	{ 0,		                    XF86WLAN,                 spawn, SHCMD( wifistatus ) },
    { 0,				            XF86XK_MonBrightnessUp,	  spawn, SHCMD( light_up ) },
	{ 0,				            XF86XK_MonBrightnessDown, spawn, SHCMD( light_down ) },
    { 0,				            Print,                    spawn, { .v = screenshot } },    
    { MODKEY,				        Print,                    spawn, SHCMD( fullscreenshot ) },
    { MODKEY,				        XK_c,                     spawn, SHCMD( calcurse ) },
    { 0,				            XF86Favorites,            spawn, SHCMD( togglepower ) },
    { 0,                            XF86Tools,                spawn, { .v = amixer } },
    { 0,                            XF86Display,              spawn, SHCMD( "xrandr --auto" ) },	
    { MODKEY|ShiftMask,             XK_j,       shiftview,  	{ .i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,       shiftview,      { .i = -1 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};





	






