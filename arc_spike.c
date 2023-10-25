 #include<stdio.h>
    int main()
    {
        int testing;
    	int rain_sensor_ip;
    	int i;
	
     	int roof_status_op = 0; 
     	int mask =0xFFFFFFFD;
     	int roof_status_op_reg;
     	
     	roof_status_op_reg = roof_status_op*2;
     	
     	
     		asm volatile(
     		"and x30, x30, %1\n\t"
     	    	"or x30, x30, %0\n\t"  
     	    	:
     	    	: "r" (roof_status_op_reg), "r"(mask)
     		: "x30" 
     		);
     		
     	for(int j=0;j<5;j++)
     	{
     			if(j<3)
     			i=0;
     			else
     			i=1;
     		asm volatile(
     		"or x30, x30, %1\n\t"
     		"andi %0, x30, 0x01\n\t"
     		: "=r" (rain_sensor_ip)
     		: "r" (i)
     		: "x30"
     		);
     	
     	if (rain_sensor_ip)
     	{
     	
     	roof_status_op = 0; 
     	 //printf("Rain not detected. Roof opened.\n");
             mask =0xFFFFFFFD;
          // printf("roof_status_op=%d \n", roof_status_op);
     	roof_status_op_reg = roof_status_op*2;
     			
     		asm volatile(
     		"and x30, x30, %1\n\t"
     		"or x30, x30, %0\n\t"   
     		:
     		: "r" (roof_status_op_reg), "r"(mask)
     		: "x30" 
     		);
     	
     	
     	}	
     	else
     	{
     	
     	roof_status_op = 1; 
     	//printf("Rain detected. Roof closed.\n");
          //printf("roof_status_op=%d \n", roof_status_op);
             mask =0xFFFFFFFD;
     	roof_status_op_reg = roof_status_op*2;
     		asm volatile(
      		        "and x30, x30, %1\n\t"
         		"or x30, x30, %0\n\t"  
         		:
         		: "r" (roof_status_op_reg), "r"(mask)
     		: "x30" 
     		);
     	
     	
     	}
     	
     	  asm volatile(
         		"addi %0, x30, 0\n\t"
         		:"=r"(testing)
         		:
         		:"x30"
         		);
         	 printf("x30 = %d\n",testing);
     	printf("rain_sensor_ip=%d \n", rain_sensor_ip);
     	 printf("roof_status_op=%d \n", roof_status_op);
     	 
     	
     	}
     	
     	return 0;

    }
