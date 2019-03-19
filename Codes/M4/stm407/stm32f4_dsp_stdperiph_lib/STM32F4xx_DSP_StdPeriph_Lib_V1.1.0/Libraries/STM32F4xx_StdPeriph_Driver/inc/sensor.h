typedef struct{
float rx;
float ry;
float rz;
float ax;
float ay;
float az;

}_IMU;

typedef struct  {
float fx,fy,fz;
float tx,ty,tz;

}_FORCE;
char read_sensors(float *rx,float *ry, float*rz);

