#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include </home/finkec/include/bsd/string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/cdefs.h>
#include <pwd.h>
#include <grp.h>
#include <locale.h>
#include <langinfo.h>
#include <unistd.h>
#define MAX 1024
#define TEST   printf( "HERE at line[%d]\n", __LINE__); // macro used for debugging only


// flags for checking if this argument is present
int a = 0;
int l = 0;
int c = 0;
int m = 0;
char cwd[1024];
char **dirs= NULL; // holds the file/directory names
char src[MAX];
char tmppath[MAX];
char *dest;
char **tmp2= NULL; // hold file names for sorting
int dirscount = 0; // counter for number of directories
int numentries= 0; // counter for the number of entries in a directory
struct dirent *dirp; // a pointer to a dirent struct
DIR *DIRdp; // pointer to a dir
struct group *grp;
struct stat statbuf; // holds stat info about a directory or file
int largestfilesize;

void alphabetize(int l);
void updatefsize();
/////////////////////////////////////////////////////////////////////////////////////////////////////
void listf() {
  getcwd(cwd, (size_t)sizeof(cwd)); //get pathname of the current directory
  int status = 0;
  strcpy(src, cwd);
  strcat(src, "/");

  int i = 0;
  if (1 == dirscount ) {
    dest = cwd;
  }

  else {
   i = 1; // skip over listf in dirs[]
  }

  for (; i < dirscount; i++ ) {

    dest = dirs[i];

    status = stat(dest, &statbuf); // does this file/directory even exist
    if (status == 0) {

      if (S_ISREG(statbuf.st_mode)) { // ok what is it?

        numentries = 1;
        printf("%d entry found\n", numentries);


        if (1==l) {

          // PRINTPERMISSIONS
          char bp[12]; // string to store file type and permissions
          strmode(statbuf.st_mode, bp); // formatting
          printf("%s", bp);  // permissions

          printf("%3d ", (int)statbuf.st_nlink );  // number of hard links

          uid_t uid = statbuf.st_uid;
          struct passwd *userid = getpwuid(uid);
          printf("%s ", userid-> pw_name); // print user ID of owner

          /* Print out group name if it is found using getgrgid(). */
          if ((grp = getgrgid(statbuf.st_gid)) != NULL) {
            printf("%s ",grp->gr_name);
          }
          else {
            printf("%d ",statbuf.st_gid);// print file owners group
          }

          printf("%6d ", (int)statbuf.st_size); // Print size of file.

          char tmbuf[26];
          strftime(tmbuf,26,"%x-%H:%M ", localtime(&statbuf.st_mtime));
          printf("M%s",tmbuf);

          if (a) {
            char tmbuf[26];
            strftime(tmbuf,26,"%x-%H:%M ", localtime(&statbuf.st_atime));
            printf("A%s",tmbuf);
          }
          if (c) {
            char tmbuf[26];
            strftime(tmbuf,26,"%x-%H:%M ", localtime(&statbuf.st_ctime));
            printf("C%s",tmbuf);
          }

          printf("%s\n", dest); // name of the file

        } // end of long print
        if (0==l) {
          printf("%s/n", dest); // name of the file
        }
      }

      if (S_ISDIR(statbuf.st_mode)) {

        DIRdp = opendir(dirs[i]); // open the directory

        while ((dirp = readdir(DIRdp)) != NULL) { // go through the dirent p entries within
          if (strcmp(dirp->d_name, "..") != 0  && strcmp(dirp->d_name, ".") != 0) {
            stat(dest, &statbuf); // get stats about each entry
            numentries++;
            updatefsize();
          }
        }

        closedir(DIRdp);

        printf("%d entries found\n", numentries);

        tmp2 = (char**)malloc(sizeof(char*)*(numentries+1));
        tmp2[numentries] = NULL;

        int k = 0;
        DIRdp = opendir(dirs[i]); // open the directory
        while ((dirp = readdir(DIRdp)) != NULL) {
          // add them to an array so we can later alphabetize
          if (strcmp(dirp->d_name, "..") != 0  && strcmp(dirp->d_name, ".") != 0){
            tmp2[k] = dirp->d_name;
            k++;
          }
        }
        closedir(DIRdp); // close the directory
        if (1==l) {
          // long print
          alphabetize(l);
        }
        else {
          alphabetize(0);
        }
      } // EOelseif
    } else {
      if (-1 == status) {
        printf("no such file or directory: %s", dirs[i]);
      }
    }
  }

} // EOlistf

///////////////////////////////////////////////////////////////////////////////
/*helper function for qsort()*/
int cmpfunc (const void * a, const void * b) {
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;


  return strcasecmp(*ia, *ib);
}
////////////////////////////////////////////////////////////////////////////////
void alphabetize (int el) {
  char *x;
  qsort (tmp2, numentries, (size_t)sizeof(x), cmpfunc);
  if (0==el){
    int q = 0;
    while (q < numentries) {
      printf("%s ", tmp2[q]);
      q++;
    }
  }
  if (1==el){
    int q = 0;
    while (q < numentries) {
      strcpy(tmppath, cwd);
      strcat(tmppath, "/");
      strcat(tmppath, dirs[el]);
      strcat(tmppath, "/");
      strcat(tmppath, tmp2[q]);


      stat(tmppath, &statbuf);

      char bp[12]; // string to store file type and permissions
      strmode(statbuf.st_mode, bp); // formatting
      printf("%s", bp);  // permissions

   printf("%3d ", (int)statbuf.st_nlink );  // number of hard links

   uid_t uid = statbuf.st_uid;
   struct passwd *userid = getpwuid(uid);
   printf("%s ", userid-> pw_name); // print user ID of owner

   /* Print out group name if it is found using getgrgid(). */
   if ((grp = getgrgid(statbuf.st_gid)) != NULL) {
     printf("%s ",grp->gr_name);
   }
   else {
     printf("%d ",statbuf.st_gid);// print file owners group
   }

    printf("%6d ", (int)statbuf.st_size); // Print size of file.

   char tmbuf[26];
   strftime(tmbuf,26,"%x-%H:%M ", localtime(&statbuf.st_mtime));
   printf("M%s",tmbuf);


   if (a) {
     char tmbuf[26];
     strftime(tmbuf,26,"%x-%H:%M ", localtime(&statbuf.st_atime));
     printf("A%s",tmbuf);
   }
   if (c) {
     char tmbuf[26];
     strftime(tmbuf,26,"%x-%H:%M ", localtime(&statbuf.st_ctime));
     printf("C%s",tmbuf);
   }
   printf("%s ", tmp2[q]);
   q++;
printf("\n");
 }// EOwhile
 printf("\n");
}
} // EOalphabetize
/////////////////////////////////////////////////////////////////////////////
void updatefsize() {
    // get largest filesize for proper display
    int tmpsize = (int)statbuf.st_size;
    if (tmpsize > largestfilesize) {
      largestfilesize = tmpsize;
    }
}

/////////////////////////////////////////////////////////////////////////
void setflags(char *argv[], int argc) {

    for (int i = 0; i < argc; i++) {
      if(argv[i][0] == '-') {  // check the first character at each arg[i]
        int size = strlen(argv[i]); // dynamically set size to length of each argument we are searching for
        for(int j = 1; j < size; j++) { // start at 1 bc first char is '-'

          switch(argv[i][j]) {  // set flag if character is present

            case 'l':
              l = 1;
              break;

            case 'a':
              a = 1;
              break;

            case 'm':
              m = 1;
              break;

            case 'c':
              c = 1;
              break;

            default:
              break;
          } // EOswitch
        } // EOfor
      } // EOif
      else{
        // add that word to the array of directories
        dirs[dirscount] = argv[i]; // dirs holds the string of directory/file arguments
        dirscount++;  // keep track of num directories/files
      } // EOelse
    } // EOfor
} // EOsetFlags
///////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {

  dirs = (char**)malloc(sizeof(char*)*(argc+1)); // dirs is now an array of pointers to hold pointers to tokens+1
  dirs[argc] = NULL; // NULL terminate the array of pointers
  setflags(argv, argc);
  listf();
  return (0);
}
