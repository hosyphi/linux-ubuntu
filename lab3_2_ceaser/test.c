#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/ctype.h>

int i,n;
static void ChuyenVi(char *XauRo,char *XauMa,int k){//En(x) = (x+n) mod 26
	//k = k % 26;
	n = strlen(XauRo);
	for(i = 0; i < n; i++)
	{
		if(isalpha(XauRo[i]))
		{
			if(isupper(XauRo[i]))// la chu hoa
			{
				XauMa[i] = ((((XauRo[i]-65)+k)%26)+65);
			}
			else if (islower(XauRo[i]))//la chu thuong
			{
				XauMa[i] = ((((XauRo[i]-97)+k)%26)+97);
			}
		}
		else 
		{
			XauMa[i] = XauRo[i];
		}
	}
	
}


//kmalloc: pha bo bo nho cho cac doi tuong nho hon trong kernel
//GFP_KERNEL: phan bo RAM binh thuong

static int __init init_test(void){
	char XauRo[15] = "Thuy Linh.";

	char *XauMaChuyenVi = (char *)kmalloc(15*sizeof(char),GFP_KERNEL);
	int k1 = 1;
	

    printk("\nXau Ro : %s",XauRo);
	printk(KERN_ALERT "\nThuc hien ma hoa chuyen vi:");
	ChuyenVi(XauRo,XauMaChuyenVi,k1);	
	printk("Xau Ma chuyen vi: %s\n",XauMaChuyenVi);
	return 0;
}

static void __exit exit_test(void){
	printk("Exit module");
	printk("\n");
}

module_init(init_test);
module_exit(exit_test);
