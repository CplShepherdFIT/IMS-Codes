#include <stdio.h>
#include "lib/calendar.h"

double in[2];
double st[] = {1.0, 1.0};
double t = 0.0;
double stepsize = 0.01;
double printstep = 0.01;
const double end = 20.0;
const unsigned N = 2;

void print_event(void);

void Dynamic() {
    in[0] = -2 * st[0] + 5;
    in[1] = st[0];
}

void EulerStep(double h) {
    Dynamic(t, st, N, in);
    for (unsigned i = 0; i < N; i++) {
        st[i] += in[i] * h;
    }

    t += h;
}

void print_event(void) {
    printf("Time: %lf, y': %g, y: %g\n", t, st[0], st[1]);
    cal_insert(print_event, 0, t + printstep);
}

int main() {
    cal_init();
    cal_insert(END, -100, end);
    cal_insert(print_event, 0, t);

    struct ev_notice e;
    while (!cal_empty()) {
        e = cal_get_first();
        cal_pop();

        while (t < e.atime) {
            if (t + stepsize > e.atime) {
                EulerStep(e.atime - t);
            }
            else {
                EulerStep(stepsize);
            }
        }
        e.evptr();
    }

    return 0;
}