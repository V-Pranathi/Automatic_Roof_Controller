# Automatic_Roof_Controller
This report is the progress of making a Automatic Roof Controller RISCV based application
* [1.Day - 1](#1-day---1)
  * [Automatic Roof Controller and its Working](#automatic-roof-controller-and-its-working)
  * [Block Diagram](#block-diagram)
  * [C - Code](#c---code)
  * [Assembly Code](#assembly-code)
  * [Unique Instructions](#unique-instructions)
* [Acknowledgemnet](#acknowledgement)
* [References](#references)

 
## <a name="1-day---1"></a> 1.Day - 1 ##
### <a name="automatic-roof-controller-and-its-working"></a> Automatic Roof Controller and its Working ###
The objective of this is to create a Rain sensor based Automatic Roof RISCV Controller. Rain sensor-based automatic roof controllers are primarily used in buildings and structures where the roof's position needs to be adjusted in response to rain or other weather conditions. These controllers are designed to detect rain and trigger the closing of the roof to protect the interior from water damage. Here are some common applications for rain sensor-based automatic roof controllers:  
1. Restaurants, cafes, and outdoor dining spaces often use rain sensor-based controllers to automatically close retractable roofs or awnings when it starts raining. This allows patrons to continue dining without being affected by rain.  
2. To prevent the harvested crops from the heavy rain and save the rain water. The rain sensor is used for the working of roof when there is rainfall.

#### Working Principle of the rain sensor ####
The working principle of the rain sensor(FC-37 rain sensor) is indeed straightforward : 
* Sensing Pad: The rain sensor has a sensing pad with exposed copper traces. These traces act as a variable resistor whose resistance changes with the amount of water on its surface.  
* Resistance Variation: The resistance of the sensing pad is inversely proportional to the amount of water present. In other words:  
        More water on the surface results in better conductivity and lower resistance.  
        Less water on the surface leads to poor conductivity and higher resistance.  
* Output Voltage: The sensor produces an output voltage that corresponds to the resistance of the sensing pad. This voltage is used to determine whether it's raining or not. When the sensing pad is wet, the output voltage will be different from when it's dry.  
* Electronic Module: The sensor includes an electronic module that connects the sensing pad to a microcontroller like Arduino. This module interfaces with the sensor and provides an output voltage at an Analog Output (AO) pin. This analog signal is proportional to the resistance of the sensing pad.  
* Digital Output: The sensor module also contains an LM393 High Precision Comparator, which is used to digitize the analog signal. The digitized signal is made available at a Digital Output (DO) pin.  
* Sensitivity Adjustment: The sensor module typically features a built-in potentiometer that allows for sensitivity adjustment of the digital output (DO). By adjusting the potentiometer, you can set a threshold value. When the amount of water on the sensing pad exceeds this threshold, the module will output a logic LOW signal; otherwise, it will output a logic HIGH signal.  
* Rain Detection: To detect rain, we monitor the digital output signal. If the signal goes LOW, it indicates that the threshold has been exceeded, and the sensor has detected rain. Conversely, a HIGH signal means no rain is detected.  

### <a name="block-diagram"></a> Block Diagram ###
**Circuit Diagram**

![image](https://github.com/V-Pranathi/Automatic_Roof_Controller/assets/140998763/8e4a947a-2abf-4f2c-9d16-6ca04ce4b456)  

**Block Diagram**

![block_diagram_arc](https://github.com/V-Pranathi/Automatic_Roof_Controller/assets/140998763/972f5ab8-994a-4cd4-b9c4-9591b7b0c26b)

### <a name="c---code"></a> C - Code ###

x30[0] - Input from the sensor  
x30[2] - output to the motor  

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


### <a name="assembly-code"></a> Assembly Code ### 

Converting the C code into the assebly code using the following commands:

    $ /home/pranathi/riscv32-toolchain/bin/riscv32-unknown-elf-gcc -march=rv32i -mabi=ilp32 -ffreestanding -nostdlib -o ./out arc.c
    $ /home/pranathi/riscv32-toolchain/bin/riscv32-unknown-elf-objdump -d -r out > assembly.txt

   **Assembly Code**
           
    out:     file format elf32-littleriscv
    
    
    Disassembly of section .text:
    
    00010074 <main>:
       10074:	ff010113          	add	sp,sp,-16
       10078:	00112623          	sw	ra,12(sp)
       1007c:	00812423          	sw	s0,8(sp)
       10080:	01010413          	add	s0,sp,16
       10084:	008000ef          	jal	1008c <read>
       10088:	ffdff06f          	j	10084 <main+0x10>
    
    0001008c <read>:
       1008c:	fe010113          	add	sp,sp,-32
       10090:	00812e23          	sw	s0,28(sp)
       10094:	02010413          	add	s0,sp,32
       10098:	001f7793          	and	a5,t5,1
       1009c:	fef42623          	sw	a5,-20(s0)
       100a0:	fec42703          	lw	a4,-20(s0)
       100a4:	00100793          	li	a5,1
       100a8:	00f70e63          	beq	a4,a5,100c4 <read+0x38>
       100ac:	ffb00793          	li	a5,-5
       100b0:	fef42423          	sw	a5,-24(s0)
       100b4:	fe842783          	lw	a5,-24(s0)
       100b8:	00ff7f33          	and	t5,t5,a5
       100bc:	004f6f13          	or	t5,t5,4
       100c0:	0180006f          	j	100d8 <read+0x4c>
       100c4:	ffb00793          	li	a5,-5
       100c8:	fef42423          	sw	a5,-24(s0)
       100cc:	fe842783          	lw	a5,-24(s0)
       100d0:	00ff7f33          	and	t5,t5,a5
       100d4:	000f6f13          	or	t5,t5,0
       100d8:	00000013          	nop
       100dc:	01c12403          	lw	s0,28(sp)
       100e0:	02010113          	add	sp,sp,32
       100e4:	00008067          	ret
       
### <a name="unique-instructions"></a> Unique Instructions ###
To find the number of unique instructions make sure to rename the filename as assembly.txt since the python script that we are using is opening the file name with assembly.txt and both files should be in the same directory. The python script I am using is already uploaded. Now follow the command to get the number of different instructions used.

    $ python3 instruction_counter.py // use this command after ensuring we are in the same directory as the script 

Number of different instructions: 11  
List of unique instructions:  
or  
li  
j  
add  
sw  
ret  
and   
lw  
beq   
nop  
jal  
  

## <a name="acknowledgement"></a> Acknowledgement ##
* Kunal Ghosh, VSD Corp. Pvt. Ltd.
* Mayank Kabra
* Bhargav, Colleague, IIITB

## <a name="references"></a> References ##
* https://randomnerdtutorials.com/guide-for-rain-sensor-fc-37-or-yl-83-with-arduino/
* https://github.com/SakethGajawada/RISCV-GNU


