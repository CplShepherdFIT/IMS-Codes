#include <stdio.h>

void Dynamic(double t, double st[], unsigned N, double in[]) {
    in[0] = -2 * st[0] + 5;
    in[1] = st[0];
}

void EulerStep(double t, double st[], unsigned N, double stepsize) {
    double in[2];
    
    Dynamic(t, st, N, in);
    for (int i = 0; i < N; i++) {
        st[i] += in[i] * stepsize;
    }
}

int main() {
    const unsigned N = 2;
    double t = 0.0; const double end = 20.0;
    double stepsize = 0.033;
    double st[] = {1.0, 1.0};

    printf("Time: %lf, y': %g, y: %g\n", t, st[0], st[1]);
    while (t < end) {
        if (t + stepsize > end) {
            EulerStep(t, st, N, end - t);
            t = end;
        } else {
            EulerStep(t, st, N, stepsize);
            t += stepsize;
        }
        printf("Time: %lf, y': %g, y: %g\n", t, st[0], st[1]);
    }

    return 0;
}