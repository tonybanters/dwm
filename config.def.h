/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
/*defaults= 20, 10, 10, 30 */
static const unsigned int gappih    = 3;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 3;       /* vert inner gap between windows */
static const unsigned int gappoh    = 3;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 3;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[] = {
    "JetBrainsMono Nerd Font Mono:style=Bold:size=16",
};
static const char dmenufont[]       = "JetBrainsMono Nerd Font Mono:style=Bold:size=16";
static const char col_gray1[]       = "#000000";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#4d6a8e";

/* TokyoNight colors */
static const char col_bg[]     = "#1a1b26";  // background
static const char col_fg[]     = "#a9b1d6";  // foreground
static const char col_blk[]    = "#32344a";  // black (normal)
static const char col_red[]    = "#f7768e";  // red
static const char col_grn[]    = "#9ece6a";  // green
static const char col_ylw[]    = "#e0af68";  // yellow
static const char col_blu[]    = "#7aa2f7";  // blue
static const char col_mag[]    = "#ad8ee6";  // magenta
static const char col_cyn[]    = "#0db9d7";  // cyan (highlight)
static const char col_brblk[]  = "#444b6a";  // bright black

static const char *colors[][3] = {
    /*               fg       bg       border */
    [SchemeNorm] = { col_fg,  col_bg,   col_brblk },
    [SchemeSel]  = { col_cyn, col_bg,   col_mag   },
};

/* tagging */
/* static const char *tags[] = { "", "󰊯", "", "", "󰙯", "󱇤", "", "󱘶", "󰧮" }; */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance    title       tags mask     isfloating   monitor */
	{ "Gimp",            NULL,       NULL,       0,            1,           -1 },
    { "Google-chrome",   NULL,       NULL,       1 << 1,       0,           -1 },
    { "Brave-browser",   NULL,       NULL,       1 << 1,       0,           -1 },
	{ "firefox",         NULL,       NULL,       1 << 2,       0,           -1 },
	{ "Slack",           NULL,       NULL,       1 << 3,       0,           -1 },
	{ "discord",         NULL,       NULL,       1 << 4,       0,           -1 },
	{ "kdenlive",        NULL,       NULL,       1 << 7,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
/*#include "fibonacci.c"*/

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "󰝘",        tile },    /* first entry is default */
	{ "",        NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
 	{ "",        spiral },
 	{ "[\\]",     dwindle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG)                                                                                               \
       &((Keychord){1, {{MODKEY, KEY}},                                        view,           {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ControlMask, KEY}},                            toggleview,     {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ShiftMask, KEY}},                              tag,            {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ControlMask|ShiftMask, KEY}},                  toggletag,      {.ui = 1 << TAG} }),

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *firefoxcmd[]  = { "firefox-bin", NULL };
static const char *slock[]    = { "slock", NULL };
static const char *screenshotcmd[] = { "/bin/sh", "-c", "maim -s | xclip -selection clipboard -t image/png", NULL };
static const char *rofi[]  = { "rofi", "-show", "drun", "-theme", "~/.config/rofi/config.rasi", NULL };
static const char *emacsclient[]  = { "emacsclient", "-c", "-a", "", NULL };

static Keychord *keychords[] = {
    /* key count, modifier/key sequence,            function,        argument */

    &((Keychord){1, {{MODKEY, XK_r}},               spawn,          {.v = dmenucmd } }),
    &((Keychord){1, {{MODKEY, XK_Return}},          spawn,          {.v = termcmd } }),
    &((Keychord){1, {{MODKEY, XK_l}},               spawn,          {.v = slock } }),
    &((Keychord){1, {{ControlMask, XK_Print}},      spawn,          {.v = screenshotcmd } }),
    &((Keychord){1, {{MODKEY, XK_d}},               spawn,          {.v = rofi } }),

    &((Keychord){1, {{MODKEY, XK_b}},               togglebar,      {0} }),
    &((Keychord){1, {{MODKEY, XK_j}},               focusstack,     {.i = +1 } }),
    &((Keychord){1, {{MODKEY, XK_k}},               focusstack,     {.i = -1 } }),
    &((Keychord){1, {{MODKEY, XK_i}},               incnmaster,     {.i = +1 } }),
    &((Keychord){1, {{MODKEY, XK_p}},               incnmaster,     {.i = -1 } }),
    &((Keychord){1, {{MODKEY, XK_g}},               setmfact,       {.f = -0.05} }),
    &((Keychord){1, {{MODKEY, XK_h}},               setmfact,       {.f = +0.05} }),

    &((Keychord){1, {{MODKEY, XK_z}},               incrgaps,       {.i = +3 } }),
    &((Keychord){1, {{MODKEY, XK_x}},               incrgaps,       {.i = -3 } }),
    &((Keychord){1, {{MODKEY, XK_a}},               togglegaps,     {0} }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_a}},     defaultgaps,    {0} }),

    &((Keychord){1, {{MODKEY, XK_Tab}},             view,           {0} }),
    &((Keychord){1, {{MODKEY, XK_q}},               killclient,     {0} }),

    &((Keychord){1, {{MODKEY|ShiftMask, XK_t}},     setlayout,      {.v = &layouts[0]} }),
    // &((Keychord){1, {{MODKEY, XK_f}},               setlayout,      {.v = &layouts[1]} }),
    &((Keychord){1, {{MODKEY, XK_m}},               setlayout,      {.v = &layouts[2]} }),
    &((Keychord){1, {{MODKEY, XK_c}},               setlayout,      {.v = &layouts[3]} }),
    &((Keychord){1, {{MODKEY, XK_o}},               setlayout,      {.v = &layouts[4]} }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_Return}},setlayout,      {0} }),

    &((Keychord){1, {{MODKEY|ShiftMask, XK_f}},     fullscreen,     {0} }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_space}}, togglefloating, {0} }),

    &((Keychord){1, {{MODKEY, XK_0}},               view,           {.ui = ~0 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_0}},     tag,            {.ui = ~0 } }),

    &((Keychord){1, {{MODKEY, XK_comma}},           focusmon,       {.i = -1 } }),
    &((Keychord){1, {{MODKEY, XK_period}},          focusmon,       {.i = +1 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_comma}}, tagmon,         {.i = -1 } }),
    &((Keychord){1, {{MODKEY|ShiftMask, XK_period}},tagmon,         {.i = +1 } }),
    &((Keychord){2, {{MODKEY, XK_space}, {0, XK_f}}, spawn, {.v = (const char*[]){"firefox", NULL}} }),

    // Keychords for navigating to tags (small hands/emacs pinky)
    &((Keychord){2, {{MODKEY, XK_space}, {0, XK_1}}, view, {.ui = 1 << 0} }),
    &((Keychord){2, {{MODKEY, XK_space}, {0, XK_2}}, view, {.ui = 1 << 1} }),
    &((Keychord){2, {{MODKEY, XK_space}, {0, XK_3}}, view, {.ui = 1 << 2} }),
    &((Keychord){2, {{MODKEY, XK_space}, {0, XK_4}}, view, {.ui = 1 << 3} }),
    &((Keychord){2, {{MODKEY, XK_space}, {0, XK_5}}, view, {.ui = 1 << 4} }),
    &((Keychord){2, {{MODKEY, XK_space}, {0, XK_6}}, view, {.ui = 1 << 5} }),
    &((Keychord){2, {{MODKEY, XK_space}, {0, XK_7}}, view, {.ui = 1 << 6} }),
    &((Keychord){2, {{MODKEY, XK_space}, {0, XK_8}}, view, {.ui = 1 << 7} }),
    &((Keychord){2, {{MODKEY, XK_space}, {0, XK_9}}, view, {.ui = 1 << 8} }),

    // Dmenu Scripts
    &((Keychord){2, {{MODKEY, XK_f}, {0, XK_f}}, spawn, SHCMD("$HOME/repos/dmenu-scripts/repos-dmenu.sh")}),
    &((Keychord){2, {{MODKEY, XK_f}, {0, XK_o}}, spawn, SHCMD("$HOME/repos/dmenu-scripts/tmux-dmenu.sh")}),
    &((Keychord){2, {{MODKEY, XK_f}, {0, XK_b}}, spawn, SHCMD("$HOME/repos/dmenu-scripts/bookmarks-dmenu.sh")}),
    
    // Emacs Scripts
    // &((Keychord){2, {{MODKEY, XK_e}, {0, XK_t}}, spawn, SHCMD("$HOME/scripts/tmux-dmenu.sh")}),
    // &((Keychord){2, {{MODKEY, XK_e}, {0, XK_a}}, spawn, SHCMD("$HOME/scripts/tmux-dmenu.sh")}),
    // &((Keychord){2, {{MODKEY, XK_e}, {0, XK_t}}, spawn, SHCMD("$HOME/scripts/tmux-dmenu.sh")}),

    // TAGKEYS
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)

    &((Keychord){1, {{MODKEY|ShiftMask,   XK_q}},   quit,           {0} }),
    &((Keychord){1, {{MODKEY|ControlMask, XK_r}},   quit,           {1} }),

    &((Keychord){1, {{0, XF86XK_AudioRaiseVolume}}, spawn, {.v = (const char*[]){"pactl", "set-sink-volume", "@DEFAULT_SINK@", "+3%", NULL} } }),
    &((Keychord){1, {{0, XF86XK_AudioLowerVolume}}, spawn, {.v = (const char*[]){"pactl", "set-sink-volume", "@DEFAULT_SINK@", "-3%", NULL} } }),
};

// static const Key keys[] = {
// 	/* modifier                     key        function        argument */
// 	{ MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
// 	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
// 	{ MODKEY,                       XK_l,      spawn,          {.v = slock } },
// 	{ ControlMask,                  XK_Print,  spawn,          {.v = screenshotcmd } },
// 	{ MODKEY,                       XK_d,      spawn,          {.v = rofi } },
// 	{ MODKEY,                       XK_b,      togglebar,      {0} },
// 	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
// 	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
// 	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
// 	{ MODKEY,                       XK_p,      incnmaster,     {.i = -1 } },
// 	{ MODKEY,                       XK_g,      setmfact,       {.f = -0.05} },
// 	{ MODKEY,                       XK_h,      setmfact,       {.f = +0.05} },
// 	{ MODKEY,                       XK_z,      incrgaps,       {.i = +3 } },
// 	{ MODKEY,                       XK_x,      incrgaps,       {.i = -3 } },
// 	{ MODKEY,                       XK_a,      togglegaps,     {0} },
// 	{ MODKEY|ShiftMask,             XK_a,      defaultgaps,    {0} },
// 	{ MODKEY,                       XK_Tab,    view,           {0} },
// 	{ MODKEY,                       XK_q,      killclient,     {0} },
// 	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
// 	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
// 	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
// 	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[3]} },
// 	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
// 	{ MODKEY|ShiftMask,             XK_Return, setlayout,      {0} },
// 	{ MODKEY|ShiftMask,             XK_f,      fullscreen,     {0} },
// 	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
// 	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
// 	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
// 	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
// 	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
// 	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
// 	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
// 	TAGKEYS(                        XK_1,                      0)
// 	TAGKEYS(                        XK_2,                      1)
// 	TAGKEYS(                        XK_3,                      2)
// 	TAGKEYS(                        XK_4,                      3)
// 	TAGKEYS(                        XK_5,                      4)
// 	TAGKEYS(                        XK_6,                      5)
// 	TAGKEYS(                        XK_7,                      6)
// 	TAGKEYS(                        XK_8,                      7)
// 	TAGKEYS(                        XK_9,                      8)
// 	// { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
// 	{ MODKEY|ControlMask,           XK_r,         quit,           {1} }, 
//     { 0, XF86XK_AudioRaiseVolume, spawn, {.v = (const char*[]){"pactl", "set-sink-volume", "@DEFAULT_SINK@", "+3%", NULL} } },
//     { 0, XF86XK_AudioLowerVolume, spawn, {.v = (const char*[]){"pactl", "set-sink-volume", "@DEFAULT_SINK@", "-3%", NULL} } },
// };

/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

