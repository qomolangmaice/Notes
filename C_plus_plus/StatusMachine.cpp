#include <iostream>

enum Status
{
	START_STATUS,
	ACTIVE_STATUS,
	WORK_STATUS,
	ZOMBIE_STATUS,
	WAKE_UP_STATUS,
	STOP_STATUS,
	END_STATUS
};

void print_status(char* status)
{
	std::cout << status << std::endl;
}

int main(int argc, char *argv[])
{
	int status = 0;

	while(status != END_STATUS)
	{
		switch(status)
		{
		case START_STATUS:
			print_status("Start process");
			status = ACTIVE_STATUS;
			break;
		case ACTIVE_STATUS:
			print_status("Active process");
			status = WORK_STATUS;
			break;
		case WORK_STATUS:
			print_status("Work process");
			status = ZOMBIE_STATUS;
			break;
		case ZOMBIE_STATUS:
			print_status("Zombie process");
			status = WAKE_UP_STATUS;
			break;
		case WAKE_UP_STATUS:
			print_status("Wake up process");
			status = STOP_STATUS;
			break;
		case STOP_STATUS:
			print_status("Stop process");
			status = START_STATUS;
			break;
		}
	}

	return 0;
}
