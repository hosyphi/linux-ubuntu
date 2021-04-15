#include <linux/init.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/ctype.h>

int Count(char *a);
char *Reverse_TheoDoDai(char *a);
int IsWhiteSpace(char c) ;
int SpecialCharacter(char c);
int CapitalSignal(char c) ;
void RemoveAtTail(char *s) ;
void RemoveAtHead(char *s);
void RemoveAtMid(char *s) ;
void AddWhiteSpace(char *s, char *res, int amount) ;
void Capitalize(char* s) ;
int CountPos(char* s);
void NormalizeString(char *s);
void appendString(char *str1, char *str2);

static int lab2_2_init(void)
{
    char a[20] = "i love you,";
    char b[20] = "to yeu cau";
    char test_string[50] = "   nguyen   nhay  lau. Ban cu thu   coi        ";
    pr_info("\ncount(%s) = %d", a, Count(a));
    
    pr_info( "\nxau dao nguoc theo do dai cua %s----> %s", b, Reverse_TheoDoDai(b));
    
    NormalizeString(test_string);

    appendString(a,b);
    
    return 0;
}

int Count(char *a)
{
	int i = 0;
	while (*(a+i) != '\0')
		i++;
	return i;
}


char *Reverse_TheoDoDai(char *a)
{
    int i, leght = Count(a);
    char  *reverse = (char*) kmalloc(leght*sizeof(char),GFP_KERNEL);
  
    for ( i = 0; i < leght; i++) 
    {
        *(reverse+i) = *(a+leght -1-i);
    }
    return reverse;
}

int IsWhiteSpace(char c) 
{ //Kiểm tra kí tự cách
    if(c == ' ' || c == '\t' || c == '\n') return 1;
    return 0;
}

int SpecialCharacter(char c) 
{ //Kiểm tra kí tự đặc biệt
    if(c == ',' || c == '.' || c == '(' || c == ')'
    || c == '?' || c == ':' || c == ';' || c == '!') return 1;
    return 0;
}

int CapitalSignal(char c) 
{ //Kiểm tra kí tự cần in hoa ở sau
    if(c == '.' || c == '!' || c == '?') return 1;
    return 0;
}

void RemoveAtTail(char *s) {
	int len = strlen(s); //yêu cầu thư viện <linux/string.h> nếu muốn sử dụng hàm strlen
	int i = len-1;
	while(IsWhiteSpace(s[i])) {
		s[i] = '\0';
		i--;
	}
}


void RemoveAtHead(char *s) {
	int counter = 0;
	int i;
	int len = strlen(s);
	// Đếm xem có tất cả bao nhiêu khoảng trắng
	for(i = 0; i < len; i++) {
		if(IsWhiteSpace(s[i])) {
			counter++;
		} else {
			break;
		}
	}
	// Xoá khoảng trắng
	for(i = 0; i <= len - counter; i++) {
		s[i] = s[i + counter];
	}
}

void RemoveAtMid(char *s) 
{
	int i, j;
	int len = strlen(s);
	for(i = 0; i < len; i++) {
		if(IsWhiteSpace(s[i]) && IsWhiteSpace(s[i + 1])) {
			for(j = i + 1; j < len; j++) {
				s[j] = s[j + 1];
			}
			i--;
			len--;
		} else if(IsWhiteSpace(s[i]) && SpecialCharacter(s[i + 1])) {
			for(j = i; j < len; j++) {
				s[j] = s[j + 1];
			}
			i--;
			len--;
		} else if(s[i] == '\t') {
			s[i] = ' ';
		}
	}
}


void AddWhiteSpace(char *s, char *res, int amount) {
	int i, j = 0;
	int len = strlen(s);
	for(i = 0; i < len; i++) {
		res[j++] = s[i];
		if(SpecialCharacter(s[i]) && !SpecialCharacter(s[i + 1])
		&& !IsWhiteSpace(s[i + 1])) {
			res[j++] = ' ';
		}
	}
	res[len + amount] = '\0';
}

void Capitalize(char* s) 
{
	int i;
	int len = strlen(s);
	if(len > 0) {
		s[i] = toupper(s[i]); //Nếu bị lỗi ở đây thì ta đang thiếu thư viện <ctype.h>
	}
	for(i = 0; i < len; i++) {
		if(CapitalSignal(s[i])) {
			s[i + 2] = toupper(s[i + 2]); 
		}
	}
}

int CountPos(char* s)
{
    int counter = 0;
    int i;
    int len = strlen(s);
    for(i = 0; i < len; i++) {
        if(SpecialCharacter(s[i]) && !SpecialCharacter(s[i + 1])
        && !IsWhiteSpace(s[i + 1])) {
            counter++;
        }
    }
    
    return counter;
}

void NormalizeString(char *s)
{
    RemoveAtTail(s);
    RemoveAtHead(s);
    RemoveAtMid(s);
    int num = CountPos(s);
    int len = strlen(s);
    char *ns = (char *) kmalloc(sizeof(char)*(len + num + 1),GFP_KERNEL);
    AddWhiteSpace(s, ns, num);
    Capitalize(ns);
    pr_info("Xau sau khi chuan hoa cua (%s)----> %s",s, ns);
}
	
void appendString(char *str1, char *str2)
{
	int i;
	int count1 = Count(str1);
	int count2 = Count(str2);
	int count = count1+count2;
	char *val = (char *)kmalloc(count*sizeof(char),GFP_KERNEL);
	for(i=0;i<count;i++){
		if(i<count1){
			*(val+i) = *(str1+i);
		}else{
			*(val+i)=*(str2+(i-count1));
		}
	}
	pr_info( "\nNoi hai xau %s va %s ----> %s",str1,str2,val);
}


static void lab2_2_exit(void)
{
    pr_info(KERN_ALERT "Ket thuc \n");
}

module_init(lab2_2_init);
module_exit(lab2_2_exit);
