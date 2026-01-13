//
// Implemented using GPT-5.2
//

#ifndef CALENDAR_H
#define CALENDAR_H

typedef void (*event_fptr)(void);

struct ev_notice {
    double atime;        // event time
    int priority;        // lower = higher priority
    event_fptr evptr;    // event function
};

void cal_init(void);
void cal_insert(event_fptr ev, int priority, double atime);
int  cal_empty(void);
struct ev_notice cal_get_first(void);
void cal_pop(void);

/* END event */
void END(void);

#endif
