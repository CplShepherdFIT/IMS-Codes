#include <stdio.h>

void Dynamic(double t, double st[], unsigned N, double in[]) {
    in[0] = -8 * st[1] + 1;
    in[1] = st[0];
    in[2] = st[1];
}

void RK_step(double t, double state[], unsigned N, double stepsize) {
    double k1[N], k2[N], mid[N];
    Dynamic(t, state, N, k1);

    for (int i = 0; i < N; i++) {
        mid[i] = state[i] + 0.75 * k1[i] * stepsize;
    }
    Dynamic(t + 0.75 * stepsize, mid, N, k2);

    for (int i = 0; i < N; i++) {
        state[i] += ((1.0/3.0) * k1[i] + (2.0/3.0) * k2[i]) * stepsize;
    }
}

void SimRun(double t1, double t2) {
    unsigned N = 3;
    double state[] = {1.0, 1.0, 1.0};
    double stepsize = 0.1;

    printf("Time: %lf, y'': %g, y': %g, y: %g\n", t1, state[0], state[1], state[2]);
    while (t1 < t2) {
        if (t1 + stepsize > t2) {
            RK_step(t1, state, N, t2 - t1);
            t1 = t2;
        } else {
            RK_step(t1, state, N, stepsize);
            t1 += stepsize;
        }
        printf("Time: %lf, y'': %g, y': %g, y: %g\n", t1, state[0], state[1], state[2]);
    }
}

int main() {
    SimRun(0.0, 20.0);
    return 0;
}