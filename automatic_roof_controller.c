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

