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
**Verifying the code**

    gcc arc_gcc_21oct.c 
    ./a.out

  ![image](https://github.com/V-Pranathi/Automatic_Roof_Controller/assets/140998763/c40f2c75-2242-4c8e-a859-9a8efb9bc7df)
  
**Block Diagram**

![block_diagram_arc](https://github.com/V-Pranathi/Automatic_Roof_Controller/assets/140998763/972f5ab8-994a-4cd4-b9c4-9591b7b0c26b)

### <a name="c---code"></a> C - Code ###

x30[0] - Input from the sensor  
x30[1] - output to the motor  

    
    //#include <stdio.h>
    int main()
    {
    
    	int rain_sensor_ip;
    	
    	int roof_status_op = 0; 
    	int roof_status_op_reg;
    	
    	roof_status_op_reg = roof_status_op*2;
    	
    	
    		asm volatile(
    	    	"or x30, x30, %0\n\t"  
    	    	:
    	    	: "r" (roof_status_op_reg)
    		: "x30" 
    		);
    		
    	while(1)
    	{
    			
    		asm volatile(
    		"andi %0, x30, 0x01\n\t"
    		: "=r" (rain_sensor_ip)
    		:
    		:);
    	
    	if (rain_sensor_ip)
    	{
    	
    	roof_status_op = 0; 
    	
    	roof_status_op_reg = roof_status_op*2;
    			
    		asm volatile(
    		"or x30, x30, %0\n\t"   
    		:
    		: "r" (roof_status_op_reg)
    		: "x30" 
    		);
    	 //printf("Rain not detected. Roof opened.\n");
       
          //printf("roof_status_op=%d \n", roof_status_op);
    	
    	}	
    	else
    	{
    	
    	roof_status_op = 1; 
    	
    	roof_status_op_reg = roof_status_op*2;
    		asm volatile(
        		"or x30, x30, %0\n\t"  
        		:
        		: "r" (roof_status_op_reg)
    		: "x30" 
    		);
    	//printf("Rain detected. Roof closed.\n");
     
         // printf("roof_status_op=%d \n", roof_status_op);
    	}
    	}
    	
    	return 0;
    
    }


### <a name="assembly-code"></a> Assembly Code ### 

Converting the C code into the assebly code using the following commands:

    riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 -ffreestanding -nostdlib -o out arc.c 
    riscv64-unknown-elf-objdump -d -r out > asm.txt

   **Assembly Code**
           
   
    out:     file format elf32-littleriscv
    
    
    Disassembly of section .text:
    
    00010054 <main>:
       10054:	fe010113          	addi	sp,sp,-32
       10058:	00812e23          	sw	s0,28(sp)
       1005c:	02010413          	addi	s0,sp,32
       10060:	fe042623          	sw	zero,-20(s0)
       10064:	fec42783          	lw	a5,-20(s0)
       10068:	00179793          	slli	a5,a5,0x1
       1006c:	fef42423          	sw	a5,-24(s0)
       10070:	fe842783          	lw	a5,-24(s0)
       10074:	00ff6f33          	or	t5,t5,a5
       10078:	001f7793          	andi	a5,t5,1
       1007c:	fef42223          	sw	a5,-28(s0)
       10080:	fe442783          	lw	a5,-28(s0)
       10084:	02078063          	beqz	a5,100a4 <main+0x50>
       10088:	fe042623          	sw	zero,-20(s0)
       1008c:	fec42783          	lw	a5,-20(s0)
       10090:	00179793          	slli	a5,a5,0x1
       10094:	fef42423          	sw	a5,-24(s0)
       10098:	fe842783          	lw	a5,-24(s0)
       1009c:	00ff6f33          	or	t5,t5,a5
       100a0:	fd9ff06f          	j	10078 <main+0x24>
       100a4:	00100793          	li	a5,1
       100a8:	fef42623          	sw	a5,-20(s0)
       100ac:	fec42783          	lw	a5,-20(s0)
       100b0:	00179793          	slli	a5,a5,0x1
       100b4:	fef42423          	sw	a5,-24(s0)
       100b8:	fe842783          	lw	a5,-24(s0)
       100bc:	00ff6f33          	or	t5,t5,a5
       100c0:	fb9ff06f          	j	10078 <main+0x24>
       
### <a name="unique-instructions"></a> Unique Instructions ###
To find the number of unique instructions make sure to rename the filename as assembly.txt since the python script that we are using is opening the file name with assembly.txt and both files should be in the same directory. The python script I am using is already uploaded. Now follow the command to get the number of different instructions used.

    $ python3 instruction_counter.py // use this command after ensuring we are in the same directory as the script 

Number of different instructions: 9  
List of unique instructions:  
   sw  
   or  
   slli  
   beqz  
   j  
   lw  
   li  
   addi  
   andi  

## <a name="acknowledgement"></a> Acknowledgement ##
* Kunal Ghosh, VSD Corp. Pvt. Ltd.
* Mayank Kabra
* Bhargav, Colleague, IIITB

## <a name="references"></a> References ##
* https://randomnerdtutorials.com/guide-for-rain-sensor-fc-37-or-yl-83-with-arduino/
* https://github.com/SakethGajawada/RISCV-GNU


