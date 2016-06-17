#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>

#define CtrlReg_start (*(volatile char*)(0x4000000c))
#define CtrlReg_size (*(volatile char*)(0x40000008))
#define CtrlReg_target (*(volatile char*)(0x40000004))
#define CtrlReg_source (*(volatile char*)(0x40000000))
#define CLRReg (*(volatile char*)(0x40000010))
static char buf_dev[512]="driver_data";

static char source[10]="source";
static char target[10]="target";
static char size[10]="size";
static char start[10]="start";
const int irq_num=9;
char read_start(void) {
return CtrlReg_start;
}
void write_start(char data) {
CtrlReg_start = data;
}

char read_size(void) {
return CtrlReg_size;
}
void write_size(char data) {
CtrlReg_size = data;
}

char read_target(void) {
return CtrlReg_target;
}
void write_target(char data) {
CtrlReg_target = data;
}

char read_source(void) {
return CtrlReg_source;
}
void write_source(char data) {
CtrlReg_source = data;
}

char read_CLR(void) {
return CLRReg;
}
void write_CLR(char data) {
CLRReg = data;
}

irqreturn_t handler (int irq, void *dev_id)
{
	write_CLR('1');
	return (IRQ_HANDLED);
}
static ssize_t drv_read(struct file *filp, char *buf, size_t count, loff_t *ppos)
{
	copy_to_user(buf,buf_dev,count);

	printk("device read\n");
	return count;
}

static ssize_t drv_write(struct file *filp, const char *buf, size_t count, loff_t *ppos)
{
	write_CLR('0');
	copy_from_user(buf_dev,buf,count);
	
	sscanf(buf,"%s %s %s %s",source,target,size,start);
	write_source(*source);
	write_target(*target);
	write_size(*size);
	write_start(*start);
	printk("device write\n");
	printk("source = %s \n",source);	
	printk("target = %s \n",target);	
	printk("size = %s \n",size);
	printk("start = %s \n",start);
	return count;
}

static int drv_open(struct inode *inode, struct file *filp)
{
	write_CLR('1');
	if (!request_irq (irq_num, handler,0x00000020, "dma", NULL)) {
		printk("irq request failed\n");
	}	
	printk("device open\n");
	return 0;
}

/*int drv_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk("device ioctl\n");
	return 0;
}*/

static int drv_release(struct inode *inode, struct file *filp)
{
	printk("device close\n");
	return 0;
}

struct file_operations        drv_fops =
{
	read:               drv_read,
	write:              drv_write,
	//ioctl:              drv_ioctl,
	open:               drv_open,
	release:            drv_release,
};

#define MAJOR_NUM             60
#define MODULE_NAME           "dma"

static int dma_init(void) {
	if (register_chrdev(MAJOR_NUM, "dma", &drv_fops) < 0) {
		printk("<1>%s: can't get major %d\n", MODULE_NAME, MAJOR_NUM); return (-EBUSY);
	}

printk("<1>%s: started\n", MODULE_NAME);
return 0;

}

static void dma_exit(void) {
	unregister_chrdev(MAJOR_NUM, "dma"); printk("<1>%s: removed\n", MODULE_NAME);
}

module_init(dma_init);
module_exit(dma_exit);
