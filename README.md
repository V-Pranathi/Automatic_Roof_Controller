# Automatic_Roof_Controller
This report is the progress of making a Automatic Roof Controller RISCV based application
* [1.Day - 1](#1-day---1)
  * [Automatic Roof Controller and its Working](#automatic-roof-controller-and-its-working)
  * [Block Diagram](#block-diagram)
  * [Spike Simulation](#spike-simulation)
  * [C - Code](#c---code)
  * [Assembly Code](#assembly-code)
  * [Unique Instructions](#unique-instructions)
  * [Functional Simulation](#functional_simulation)
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

### <a name="spike-simulation"></a> Spike Simulation ###  

* We tested the functional simulation above but here using spike also we would be doing fuctional simulation but here the differnce is we are linking the  c program with the inline assembly(**asm**).

Modifying C code for spike simulation and verification which can be found above as ```arc_spike.c```

Commands to run spike: 

     riscv64-unknown-elf-gcc -march=rv64i -mabi=lp64 -ffreestanding -o out arc.c
     spike pk out

![image](https://github.com/V-Pranathi/Automatic_Roof_Controller/assets/140998763/b581d3eb-f956-48e0-b61a-50d17fc78815)  

* Based on my application whenever the sensor reads '0' (rain_sensor_ip) which means rain is falling then the roof should be closed which is '1'(roof_status_op). We can observe the same in the above spike simulation my input is being '0' my output for that input is showing '1' and for the x30 register positions of input and output are x30[0] and x30[1] respectively for the given input and output combination my x30 register should be '2'.
  
* Similarly, whenever the sensor reads '1' (rain_sensor_ip) which means rain is not falling then the roof should be opened which is '0'(roof_status_op). We can observe the same in the above spike simulation my input is being '1' ,so my output for that input is showing '0' and for the x30 register positions of input and output are x30[0] and x30[1] respectively for the given input and output combination my x30 register should be '1'. As expected, we can see the Spike simulation results from the above figure.  

### <a name="c---code"></a> C - Code ###

x30[0] - Input from the sensor  
x30[1] - output to the motor  

    
       int main()
    {
   
   	int rain_sensor_ip;
   	
   	int roof_status_op = 0; 
   	int roof_status_op_reg;
   	int mask =0xFFFFFFFD;
   	roof_status_op_reg = roof_status_op*2;
   	
   	
   		asm volatile(
   		"and x30, x30, %1\n\t"
   	    	"or x30, x30, %0\n\t"  
   	    	:
   	    	: "r" (roof_status_op_reg), "r"(mask)
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
   	mask =0xFFFFFFFD;
   	roof_status_op_reg = roof_status_op*2;
   			
   		asm volatile(
   		"and x30,x30, %1\n\t"  
   		"or x30, x30, %0\n\t"   
   		:
   		: "r" (roof_status_op_reg), "r"(mask)
   		: "x30" 
   		);
   	 //printf("Rain not detected. Roof opened.\n");
      
         //printf("roof_status_op=%d \n", roof_status_op);
   	
   	}	
   	else
   	{
   	
   	roof_status_op = 1; 
   	mask =0xFFFFFFFD;
   	roof_status_op_reg = roof_status_op*2;
   		asm volatile(
   		"and x30,x30, %1\n\t"  
       		"or x30, x30, %0\n\t"  
       		:
       		: "r" (roof_status_op_reg), "r"(mask)
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
       10064:	ffd00793          	li	a5,-3
       10068:	fef42423          	sw	a5,-24(s0)
       1006c:	fec42783          	lw	a5,-20(s0)
       10070:	00179793          	slli	a5,a5,0x1
       10074:	fef42223          	sw	a5,-28(s0)
       10078:	fe442783          	lw	a5,-28(s0)
       1007c:	fe842703          	lw	a4,-24(s0)
       10080:	00ef7f33          	and	t5,t5,a4
       10084:	00ff6f33          	or	t5,t5,a5
       10088:	001f7793          	andi	a5,t5,1
       1008c:	fef42023          	sw	a5,-32(s0)
       10090:	fe042783          	lw	a5,-32(s0)
       10094:	02078863          	beqz	a5,100c4 <main+0x70>
       10098:	fe042623          	sw	zero,-20(s0)
       1009c:	ffd00793          	li	a5,-3
       100a0:	fef42423          	sw	a5,-24(s0)
       100a4:	fec42783          	lw	a5,-20(s0)
       100a8:	00179793          	slli	a5,a5,0x1
       100ac:	fef42223          	sw	a5,-28(s0)
       100b0:	fe442783          	lw	a5,-28(s0)
       100b4:	fe842703          	lw	a4,-24(s0)
       100b8:	00ef7f33          	and	t5,t5,a4
       100bc:	00ff6f33          	or	t5,t5,a5
       100c0:	fc9ff06f          	j	10088 <main+0x34>
       100c4:	00100793          	li	a5,1
       100c8:	fef42623          	sw	a5,-20(s0)
       100cc:	ffd00793          	li	a5,-3
       100d0:	fef42423          	sw	a5,-24(s0)
       100d4:	fec42783          	lw	a5,-20(s0)
       100d8:	00179793          	slli	a5,a5,0x1
       100dc:	fef42223          	sw	a5,-28(s0)
       100e0:	fe442783          	lw	a5,-28(s0)
       100e4:	fe842703          	lw	a4,-24(s0)
       100e8:	00ef7f33          	and	t5,t5,a4
       100ec:	00ff6f33          	or	t5,t5,a5
       100f0:	f99ff06f          	j	10088 <main+0x34>
       
### <a name="unique-instructions"></a> Unique Instructions ###
To find the number of unique instructions make sure to rename the filename as assembly.txt since the python script that we are using is opening the file name with assembly.txt and both files should be in the same directory. The python script I am using is already uploaded. Now follow the command to get the number of different instructions used.

    $ python3 instruction_counter.py // use this command after ensuring we are in the same directory as the script 

Number of different instructions: 10
List of unique instructions:  
or  
j  
beqz  
sw  
addi  
andi  
li  
and  
slli  
lw  


### <a name="functional_simulation"></a> Functional Simulation ###

* We will do the functional simulation for the processors that are being created for the assembly program that is being created for my application. The ```processor.v``` and the ```testbench.v``` is uploaded and those can be seen above.  
* Commands to run the verilog file

      iverilog -o test processor.v testbench.v
      ./test
      gtkwave waveform.vcd

![image](https://github.com/V-Pranathi/Automatic_Roof_Controller/assets/140998763/536d16bf-4788-4392-90bc-2c01541ea7fa)

![image](https://github.com/V-Pranathi/Automatic_Roof_Controller/assets/140998763/fb2d8275-4c30-41a3-b7a1-8fd41bd42ca0)

* Since we bypassed the UART write_done is becoming at t=o itself and the ID_instruction starts executing after making the reset zero.
    
![image](https://github.com/V-Pranathi/Automatic_Roof_Controller/assets/140998763/62d6f074-e0c5-46db-a510-63943a054250)

* In the testbench I have given the input initially as "1" and then changing the input to "0" correspondingly output is inverted.  By this we verified the functional simulation and now can proceed with the gate level simulation.
* And the spikes that are visible in the output signal is due to resetting the output to zero after every cycle.
  
![image](https://github.com/V-Pranathi/Automatic_Roof_Controller/assets/140998763/d27ea076-0efc-405b-92b7-7fde491ad73c)

![image](https://github.com/V-Pranathi/Automatic_Roof_Controller/assets/140998763/c55fe4fc-85c3-43da-a293-1ac2eb2e8144)

* The ```$signal[31:0]``` is actually represents x30 register where inputs are being stored in there from the sensor retieving thoe values to compute the output and then stored back in the x30 register.
* The ```ID_instruction``` , each instruction is decoded and then only the later steps takes place one by one.

![image](https://github.com/V-Pranathi/Automatic_Roof_Controller/assets/140998763/2f90ebe1-ed37-4331-8473-7c38c15cbc65)

* In the above figure when ID_instruction is being ```02078863```  the pc is getting jumped to a particular instruction this is because at that instruction it is checking whether the input is being 1 or not since it is not '1' so the pc value is not incrementing by one instead jumping to another location.  

           10094:	02078863          	beqz	a5,100c4 <main+0x70>

![image](https://github.com/V-Pranathi/Automatic_Roof_Controller/assets/140998763/a52bfaee-bcfb-4034-b689-73999a01d168)

* Output is changing from 0 to 1 after the execution of instruction ```00FF6F33```      
  
![image](https://github.com/V-Pranathi/Automatic_Roof_Controller/assets/140998763/db1bce17-1feb-4197-8476-3ca5266fd750)

* Output is changing from 1 to 0 after the execution of instruction ```00ef7f33```  


    
## <a name="acknowledgement"></a> Acknowledgement ##
* Kunal Ghosh, VSD Corp. Pvt. Ltd.
* Mayank Kabra
* Bhargav, Colleague, IIITB

## <a name="references"></a> References ##
* https://randomnerdtutorials.com/guide-for-rain-sensor-fc-37-or-yl-83-with-arduino/
* https://github.com/SakethGajawada/RISCV-GNU


