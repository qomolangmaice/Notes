Java实现一个简单航空购票程序
===========================

一、航空购票程序描述
-------------------

假设头等舱5个座位，编号为1~5，经济舱20个座位，编号为1~20

具体要求如下：

当程序开始时，提示输入乘客姓名，一个名字只能订一个座位，如果该名字已经订过座位了，则提示该乘客座位已定及座位号，否则提示输入舱位类型；

当输入舱位类型后，如果该舱位已全部订出，则提示此舱全部订完，重新选择舱位类型；否则显示可以选择的座位号，注意已经被订出的座位号不能出现，提示输入座位号；

舱位类型：只能输入”F”或”E”，一等舱为F，经济舱为E，如果输入其它字符则提示输入无效。

座位号：只能输入前面列出可以选择的座位号，若输入其它则提示输入无效。

程序可以为多个乘客订票，直到输入名字为”.”;

最后程序按照舱位类型与座位号排序打印出所有订票信息（座位号及对应乘客姓名）.

二、流程图
---------

请参照我写文章: http://wangkeguan.com/?p=1254

三、类抽象
---------

根据上面的程序描述，我们可以提取出两个类：座位Seat和票Ticket.
座位(Seat)有3个基本属性：座位类型（F/E），座位编号，座位预定状态（是否已经被预定）.
票(Ticket)其实也具有和座位相同的属性，也包含座位的各个信息，并且还增加持票人的姓名这个属性，所以”票”这个类继承自“座位”类。
确定好了“座位”类和“票”类的属性之后，咱们需要确定设置这些属性的方法，其实也就是各个属性的set方法和get方法。
类抽象1
Seat类

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

Ticket类

	class Ticket extends Seat {
		public String passenger_name;
	 
		public void set_passenger_name(String passenger_name) {
			this.passenger_name = passenger_name;
		}
		public String get_passenger_name() {
			return passenger_name;
		}
	}

四、全局变量
-----------

由于描述中给定总共25个座位，意味着25张票，则将它们对应的舱位类型的数量用static关键字声明成全局变量，用于统计订票过程。还有Seat类和Ticket类以及各子函数的共享变量。

	//定义全局变量舱位，票，各类舱位剩余数，便于其他函数全局访问
	public static int surplusF = 5;　　　　//头等舱座位
	public static int surplusE = 20;　　　　//经济舱座位
	public static int flag;
	public static String seat_type;
	public static int seat_number;
	public static String name = "";

五、数据结构
-----------

这里咱们选用ArrayList的泛型来存储我们的Seat类实例和Ticket类实例，这个过程优点类似于C语言中的结构体，不过在面向对象的编程语言中，咱们不用考虑如何ArrayList的实现细节，只需要知道怎么使用就行了。

	public static ArrayList<Seat> seats = new ArrayList<Seat>();
	public static ArrayList<Ticket> tickets = new ArrayList<Ticket>();

定义好ArrayList后，咱们再新建Seat类实例，并初始化Seat类实例中属性后，通过使用ArrayList的add()方法将Seat类实例添加到ArrayList中，当咱们需要调用存储在ArrayList中的Seat类实例中数据时，可以使用ArrayList的get(i)方法来取Seat类实例的数据。

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
		}

六、函数
--------

我们需要将一些输出输入的操作提取成函数，简化我们的代码，这里之所以将输入提取成input()函数，是因为我们需要不断输入，即使出错，也可以重新输入，所以为了便于递归调用就封装成函数，当然还是使用static关键字；

	//检测是否已购票
	public static boolean check(String name) {}
	　　　　　
	//用于执行输入
	public static void input() {}
	 
	//显示输入不合法
	public static void print_error(){}
	 
	//显示剩余座位
	public static void print_seat(String seat_type){}
	 
	//购票
	public static void reservation(String name, int seat_number, String seat_type){}
	 
	//显示已购票信息
	public static void print_tickets(String name){}

七、主体逻辑
-----------

main函数非常简单，首先还是需要将座位和票信息初始化，即将它们编号，预定状态全为“未预定”(空)，然后执行输入，一个while循环，如果不符合条件就直接跳出while，执行print_tickets(“all”)打印出所有的订票信息.

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
