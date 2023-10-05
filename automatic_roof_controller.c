//#include <stdio.h>
void read();
void control_roof();
 //assuming sensors gives 0 if it detects rain
    //sensors to detect rain :    0    
    //gpio's for motors operating the roof  : 2
int main(){
    while(1){
        read();
    }
    return(0);
}

void control_roof() {
int rain_sensor_ip;
int roof_status_op;
int dummy;

    if (rain_sensor_ip!=1) {
        // It's raining, close the roof (replace with actual roof control)
       //printf("Rain detected. Roof closed.\n");
       dummy = 0xFFFFFFFB;
        asm(
            "and x30,x30, %0\n\t"     // Load immediate 1 into x30
            "or %1, x30, 4\n\t"           // output at 3rd bit, that switches on the motor
            :"=r"(dummy)
            :"r"(roof_status_op)
            );

        
    } else {
        // No rain, open the roof (replace with actual roof control)
       //printf("No rain detected. Roof opened.\n");
       dummy = 0xFFFFFFFB;
       asm(
            "and x30,x30, %0\n\t"     // Load immediate 1 into x30
            "or %1, x30, 0\n\t"       //// output at 3rd bit , that switches off the motor
            :"=r"(dummy)
            :"r"(roof_status_op)
        );
    }
}

void read(){
// Simulated rain sensor input (1: No rain, 0: Rain)
 // rain_sensor_ip = digital_read(0);
 int rain_sensor_ip;
 asm (
            "and %0, x30, 1\n\t"
            : "=r"(rain_sensor_ip)
        );
 // Roof status output (0: Open, 1: Closed)
 // roof_status_op = digital_write(2);
control_roof();
}




