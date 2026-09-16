
void fullCircleCordic(int angle, double *sine, double *cosine);
float degreeToRadian(int angle);
void cordicPassthrough(int theta, int *s, int *c);
int cordicDegrees(int angle);
void rhumbBearing(float delta_lon, float delta_iso_lat, float *bearing);
void calculateRhumb(float start_lat, float start_long, float end_lat, float end_long, float *bearing, int *distance);