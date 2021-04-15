#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

int i;
static void ChuyenVi(char *XauRo,char *XauMa,int k){//En(x) = (x+n) mod 26
	i=0;
	while(XauRo[i]!='\0'){
		if(XauRo[i] >= 97 && XauRo[i] <=122)
		{
			XauMa[i] = ((((XauRo[i]-97)+k)%26)+97);
		}
		else if(XauRo[i] >= 65 && XauRo[i] <=90)
		{
			XauMa[i] = ((((XauRo[i]-65)+k)%26)+65);
		}
		else 
		{
			XauMa[i] = XauRo[i];
		}
		i++;
	}
}

static void ThayThe(char *XauRo,char *XauMa,char *k){
	i=0;
	while(XauRo[i]!='\0'){
		if(XauRo[i]==' '){
			XauMa[i]=' ';
		}
		else{
			if(XauRo[i]>=97){
				XauMa[i] = *(k+(XauRo[i]-97));
			}else{
				XauMa[i] = *(k+(XauRo[i]-65));
			}
		}
		i++;
	}
}

static void HoanViToanCuc(char *XauRo,char *XauMa,char *k){
	i=0;
	while(XauRo[i]!='\0'){
		XauMa[i] = XauRo[i]^k[i%4];
		i++;
	}
}


//kmalloc: pha bo bo nho cho cac doi tuong nho hon trong kernel
//GFP_KERNEL: phan bo RAM binh thuong

static int __init init_lab(void){
	char XauRo[15] = "Thuy Linh";

	char *XauMaChuyenVi = (char *)kmalloc(15*sizeof(char),GFP_KERNEL);
	int k1 = 1;
	
	char *XauMaThayThe = (char *)kmalloc(15*sizeof(char),GFP_KERNEL);
	char *k2 = "ZPBYJRSKFLXQNWVDHMGUTOIAEC";
	
	char *XauMaHoanViToanCuc = (char *)kmalloc(15*sizeof(char),GFP_KERNEL);
	char *k3 = "CT2C";

    printk("\nXau Ro : %s",XauRo);
	printk(KERN_ALERT "\nThuc hien ma hoa chuyen vi:");
	ChuyenVi(XauRo,XauMaChuyenVi,k1);	
	printk("Xau Ma chuyen vi: %s\n",XauMaChuyenVi);

	printk(KERN_ALERT "\nThuc hien ma hoa thay the");
	ThayThe(XauRo,XauMaThayThe,k2);
	printk("Xau Ma thay the: %s\n",XauMaThayThe);

	printk(KERN_ALERT "\nThuc hien ma hoa hoan vi toan cuc");	
	HoanViToanCuc(XauRo,XauMaHoanViToanCuc,k3);
	printk("Xau Ma hoan vi: %s\n",XauMaHoanViToanCuc);
	return 0;
}

static void __exit exit_lab(void){
	printk("Exit module");
	printk("\n");
}

module_init(init_lab);
module_exit(exit_lab);
