//
// Implemented using GPT-5.2
//

#include <stdio.h>
#include <stdlib.h>
#include "calendar.h"

/* Linked list node */
typedef struct ev_node {
    struct ev_notice ev;
    struct ev_node *next;
} ev_node;

static ev_node *calendar = NULL;

/* Initialize calendar */
void cal_init(void) {
    calendar = NULL;
}

/* Insert event sorted by time, then priority */
void cal_insert(event_fptr ev, int priority, double atime) {
    ev_node *n = malloc(sizeof(ev_node));
    if (!n) {
        perror("malloc");
        exit(1);
    }

    n->ev.evptr = ev;
    n->ev.priority = priority;   /* ← FIXED */
    n->ev.atime = atime;
    n->next = NULL;

    /* Empty calendar or insert at front */
    if (!calendar ||
        atime < calendar->ev.atime ||
        (atime == calendar->ev.atime &&
         priority < calendar->ev.priority)) {

        n->next = calendar;
        calendar = n;
        return;
    }

    /* Find insertion point */
    ev_node *p = calendar;
    while (p->next &&
          (p->next->ev.atime < atime ||
          (p->next->ev.atime == atime &&
           p->next->ev.priority <= priority))) {
        p = p->next;
    }

    n->next = p->next;
    p->next = n;
}

/* Check if calendar is empty */
int cal_empty(void) {
    return calendar == NULL;
}

/* Get first event (does not remove it) */
struct ev_notice cal_get_first(void) {
    return calendar->ev;
}

/* Remove first event */
void cal_pop(void) {
    if (!calendar) return;
    ev_node *tmp = calendar;
    calendar = calendar->next;
    free(tmp);
}

/* END event handler */
void END(void) {
    printf("Simulation finished.\n");
    exit(0);
}
