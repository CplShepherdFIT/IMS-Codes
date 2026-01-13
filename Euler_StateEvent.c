#include <stdio.h>
#include <stdbool.h>

void Dynamic(double t, double st[], unsigned N, double in[]) {
    in[0] = -st[1];
    in[1] = st[0];
}

void EulerStep(double t, double st[], unsigned N, double stepsize) {
    double in[2];
    
    Dynamic(t, st, N, in);
    for (int i = 0; i < N; i++) {
        st[i] += in[i] * stepsize;
    }
}

void StateEvent(double t, double st[]) {
    printf("Time: %lf, y': %g, y: %g\n", t, st[0], st[1]);
}

int main() {
    const unsigned N = 2;
    double t = 0.0; const double end = 50.0;
    double normalstep = 0.1; 
    double minstep = 0.01; 
    double stepsize = normalstep;
    double st[] = {1.0, 1.0};

    while (t < end) {
        double backup[N];
        for (unsigned i = 0; i < N; i++) { backup[i] = st[i]; }
        double time_backup = t;

        bool cond = st[0] > st[1];
        EulerStep(t, st, N, stepsize);
        t += stepsize;
        bool newcond = st[0] > st[1];

        if (cond != newcond) {
            if (stepsize <= minstep) {
                StateEvent(t, st);
                stepsize = normalstep;
            } else {
                t = time_backup;
                for (unsigned i = 0; i < N; i++) { st[i] = backup[i]; }
                stepsize /= 2;
                if (stepsize < minstep) {
                    stepsize = minstep;
                }
            }
        }
    }

    return 0;
}