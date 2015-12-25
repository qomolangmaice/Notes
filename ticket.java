import java.util.*;

//座位有3个基本属性：座位类型（F/E），座位编号，座位预定状态（是否已经被预定）
class Seat{
	public String type;
	public int number;
	public boolean vacancy;

	public void set_type(String type) {
		this.type = type;
	} 

	public String get_type() {
		return type;
	}

	public void set_number(int number) {
		this.number = number;
	}

	public int get_number() {
		return number;
	}

	public void set_vacancy(boolean vacancy) {
		this.vacancy = vacancy;
	}

	public boolean get_vacancy() {
		return vacancy;
	}
}

class Ticket extends Seat {
	public String passenger_name;

	public void set_passenger_name(String passenger_name) {
		this.passenger_name = passenger_name;
	}

	public String get_passenger_name() {
		return passenger_name;
	}
}

public class ticket{	

	//定义全局变量舱位，票，各类舱位剩余数，便于其他函数全局访问
	public static int surplusF = 5;
	public static int surplusE = 20;
	public static int flag;
	public static String seat_type;
	public static int seat_number;
	public static String name = "";
	public static ArrayList<Seat> seats = new ArrayList<Seat>();
	public static ArrayList<Ticket> tickets = new ArrayList<Ticket>();

	public static void main(String[] args) {
		//初始化舱位和票
		for(int i=0; i < 25; i++){
			//初始化舱位
			Seat seat = new Seat();
			if (i < 5) {
					seat.set_type("F");
					seat.set_number(i + 1);
			}
			else{
				seat.set_type("E");
				seat.set_number(i+1 - 5);
			}
			seat.set_vacancy(false);
			seats.add(seat);

			//初始化票
			Ticket ticket = new Ticket();
			if (i < 5) {
				ticket.set_type("F");
				ticket.set_number(i + 1);
			}
			else{
				ticket.set_type("E");
				ticket.set_number(i+1 - 5);
			}
			ticket.set_vacancy(false);
			ticket.set_passenger_name("空");
			tickets.add(ticket);
		}	

		// System.out.printf("\n------------座位------------\n座位号   座位类型   是否预定\n");
		// for(int i=0; i < seats.size(); i++)	{
		// 	if (i < 14) {		
		// 		System.out.printf("  %d       %s     %s\n", 
		// 		seats.get(i).get_number(), 
		// 		seats.get(i).get_type(), 
		// 		seats.get(i).get_vacancy());
		// 	}	
		// 	else {
		// 		System.out.printf(" %d       %s     %s\n", 
		// 		seats.get(i).get_number(), 
		// 		seats.get(i).get_type(), 
		// 		seats.get(i).get_vacancy());
		// 	}
		// }

		// System.out.printf("\n-------------机票--------------\n票号   姓名   票类型   是否预定\n");
		// for(int i=0; i < tickets.size(); i++)	{
		// 	if (i < 14) {		
		// 		System.out.printf("  %d     %s    %s    %s\n", 
		// 			tickets.get(i).get_number(), 
		// 			tickets.get(i).get_passenger_name(), 
		// 			tickets.get(i).get_type(), 
		// 			tickets.get(i).get_vacancy());
		// 	}	
		// 	else {
		// 		System.out.printf(" %d     %s    %s    %s\n", 
		// 			tickets.get(i).get_number(), 
		// 			tickets.get(i).get_passenger_name(), 
		// 			tickets.get(i).get_type(), 
		// 			tickets.get(i).get_vacancy());
		// 	}
		// }

		//主程序开始
		System.out.println("===============================");
		System.out.println("       欢迎使用K票务系统          ");
		System.out.println("===============================");

		//使用Scanner类读取用户输入
		Scanner stdin = new Scanner(System.in);
		System.out.println("请输入乘客姓名: ");

		while(name != null) {
			name = stdin.nextLine();
			if (name.equals("."))
				break;
			if (surplusF + surplusE == 0){
				System.out.println("很遗憾，所有票已售完，谢谢!\n");
				break;
			}
			if (check(name)) {
				System.out.println("您已购票，购票信息为: ");
				print_tickets(name);
			}
			else {
				System.out.println("请输入(F:一等舱/E:经济舱)选择舱位类型: ");
				input();
			}
			System.out.println(name + " 订票完毕.\n请继续输入需要订票的乘客姓名: ");
		}

		print_tickets("All");
	}

	public static boolean check(String name) {
		for (int i =0; i < tickets.size(); i++) {
			if (name.equals(tickets.get(i).get_passenger_name())) {
				return true;
			}
		}
		System.out.println("检查完毕，该乘客还未订票.");
		return false;
	}

	//负责输入，单独写成函数是因为非法输入的时候需要递归调用
	public static void input() {
		flag = 0;
		Scanner stdin = new Scanner(System.in);
		seat_type = stdin.nextLine();
		switch(seat_type) {
			case "F":
				if (surplusF > 0)
					System.out.println("可选座位号： ");
				print_seat("F");
				if (flag != 0)
					break;
				seat_number = stdin.nextInt();
				reservation(name, seat_number, "F");
				break;
			case "E":
				if (surplusE >0)
					System.out.printf("可选座位号：");
				print_seat("E");
				if (flag != 0) 
					break;
				seat_number = stdin.nextInt();
				reservation(name, seat_number, "E");
				break;
			default:
				print_error();
				input();
				break;
		}
	}

	public static void print_error() {
		System.out.println("输入无效! 请重新输入: \n");
	}

	//输出可选的座位号
	public static void print_seat(String seat_type) {
		int full = 0;
		for (int i=0; i<25; i++) {
			if (seats.get(i).get_vacancy() == false) {
				if (seat_type == "F") {
					if (surplusF == 0) {
						System.out.println("头等舱舱位全部定完，请重新选择舱位类型: ");
						full++;
						break;
					}
					System.out.printf(" %d", seats.get(i).get_number());
					if (i == 4)
						break;
				}
				if (seat_type == "E") {
					if (surplusE == 0) {
						System.out.println("经济舱舱位全部定完，请重新选择舱位类型: ");
						full++;
						break;
					}
					if (i > 4) {
						System.out.printf(" %d", seats.get(i).get_number());
					}
				}
			}
		}
		if (full == 0) {
			System.out.println("\n座位号: ");
		}
		else {
			input();
			flag++;
		}
	}

	//购票，即改变seats, tickets属性值
	public static void reservation(String name, int seat_number, String seat_type) {
		if (seat_number < 1 && seat_number > 20) {
			print_error();
		}
		else {
			switch (seat_type) {
				case "F":
					if (seat_number > 0 && seat_number < 6) {
						if (seats.get(seat_number - 1).get_vacancy() == false) {
							seats.get(seat_number - 1).set_vacancy(true);
							tickets.get(seat_number - 1).set_type("F");
							tickets.get(seat_number - 1).set_vacancy(true);
							tickets.get(seat_number - 1).set_passenger_name(name);
							surplusF--;
							System.out.printf("恭喜 %s 订票成功！\n", name);
							print_tickets(name);
							break;
						}
						else {
							print_error();
						}
					}
					else {
						print_error();
					}
					break;
				case "E":
					if (seat_number > 0 && seat_number <= 20) {
						if (seats.get(seat_number + 4).get_vacancy() == false) {
							seats.get(seat_number + 4).set_vacancy(true);
							tickets.get(seat_number + 4).set_type("E");
							tickets.get(seat_number + 4).set_vacancy(true);
							tickets.get(seat_number + 4).set_passenger_name(name);
							surplusE--;
							System.out.printf("恭喜 %s 订票成功！\n", name);
							print_tickets(name);
							break;
						}
						else {
							print_error();
						}
					}
					else {
						print_error();
					}
					break;
			}
		}
	}

	public static void print_tickets(String name) {
		System.out.println("\n======================================");
		System.out.println("        　　　 购票信息　　　　　　　　　　  ");
		System.out.println("======================================");
		System.out.printf("票号   票类型  　是否预定　　 姓 　名\n");
		for (int i=0; i<25; i++) {
			if (name.equals(tickets.get(i).get_passenger_name())) {
				if (i < 14) {	
					if (tickets.get(i).get_type() == "F"){
						System.out.printf(" F%d    %s     %s      %s\n", 
							tickets.get(i).get_number(), 
							"头等舱", 
							"已预定", 
							tickets.get(i).get_passenger_name());
					}
					else if(tickets.get(i).get_type() == "E") {
						System.out.printf(" E%d    %s     %s      %s\n", 
							tickets.get(i).get_number(), 
							"经济舱", 
							"已预定", 
							tickets.get(i).get_passenger_name());
					}
				}	
				else {
					System.out.printf(" E%d   %s     %s      %s\n", 
						tickets.get(i).get_number(), 
						"经济舱", 
						"已预定", 
						tickets.get(i).get_passenger_name());
				}
			}
			else if (name == "All") {		
				if (tickets.get(i).get_vacancy() == true) {
					if (i < 14) {	
						if (tickets.get(i).get_type() == "F"){
							System.out.printf(" F%d    %s     %s      %s\n", 
								tickets.get(i).get_number(), 
								"头等舱", 
								"已预定", 
								tickets.get(i).get_passenger_name());
						}
						else if(tickets.get(i).get_type() == "E") {
							System.out.printf(" E%d    %s     %s      %s\n", 
								tickets.get(i).get_number(), 
								"经济舱", 
								"已预定", 
								tickets.get(i).get_passenger_name());
						}
					}	
					else {
						System.out.printf(" E%d   %s     %s      %s\n", 
							tickets.get(i).get_number(), 
							"经济舱", 
							"已预定", 
							tickets.get(i).get_passenger_name());
					}
				}
				
			}
		}
		System.out.println("======================================\n");
	}	
}
