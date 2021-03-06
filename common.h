#ifndef _COMMON_
#define _COMMON_
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <netinet/in.h>
#include <time.h>
#include <dirent.h>

#ifndef BSIZE
  #define BSIZE 1024
#endif

typedef struct User
{
  char username[32];
  char password[32];
  char root[BSIZE];
} User;

typedef struct Users
{
  User *users;
  int count;
} Users;

typedef struct State
{
  /* Connection mode: NORMAL, SERVER, CLIENT */
  int mode;

  /* Is user loggd in? */
  int logged_in;

  /* Is this username allowed? */
  int username_ok;
  User user;
  
  /* Response message to client e.g. 220 Welcome */
  char *message;

  /* Commander connection */
  int connection;

  /* Socket for passive connection (must be accepted later) */
  int sock_pasv;

  /* Transfer process id */
  int tr_pid;

  /* Rename buffer */
  char *rename;
} State;


/* Command struct */
typedef struct Command
{
  char command[5];
  char arg[BSIZE];
  char arg1[BSIZE];
} Command;

/**
 * Connection mode
 * NORMAL - normal connection mode, nothing to transfer
 * SERVER - passive connection (PASV command), server listens
 * CLIENT - server connects to client (PORT command)
 */
typedef enum conn_mode{ NORMAL, SERVER, CLIENT }conn_mode;

/* Commands enumeration */
typedef enum cmdlist 
{ 
  ABOR, CWD, DELE, LIST, MDTM, MKD, NLST, PASS, PASV,
  PORT, PWD, QUIT, RETR, RMD, RNFR, RNTO, SITE, SIZE,
  STOR, TYPE, USER, NOOP,
  APPE, CDUP, HELP,
} cmdlist;

/* String mappings for cmdlist */
static const char *cmdlist_str[] = 
{
  "ABOR", "CWD", "DELE", "LIST", "MDTM", "MKD", "NLST", "PASS", "PASV",
  "PORT", "PWD", "QUIT", "RETR", "RMD", "RNFR", "RNTO", "SITE", "SIZE",
  "STOR", "TYPE", "USER", "NOOP",
  "APPE", "CDUP", "HELP",
};

/* Site argument enumeration */
typedef enum arglist 
{ 
  CHMOD, GROUP, EXEC, GROUPS,
  IDLE, GPASS, ALIAS, CHECKSUM
} arg1list;

/* String mappings for arglist */
static const char *arglist_str[] = 
{
  "CHMOD", "GROUP", "EXEC", "GROUPS",
  "IDLE", "GPASS", "ALIAS", "CHECKSUM"
};

/* Welcome message */
static char *welcome_message = "Fork of the infamous wu-ftpd https://github.com/Crewdolfos/woot-ftpd/\n"
                               "(Version woot-2.5.0(1) Tue Sep 21 16:48:12 EDT 1999)";

/* Server functions */
int gen_port();
void parse_command(char *, Command *);
int create_socket(int port);
void write_state(State *);
int accept_connection(int);
char* getLocalPath(char*,const char*);
char* getFtpPath(char*, const char*);

/* Commands handle functions*/
void response(Command *, State *);
void ftp_user(Command *, State *);
void ftp_pass(Command *, State *);
void ftp_pwd(Command *, State *);
void ftp_cwd(Command *, State *);
void ftp_mkd(Command *, State *);
void ftp_rmd(Command *, State *);
void ftp_pasv(Command *, State *);
void ftp_list(Command *, State *);
void ftp_retr(Command *, State *);
void ftp_stor(Command *, State *);
void ftp_appe(Command *, State *);
void ftp_dele(Command *, State *);
void ftp_site(Command *, State *);
void ftp_size(Command *, State *);
void ftp_quit(State *);
void ftp_type(Command *, State *);
void ftp_abor(State *);
void ftp_nlst(Command *, State *);
void ftp_rnfr(Command *, State *); 
void ftp_rnto(Command *, State *);
void ftp_cdup(State *);
void ftp_help(Command *, State *);

void str_perm(int, char *);
void my_wait(int);

Users users;
#endif
