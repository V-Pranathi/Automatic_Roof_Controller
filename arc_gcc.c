#include <stdio.h>
int main()
{

	int rain_sensor_ip;
	
	int roof_status_op = 0; 
  
		
	while(1)
	{
			
		rain_sensor_ip=1;
	
	if (rain_sensor_ip)
	{
	
	roof_status_op = 0; 
      printf("Rain not detected. Roof opened.\n");
   
      printf("roof_status_op=%d \n", roof_status_op);
	
	}	
	else
	{
	
	roof_status_op = 1; 
	
	printf("Rain detected. Roof closed.\n");
 
      printf("roof_status_op=%d \n", roof_status_op);

	}
	}
	
	return 0;

}

