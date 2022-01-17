#ifndef __INC_H_
#define __INC_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sqlite3.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define REG_STAFF "1"
#define REG_ADMIN "2"
#define LOG_STAFF "3"
#define LOG_ADMIN "4"
#define CHECK_SELF "5"
#define CHANGE_SELF "6"
#define CHECK_OTHER "7"
#define CHANGE_OTHER "8"
#define DELATE_OTHER "9"

#define ERRMSG(msg) do{\
	printf("__%d__\n",__LINE__);\
	perror(msg);\
}while(0)

typedef struct {
	sqlite3 *efpd;
	int newfd;
}parameter;

typedef struct {
	char name[24];
	char phone[24];
	char passwd[24];
	char addr[48];
	char salary[12];
	char age[8];
	char type[4];
	char power[4];
}message;

//ser.c
int create_socket_ser(int *sockfd,const char *argv[]);
int create_socket_cil(int *sockfd,const char *argv[]);
int do_register_staff_ser(char *inform,parameter *ptr,int power);
int do_login_staff_ser(char *inform,parameter *ptr,int power);
int check_self_ser(char *inform,parameter *ptr);
int send_message_ser(parameter *ptr,message *msg);
int make_message(message *msg,char **presult);
int change_self_ser(int *sockfd,char *inform,parameter *ptr);
int update_message_ser(message *newmsg,message *msg);
int change_message_ser(message *msg,parameter *ptr,message *oldmsg);
int do_delete(int *sockfd,char *inform,parameter *ptr);

//cil.c
int do_register_staff_cil(int *sockfd,int power);
int do_login_staff_cil(int *sockfd,int power);

//form.c
int import_form(sqlite3 **efpd);
void *myrecv(void *arg);


//view.c
int view_1(int *sockfd);
int view_staff(int *sockfd,char *phone);
int view_admin(int *sockfd,char *phone);
int dispaly_message(message *msg);
int view_change_staff(int *sockfd,char *phone);
int view_change_admin(int *sockfd,char *phone);
int dispaly_message_admin(message *msg);


#endif
