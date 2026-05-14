#ifndef _XKBRULES_H_
#define	_XKBRULES_H_ 1

/************************************************************
 Copyright (c) 1996 by Silicon Graphics Computer Systems, Inc.

 Permission to use, copy, modify, and distribute this
 software and its documentation for any purpose and without
 fee is hereby granted, provided that the above copyright
 notice appear in all copies and that both that copyright
 notice and this permission notice appear in supporting
 documentation, and that the name of Silicon Graphics not be
 used in advertising or publicity pertaining to distribution
 of the software without specific prior written permission.
 Silicon Graphics makes no representation about the suitability
 of this software for any purpose. It is provided "as is"
 without any express or implied warranty.

 SILICON GRAPHICS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
 SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SILICON
 GRAPHICS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
 THE USE OR PERFORMANCE OF THIS SOFTWARE.

 ********************************************************/

/***====================================================================***/

/**
 * @brief xkb rules definitions
 *
 * The structure is used to query xkb rules definitions used to construct
 * keyboard mapping. Those defs could be either set by the xorg.conf (with
 * XkbModel, XkbLayout, XkbVariant and XkbOptions) or by client utility
 * like setxkbmap.
 *
 * @see XkbRF_GetNamesProp, XkbRF_SetNamesProp, XkbRF_FreeVarDefs,
 *      man setxkbmap
 */
typedef struct _XkbRF_VarDefs {
	char *			model;     /**< keyboard model e.g. "pc104" */
	char *			layout;    /**< layout list e.g. "us,ru" */
	char *			variant;   /**< layout variants list e.g. "dvorak," */
	char *			options;   /**< xkb options e.g "grp:toggle,misc:typo" */
	unsigned short		sz_extra;  /**< unused */
	unsigned short		num_extra; /**< unused */
	char *			extra_names;  /**< unused */
	char **			extra_values; /**< unused */
} XkbRF_VarDefsRec,*XkbRF_VarDefsPtr;

typedef struct _XkbRF_VarDesc {
	char *			name;
	char *			desc;
} XkbRF_VarDescRec, *XkbRF_VarDescPtr;

typedef struct _XkbRF_DescribeVars {
	int			sz_desc;
	int			num_desc;
	XkbRF_VarDescPtr	desc;
} XkbRF_DescribeVarsRec,*XkbRF_DescribeVarsPtr;

typedef struct _XkbRF_Rule {
	int			number;
        int			layout_num;
        int			variant_num;
	char *			model;
	char *			layout;
	char *			variant;
	char *			option;
	/* yields */
	char *			keycodes;
	char *			symbols;
	char *			types;
	char *			compat;
	char *			geometry;
	char *			keymap;
	unsigned		flags;
} XkbRF_RuleRec,*XkbRF_RulePtr;

typedef struct _XkbRF_Group {
	int			number;
	char *			name;
	char *			words;
} XkbRF_GroupRec, *XkbRF_GroupPtr;

#define	XkbRF_PendingMatch	(1L<<1)
#define	XkbRF_Option		(1L<<2)
#define	XkbRF_Append		(1L<<3)
#define	XkbRF_Normal		(1L<<4)
#define	XkbRF_Invalid		(1L<<5)

typedef struct _XkbRF_Rules {
	XkbRF_DescribeVarsRec	models;
	XkbRF_DescribeVarsRec	layouts;
	XkbRF_DescribeVarsRec	variants;
	XkbRF_DescribeVarsRec	options;
	unsigned short		sz_extra;
	unsigned short		num_extra;
	char **			extra_names;
	XkbRF_DescribeVarsPtr	extra;

	unsigned short		sz_rules;
	unsigned short		num_rules;
	XkbRF_RulePtr		rules;
	unsigned short		sz_groups;
	unsigned short		num_groups;
        XkbRF_GroupPtr		groups;
} XkbRF_RulesRec, *XkbRF_RulesPtr;

/***====================================================================***/

_XFUNCPROTOBEGIN

extern Bool	XkbRF_GetComponents(
    XkbRF_RulesPtr		/* rules */,
    XkbRF_VarDefsPtr		/* var_defs */,
    XkbComponentNamesPtr	/* names */
);

extern XkbRF_RulePtr	XkbRF_AddRule(
    XkbRF_RulesPtr	/* rules */
);

extern XkbRF_GroupPtr XkbRF_AddGroup(XkbRF_RulesPtr  rules);

extern Bool	XkbRF_LoadRules(
    FILE *		/* file */,
    XkbRF_RulesPtr	/* rules */
);

extern Bool XkbRF_LoadRulesByName(
    char *		/* base */,
    char *		/* locale */,
    XkbRF_RulesPtr	/* rules */
);

/***====================================================================***/

extern XkbRF_VarDescPtr	XkbRF_AddVarDesc(
    XkbRF_DescribeVarsPtr	/* vars */
);

extern XkbRF_VarDescPtr	XkbRF_AddVarDescCopy(
    XkbRF_DescribeVarsPtr	/* vars */,
    XkbRF_VarDescPtr		/* copy_from */
);

extern XkbRF_DescribeVarsPtr XkbRF_AddVarToDescribe(
    XkbRF_RulesPtr		/* rules */,
    char *			/* name */
);

extern Bool	XkbRF_LoadDescriptions(
    FILE *		/* file */,
    XkbRF_RulesPtr	/* rules */
);

extern Bool XkbRF_LoadDescriptionsByName(
    char *		/* base */,
    char *		/* locale */,
    XkbRF_RulesPtr	/* rules */
);

extern XkbRF_RulesPtr XkbRF_Load(
    char *		/* base */,
    char *		/* locale */,
    Bool		/* wantDesc */,
    Bool		/* wantRules */
);

extern XkbRF_RulesPtr XkbRF_Create(
    int			/* sz_rules */,
    int			/* sz_extra */
);

/***====================================================================***/

extern void XkbRF_Free(
    XkbRF_RulesPtr	/* rules */,
    Bool		/* freeRules */
);


/***====================================================================***/

/** Name of the atom of the root window used to store/query xkb rules */
#define	_XKB_RF_NAMES_PROP_ATOM		"_XKB_RULES_NAMES"
#define	_XKB_RF_NAMES_PROP_MAXLEN	1024

/**
 * @brief Queries X server's xkb rules
 * @param[in]  dpy - X Display
 * @param[out] rules_file_rtrn
 *   if not NULL returns a pointer to the name of xkb rule file used by the
 *   X server (usually located in /usr/share/X11/xkb/rules/). The string
 *   shall be explicitly freed by the caller using free().
 * @param[out] var_defs_rtrn
 *   a pointer to a @struct XkbRF_VarDefsRec where a data queried from the
 *   server will be stored. Some data in the structure will be dynamically
 *   allocated and shall be later explicitly freed by the caller using
 *   XkbRF_FreeVarDefs().
 * @return @c True on success, @c False on failure
 *
 * Queries X server's xkb rules using the "_XKB_RULES_NAMES" atom of the
 * root window. As of 2024 (and been like that for a long time) this is not
 * officially documented in the form of an extension to X11 protocol, but
 * it's provided by the reference xorg implementation and used by several
 * utilities like setxkbmap.
 */
extern Bool XkbRF_GetNamesProp(
   Display *        /* dpy */,
   char **          /* rules_file_rtrn */,
   XkbRF_VarDefsPtr /* var_defs_rtrn */
);

/**
 * @brief Sets the X server's xkb rules property
 * @param[in] dpy        - X Display
 * @param[in] rules_file - a rules file used to compile xkb key mapping
 * @param[in] var_defs   - list of xkb rules
 * @return @c True on success, @c False on failure
 *
 * Sets X server's xkb rules property stored in the "_XKB_RULES_NAMES" atom
 * of root window.
 *
 * @note Setting this property doesn't change the layout configuration.
 *   It's only used to tell other clients which rules were used to compile
 *   the current layout configuration. In order to actually construct and
 *   set configuration based on rules @see XkbRF_GetComponents() and
 *   XkbGetKeyboardByName() (from Xkblib)
 */
extern Bool XkbRF_SetNamesProp(
   Display *        /* dpy */,
   char *           /* rules_file */,
   XkbRF_VarDefsPtr /* var_defs */
);

/**
 * @brief Destroy @struct XkbRF_VarDefsRec
 * @param[in] var_defs    - a XkbRF_VarDefsRec to clean up
 * @param[in] freeVarDefs - if @c True also free the @p var_defs itself
 *
 * Frees the memory associated with the @p var_defs. If @p free_var_defs is
 * @c True also frees the @p var_defs pointer itself.
 */
extern void XkbRF_FreeVarDefs(
   XkbRF_VarDefsPtr /* var_defs */,
   Bool             /* freeVarDefs */
);

_XFUNCPROTOEND

#endif /* _XKBRULES_H_ */
