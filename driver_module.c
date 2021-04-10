#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

#define MEM_SIZE 1024

char *kernel_buffer;
char XauRo[MEM_SIZE], XauMa[MEM_SIZE], XauGiaiMa[MEM_SIZE];
int i,len, key, choice;
char x;

struct vchar_drv
{
	dev_t dev_num;
	struct class *dev_class;
	struct device *dev;
	struct cdev *vcdev;
}driver_module;

static int my_open(struct inode *inode, struct file *filp);
static int my_release(struct inode *inode, struct file *filp);

static ssize_t my_read(struct file *filp, char __user *user_buf,size_t len, loff_t *off);
static ssize_t my_write(struct file *filp, const char __user *user_buf, size_t len, loff_t *off);

void MaHoa(char XauRo[MEM_SIZE],char XauMa[MEM_SIZE], int key);
void GiaiMa(char XauMa[MEM_SIZE],char XauGiaiMa[MEM_SIZE], int key);

static struct file_operations fops = {
	.owner  = THIS_MODULE,
	.read   = my_read,
	.write  = my_write,
	.open   = my_open,
	.release= my_release,
};

int my_open(struct inode *inode, struct file *filp)
{
	pr_info("Driver: open\n");
	return 0;
}

int my_release(struct inode *inode, struct file *filp)
{
	pr_info("Driver: close\n");
	return 0;
}

ssize_t my_read(struct file *filp, char __user *user_buf, size_t len, loff_t *off)
{
	if(choice == 2) {
		copy_to_user(user_buf, XauMa, MEM_SIZE);
	}
	if(choice == 3 )
	{
		copy_to_user(user_buf, XauGiaiMa, MEM_SIZE);
	}
	return MEM_SIZE;
}

ssize_t my_write(struct file *filp, const char __user *user_buf, size_t len, loff_t *off)
{	
	copy_from_user(kernel_buffer, user_buf, len);
	choice = *kernel_buffer;
	if(choice == 1)
	{
		i = 0;
		kernel_buffer++;
		while(*kernel_buffer != '\0') {
			XauRo[i++] = *kernel_buffer;
			kernel_buffer++;
		}
		XauRo[i] = '\0';
	}
	
	if(choice == 2)//encrypt
	{
		kernel_buffer++;
		key = *kernel_buffer;
		key = key - 22;
		MaHoa(XauRo,XauMa,key);
	}
	if(choice == 3)//decrypt
	{
		kernel_buffer++;
		key = *kernel_buffer;
		key = key - 22;
		GiaiMa(XauMa,XauGiaiMa,key);
	}
	return len;
}

void MaHoa(char XauRo[MEM_SIZE],char XauMa[MEM_SIZE], int key)
{
	len = strlen(XauRo);// do dai xau
	for(i = 0; i < len; i++)
	{
		if(isalpha(XauRo[i]))//check chu
		{
			if(isupper(XauRo[i]))// la chu hoa
			{
				XauMa[i] = ((((XauRo[i] - 65) + key) % 26) + 65);
			}
			else if (islower(XauRo[i]))//la chu thuong
			{
				XauMa[i] = ((((XauRo[i] - 97) + key) % 26) + 97);
			}
		}
		else 
		{
			XauMa[i] = XauRo[i];
		}
	}
}
void GiaiMa(char XauMa[MEM_SIZE],char XauGiaiMa[MEM_SIZE], int key)
{
	len = strlen(XauMa);// do dai xau
	for(i = 0; i < len; i++)
	{
		if(isalpha(XauMa[i]))//check chu
		{
			if(isupper(XauMa[i]))// la chu hoa
			{
				XauGiaiMa[i] = (((((XauMa[i] - 65) + 26) - key) % 26) + 65);
			}
			else if (islower(XauRo[i]))//la chu thuong
			{
				XauGiaiMa[i] = (((((XauMa[i] - 97) + 26) - key) % 26) + 97);
			}
		}
		else 
		{
			XauGiaiMa[i] = XauMa[i];
		}
	}
}

static int driver_module_init(void)
{
	pr_info("Bat dau driver_module\n");
	alloc_chrdev_region(&driver_module.dev_num,0,1,"driver_module");
	driver_module.dev_class = class_create(THIS_MODULE, "driver_module_class");
	driver_module.dev = device_create(driver_module.dev_class, NULL, driver_module.dev_num, NULL, "driver_module_device");
	kernel_buffer = kmalloc(MEM_SIZE, GFP_KERNEL);
	driver_module.vcdev = cdev_alloc();
	cdev_init(driver_module.vcdev, &fops);
	cdev_add(driver_module.vcdev, driver_module.dev_num,1);
	return 0;
}

static void driver_module_exit(void)
{
	cdev_del(driver_module.vcdev);
	kfree(kernel_buffer);
	device_destroy(driver_module.dev_class, driver_module.dev_num);
	class_destroy(driver_module.dev_class);
	pr_info("ket thu driver_module \n");
}

module_init(driver_module_init);
module_exit(driver_module_exit)

MODULE_LICENSE("GPL");

