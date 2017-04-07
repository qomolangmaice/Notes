#include <linux/fs.h>   
#include <linux/iobuf.h>   
//#include <linux/major.h>      
#include <linux/blkdev.h>   
#include <linux/capability.h>   
#include <linux/smp_lock.h>   
#include <asm/io.h>   
#include <asm/uaccess.h>   
#include <linux/module.h>   
#include <linux/delay.h>   
#define RS485DE      GPIO_G9     //RED   
   
#define nREV      0   
#define nSENT     1   
#define RS485SENTBEGIN  1   
#define RS485SENTOVER   2   
#define LAST_BYTES_FIFO_TO_SHIFTER 1   
static int RS485_MAJOR=0;   
static devfs_handle_t devfs_handle;   
//static struct timer_list rs485_timer;   
static int delay_10ms_count;   
int    rs485_open(struct inode*, struct file *);   
int    rs485_colse(struct inode*, struct file *);   
int    rs485_ioctl(struct inode*inode,struct file *flip,unsigned int cmd,unsigned long arg);   
//#define DEBUG_WG   
#ifdef DEBUG_WG  
#define PRIN_DEBUG printk  
#else  
#define PRIN_DEBUG  
#endif   
   
struct rs485de_ctl   
{   
    long BaudRate;   
    int parity;   
    int dataBits;   
    int stopBits;      
    long count;   
};   
   
struct rs485de_ctl *dep;   
   
static struct file_operations gpio_ctl_fops= {    
    ioctl:       rs485_ioctl,   
    open:        rs485_open,   
    release:     rs485_colse,   
};   
   
int __init gpio_init(void)    
{    
    int result;    
    result = register_chrdev(0, "rs485driver", &gpio_ctl_fops);    
    if (result < 0) {    
        printk("rs485de register error!\n");    
        return result;    
    }    
    RS485_MAJOR=result;   
    devfs_handle = devfs_register(NULL, "rs485driver", DEVFS_FL_DEFAULT,RS485_MAJOR, 0, S_IFCHR | S_IRUSR | S_IWUSR,&gpio_ctl_fops, NULL);   
    return 0;    
}    
   
   
int gpio_release(struct inode*inode, struct file *filp)   
{    
	devfs_unregister(devfs_handle);   
    unregister_chrdev(RS485_MAJOR, "rs485driver");    
}   
   
int    rs485_open(struct inode*inode, struct file *filp)   
{   
        
	set_gpio_ctrl(GPIO_MODE_OUT | RS485DE);    
	set_gpio_pullup(RS485DE);   
	write_gpio_bit(GPIO_MODE_OUT | RS485DE,nREV);   
    return 0;   
}   
   
   
int    rs485_colse(struct inode*inode, struct file *filp)   
{    
    return 0;   
}   
   
void set_rs485de_sent(void)   
{   
    write_gpio_bit(GPIO_MODE_OUT | RS485DE,nSENT);   
    mdelay(1);   
}   
   
void set_rs485de_receive(void)   
{   
    //udelay(9000);   
    write_gpio_bit(GPIO_MODE_OUT | RS485DE,nREV);   
}   
   
   
void read_UFSTAT1_and_clr_de()   
{   
    unsigned int tmp_data,i;   
    unsigned int delay_time_us;   
    delay_time_us=(1000000/dep->BaudRate)+1;   
    delay_time_us=delay_time_us*(dep->parity+dep->stopBits+dep->dataBits)*(LAST_BYTES_FIFO_TO_SHIFTER);   
    PRIN_DEBUG("delay time is %dus\n",delay_time_us);      
    for(i=0;i<10;i++)   
    {   
        tmp_data=UFSTAT1;   
        while((tmp_data&0x3f00)!=0)   
            {   
                udelay(10);   
                tmp_data=UFSTAT1;   
            }   
    }   
        for(i=0;i<delay_time_us;i++)   
            udelay(1);   
    set_rs485de_receive();     
}   
   
int rs485_ioctl(struct inode*inode,struct file *flip,unsigned int cmd,unsigned long arg)   
{   
   
    int ret=0;   
  switch(cmd)   
  {   
    case RS485SENTBEGIN:   
          set_rs485de_sent();   
          break;   
    case RS485SENTOVER:   
          dep=kmalloc(sizeof(*dep),GFP_KERNEL);   
          copy_from_user(dep,(struct rs485de_ctl *)arg,sizeof(struct rs485de_ctl));   
          PRIN_DEBUG("baudrate is %d \n",dep->BaudRate);   
          PRIN_DEBUG("stop is %d \n",dep->stopBits);   
          PRIN_DEBUG("parity is %d \n",dep->parity);   
          PRIN_DEBUG("data is %d \n",dep->dataBits);   
          PRIN_DEBUG("data is %d \n",dep->count);   
          read_UFSTAT1_and_clr_de();   
          kfree(dep);   
          break;   
    default:   
          ret=-1;   
          break;   
  }         
    return ret;   
}   
   
module_init(gpio_init);   
module_exit(gpio_release);   
