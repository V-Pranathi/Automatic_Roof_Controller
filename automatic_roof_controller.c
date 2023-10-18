
//#include <stdio.h>
void read();

int main(){
    while(1){
        read();
    }
    return(0);
}  
        
  void read(){
  
 int rain_sensor_ip;
  asm volatile(
            "and %0, x30, 1\n\t"
            : "=r"(rain_sensor_ip)
        );
        
 int roof_status_op;
 int dummy;
  
        if (rain_sensor_ip!=1) {
        // It's raining, close the roof (replace with actual roof control)
      //printf("Rain detected. Roof closed.\n");
     // roof_status_op = 1;
      //printf("roof_status_op=%d \n", roof_status_op);
      
       dummy = 0xFFFFFFFB;
        asm volatile(
            "and x30, x30, %0\n\t"     
            "or x30, x30, 4\n\t"    // output at 3rd bit, that switches on the motor(........000100)
            :
            :"r"(dummy)
            :"x30"
            );
        
    } else {
        // No rain, open the roof (replace with actual roof control)
       //printf("No rain detected. Roof opened.\n");
      // roof_status_op = 0;
      //printf("roof_status_op=%d \n", roof_status_op);
       dummy = 0xFFFFFFFB;
       asm volatile(
            "and x30, x30, %0\n\t"    
            "or x30, x30, 0\n\t"       // output at 3rd bit , that switches off the motor(........000)
            :
            :"r"(dummy)
            :"x30"
        );
    }

  

}

