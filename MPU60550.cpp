#include "mbed.h"

Serial pc(SERIAL_TX, SERIAL_RX);
I2C i2c(PB_9, PB_8 );

const int addr = 0xD0;      // 7 bit I2C address
char cmd[2];
char read_buffer[14];

int16_t acc_x = 0, acc_y = 0, acc_z = 0;
int16_t gyr_x = 0, gyr_y = 0, gyr_z = 0;
int16_t temp = 0;

float ACC_SEN = 16384.0;    //Resolución ACC
float GYR_SEN = 131.0;      //Resolución GYR
float TEM_SEN = 340.0;      //Resolución temp

float facc_x = 0, facc_y = 0, facc_z = 0;
float fgyr_x = 0, fgyr_y = 0, fgyr_z = 0;
float ftemp = 0;

DigitalOut myled(LED1);

int main()
{
    pc.printf("Prueba MPU6050 \n\r");

    cmd[0] = 0x6B;
    cmd[1] = 0x00;
    i2c.write(addr, cmd, 2);                //Desactivar modo hibernación
    
    cmd[0] = 0x1B;
    cmd[1] = 0x00;
    i2c.write(addr, cmd, 2);                //gyro full scale 250 DPS

    cmd[0] = 0x1C;
    cmd[1] = 0x00;
    i2c.write(addr, cmd, 2);                //ACC fullsclae 2G

    while(1) {
        wait(0.25);

        cmd[0]=0x3B;
        i2c.write(addr, cmd, 1);            //Escritura del registro de inicio
        i2c.read(addr, read_buffer, 14);    //Lectura en rafaga de los valores de la MPU

        //.................Construcción de la medición de los valores .................. 
        acc_x = read_buffer[0]<<8 | read_buffer[1];    
        acc_y = read_buffer[2]<<8 | read_buffer[3];
        acc_z = read_buffer[4]<<8 | read_buffer[5];
        temp  = read_buffer[6]<<8 | read_buffer[7];
        gyr_x = read_buffer[8]<<8 | read_buffer[9];
        gyr_y = read_buffer[10]<<8 | read_buffer[11];
        gyr_z = read_buffer[12]<<8 | read_buffer[13];
        
        
        //pc.printf("ACCx = %i ACCy = %i ACCz = %i \n\r", acc_x, acc_y, acc_z);
        
        facc_x = acc_x/ACC_SEN;
        facc_y = acc_y/ACC_SEN;
        facc_z = acc_z/ACC_SEN;
               
        //pc.printf("ACCx = %.2f ACCy = %.2f ACCz = %.2f \n\r", facc_x, facc_y, facc_z);
        
        fgyr_x = gyr_x/GYR_SEN;
        fgyr_y = gyr_y/GYR_SEN;
        fgyr_z = gyr_z/GYR_SEN;
               
        pc.printf("GYRx = %.2f GYRy = %.2f GYRz = %.2f \n\r", fgyr_x, fgyr_y, fgyr_z);

        ftemp= (temp - 521)/TEM_SEN;
        
        //pc.printf("Temp = %.2f \r\n",ftemp);

       /* for(int i=0; i<14; i++) {
            pc.printf("Buffer[%i] = %i \n\r",i,read_buffer[i]);
        }*/

        myled = !myled;
    }
}
