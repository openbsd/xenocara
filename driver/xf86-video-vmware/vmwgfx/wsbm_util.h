/*
 * This file is not copyrighted.
 */

#ifndef _WSBM_UTIL_H_
#define _WSBM_UTIL_H_

#include <stddef.h>

#ifndef containerOf
#define containerOf(__item, __type, __field)				\
      ((__type *)(((char *) (__item)) - offsetof(__type, __field)))
#endif

struct _WsbmListHead
{
    struct _WsbmListHead *prev;
    struct _WsbmListHead *next;
};

#define WSBMINITLISTHEAD(__item)		       \
    do{						       \
	(__item)->prev = (__item);		       \
	(__item)->next = (__item);		       \
    } while (0)

#define WSBMLISTADD(__item, __list)		\
  do {						\
    (__item)->prev = (__list);			\
    (__item)->next = (__list)->next;		\
    (__list)->next->prev = (__item);		\
    (__list)->next = (__item);			\
  } while (0)

#define WSBMLISTADDTAIL(__item, __list)		\
  do {						\
    (__item)->next = (__list);			\
    (__item)->prev = (__list)->prev;		\
    (__list)->prev->next = (__item);		\
    (__list)->prev = (__item);			\
  } while(0)

#define WSBMLISTDEL(__item)			\
  do {						\
    (__item)->prev->next = (__item)->next;	\
    (__item)->next->prev = (__item)->prev;	\
  } while(0)

#define WSBMLISTDELINIT(__item)			\
  do {						\
    (__item)->prev->next = (__item)->next;	\
    (__item)->next->prev = (__item)->prev;	\
    (__item)->next = (__item);			\
    (__item)->prev = (__item);			\
  } while(0)

#define WSBMLISTFOREACH(__item, __list) \
    for((__item) = (__list)->next; (__item) != (__list); (__item) = (__item)->next)

#define WSBMLISTFOREACHPREV(__item, __list) \
    for((__item) = (__list)->prev; (__item) != (__list); (__item) = (__item)->prev)

#define WSBMLISTFOREACHSAFE(__item, __next, __list)		\
        for((__item) = (__list)->next, (__next) = (__item)->next;	\
	(__item) != (__list);					\
	(__item) = (__next), (__next) = (__item)->next)

#define WSBMLISTFOREACHPREVSAFE(__item, __prev, __list)		\
    for((__item) = (__list)->prev, (__prev) = (__item->prev);	\
	(__item) != (__list);					\
	(__item) = (__prev), (__prev) = (__item)->prev)

#define WSBMLISTENTRY(__item, __type, __field)			\
    containerOf(__item, __type, __field)

#define WSBMLISTEMPTY(__item)			\
    ((__item)->next == (__item))

#endif
