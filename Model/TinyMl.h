#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define N_FEATURES 3
#define N_CLASSES 3
#define N_VECTORS 17
#define N_ROWS 3
#define N_COEFFICIENTS 2
#define N_INTERCEPTS 3
#define KERNEL_TYPE 'l'
#define KERNEL_GAMMA 0.001
#define KERNEL_COEF 0.0
#define KERNEL_DEGREE 3

double vectors[17][3] = {{16.0, 17.0, 23.0}, {13.0, 25.0, 27.0}, {24.0, 21.0, 7.0}, {28.0, 15.0, 18.0}, {13.0, 22.0, 10.0}, {16.0, 27.0, 21.0}, {24.0, 18.0, 19.0}, {8.0, 31.0, 25.0}, {13.0, 27.0, 20.0}, {20.0, 26.0, 23.0}, {13.0, 7.0, 20.0}, {13.0, 20.0, 18.0}, {21.0, 22.0, 16.0}, {20.0, 21.0, 18.0}, {20.0, 23.0, 19.0}, {6.0, 25.0, 15.0}, {10.0, 12.0, 21.0}};
double coefficients[2][17] = {{0.0037112628473542747, 0.10745707921393607, -0.0, -0.02983593151289587, -0.0, -0.0, -0.0, -0.0813324105483945, -0.0, -0.49358923474694866, -0.3351625751098237, -0.0, -0.0, -0.0, -0.0, -0.0, -0.17124819014322765}, {1.0, 0.0, 0.1407925997498373, 1.0, 1.0, 0.4155979425873918, 1.0, 1.0, 1.0, -1.0, -0.0, -0.7902811649553827, -1.0, -1.0, -1.0, -0.7661093773818467, -0.0}};
double intercepts[3] = {-4.371496908681692, -27.080667046065575, -7.381530743778079};
int weights[3] = {2, 7, 8};

int predict (double features[]) {
    int i, j, k, d, l;

    double kernels[N_VECTORS];
    double kernel;
    switch (KERNEL_TYPE) {
        case 'l':
            // <x,x'>
            for (i = 0; i < N_VECTORS; i++) {
                kernel = 0.;
                for (j = 0; j < N_FEATURES; j++) {
                    kernel += vectors[i][j] * features[j];
                }
                kernels[i] = kernel;
            }
            break;
        case 'p':
            // (y<x,x'>+r)^d
            for (i = 0; i < N_VECTORS; i++) {
                kernel = 0.;
                for (j = 0; j < N_FEATURES; j++) {
                    kernel += vectors[i][j] * features[j];
                }
                kernels[i] = pow((KERNEL_GAMMA * kernel) + KERNEL_COEF, KERNEL_DEGREE);
            }
            break;
        case 'r':
            // exp(-y|x-x'|^2)
            for (i = 0; i < N_VECTORS; i++) {
                kernel = 0.;
                for (j = 0; j < N_FEATURES; j++) {
                    kernel += pow(vectors[i][j] - features[j], 2);
                }
                kernels[i] = exp(-KERNEL_GAMMA * kernel);
            }
            break;
        case 's':
            // tanh(y<x,x'>+r)
            for (i = 0; i < N_VECTORS; i++) {
                kernel = 0.;
                for (j = 0; j < N_FEATURES; j++) {
                    kernel += vectors[i][j] * features[j];
                }
                kernels[i] = tanh((KERNEL_GAMMA * kernel) + KERNEL_COEF);
            }
            break;
    }

    int starts[N_ROWS];
    int start;
    for (i = 0; i < N_ROWS; i++) {
        if (i != 0) {
            start = 0;
            for (j = 0; j < i; j++) {
                start += weights[j];
            }
            starts[i] = start;
        } else {
            starts[0] = 0;
        }
    }

    int ends[N_ROWS];
    for (i = 0; i < N_ROWS; i++) {
        ends[i] = weights[i] + starts[i];
    }

    if (N_CLASSES == 2) {

        for (i = 0; i < N_VECTORS; i++) {
            kernels[i] = -kernels[i];
        }

        double decision = 0.;
        for (k = starts[1]; k < ends[1]; k++) {
            decision += kernels[k] * coefficients[0][k];
        }
        for (k = starts[0]; k < ends[0]; k++) {
            decision += kernels[k] * coefficients[0][k];
        }
        decision += intercepts[0];

        if (decision > 0) {
            return 0;
        }
        return 1;

    }

    double decisions[N_INTERCEPTS];
    double tmp;
    for (i = 0, d = 0, l = N_ROWS; i < l; i++) {
        for (j = i + 1; j < l; j++) {
            tmp = 0.;
            for (k = starts[j]; k < ends[j]; k++) {
                tmp += kernels[k] * coefficients[i][k];
            }
            for (k = starts[i]; k < ends[i]; k++) {
                tmp += kernels[k] * coefficients[j - 1][k];
            }
            decisions[d] = tmp + intercepts[d];
            d = d + 1;
        }
    }

    int votes[N_INTERCEPTS];
    for (i = 0, d = 0, l = N_ROWS; i < l; i++) {
        for (j = i + 1; j < l; j++) {
            votes[d] = decisions[d] > 0 ? i : j;
            d = d + 1;
        }
    }

    int amounts[N_CLASSES];
    for (i = 0, l = N_CLASSES; i < l; i++) {
        amounts[i] = 0;
    }
    for (i = 0; i < N_INTERCEPTS; i++) {
        amounts[votes[i]] += 1;
    }

    int classVal = -1;
    int classIdx = -1;
    for (i = 0; i < N_CLASSES; i++) {
        if (amounts[i] > classVal) {
            classVal = amounts[i];
            classIdx= i;
        }
    }
    return classIdx;

}

int main(int argc, const char * argv[]) {

    /* Features: */
    double features[argc-1];
    int i;
    for (i = 1; i < argc; i++) {
        features[i-1] = atof(argv[i]);
    }

    /* Prediction: */
    printf("%d", predict(features));
    return 0;

}
