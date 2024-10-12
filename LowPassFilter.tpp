template <int order>
LowPass<order>::LowPass(float f0, float fs, bool adaptive) {
    // Constructor implementation
    omega0 = 6.28318530718 * f0;
    dt = 1.0 / fs;
    adapt = adaptive;
    tn1 = -dt;

    for (int k = 0; k < order + 1; k++) {
        x[k] = 0;
        y[k] = 0;
    }

    setCoef();
}

template <int order>
void LowPass<order>::setCoef() {
    if (adapt) {
        float t = micros() / 1.0e6;
        dt = t - tn1;
        tn1 = t;
    }

    float alpha = omega0 * dt;

    if (order == 1) {
        a[0] = -(alpha - 2.0) / (alpha + 2.0);
        b[0] = alpha / (alpha + 2.0);
        b[1] = alpha / (alpha + 2.0);
    } else if (order == 2) {
        float alphaSq = alpha * alpha;
        float beta[] = {1, sqrt(2), 1};
        float D = alphaSq * beta[0] + 2 * alpha * beta[1] + 4 * beta[2];
        b[0] = alphaSq / D;
        b[1] = 2 * b[0];
        b[2] = b[0];
        a[0] = -(2 * alphaSq * beta[0] - 8 * beta[2]) / D;
        a[1] = -(beta[0] * alphaSq - 2 * beta[1] * alpha + 4 * beta[2]) / D;
    }
}

template <int order>
float LowPass<order>::filt(float xn) {
    if (adapt) {
        setCoef(); // Update coefficients if necessary
    }

    y[0] = 0;
    x[0] = xn;

    for (int k = 0; k < order; k++) {
        y[0] += a[k] * y[k + 1] + b[k] * x[k];
    }
    y[0] += b[order] * x[order];

    // Save historical values
    for (int k = order; k > 0; k--) {
        y[k] = y[k - 1];
        x[k] = x[k - 1];
    }

    return y[0];
}