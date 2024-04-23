/*
 * states.h
 *
 * Created: 17.04.2024 14:50:26
 *  Author: jarle
 */ 


#ifndef STATES_H_
#define STATES_H_




enum mode { //definerer states for vifter
	RPM,
	OFF,
	AUTO
};

const char *state_names[] = { "RPM", "OFF", "AUTO" };
typedef struct rpm {
	enum mode State;
	uint8_t rpm; // RPM value (0-100)
	}Fan;
	


	Fan fan1 = {AUTO, 0 };
	Fan fan2 = {OFF, 0 };
	Fan fan3 = {OFF, 0 };
	Fan fan4 = {OFF, 0 };



/*

void handleFan1(char *command){ //endre vifte 1
	printf("Type wanted mode or rpm \r\n");
	printf("Alternatives: RPM, OFF, AUTO\r\n");
	returnUart(command);
	if (strcmp(command, "RPM") == 0) {
		printf("Type wanted rpm \r\n");
		returnUart(command);
		fan1.State = RPM;
		fan1.rpm = atoi(command);
		printf("fan1 RPM is set to: %d\r\n", atoi(command));
		}
	else if (strcmp(command, "OFF") == 0) {
		fan1.State = OFF;
		fan1.rpm = 0;
		printf("fan1 is now OFF\r\n");
		}
	else if (strcmp(command, "AUTO") == 0) {
		fan1.State = AUTO;
		
		printf("fan1 is now in mode AUTO\r\n");

	}
	else {
		;
	}
}

*/

/*
void handleFan2(selectedMode){ //endre vifte 2
	
	if (strcmp(selectedMode, "RPM") == 0) {
		printf("Type wanted rpm \r\n");
		returnUart(command);
		fan.State = RPM;
		fan2.rpm = atoi(command);
		printf("fan2 RPM is set to: %d\r\n", atoi(command));
	}
	else if (strcmp(selectedMode, "OFF") == 0) {
		fan2.State = OFF;
		fan2.rpm = 0;
		printf("fan2 is now OFF\r\n");
	}
	else if (strcmp(selectedMode, "AUTO") == 0) {
		fan2.State = AUTO;
		printf("fan2 is now in mode AUTO\r\n");

	}
	
	else {
		;
	}
}
*/


void selectFan (char *command) { //test
	
	switch(atoi(command)) {
		
	case 1: 
		printf("fan1 selected\r\n");
		//handleFan1(command);
		break;
	case 2: 
		printf("fan2 selected\r\n");
		//handleFan2(command);
		break;
	case 3: 
		printf("fan3 selected\r\n");
		//handleFan3(command);
		break;
	case 4:
		printf("fan4 selected\r\n"); 
		//handleFan4(command);
		break;
	
	}
}


#endif /* STATES_H_ */
