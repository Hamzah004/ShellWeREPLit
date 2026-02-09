#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
        // int id = fork();
        // if (id == 0)
        // {
                chdir("/home/amufleh/abdallah");
                int fd = open("file_in_anoter_dir.txt",O_CREAT | O_WRONLY , 0644);
                write(fd,"abdallah in another dir",8);
        // }
       //  else
        // {
          //       int fd = open("file_in_the_main_dir.txt",O_CREAT | O_WRONLY, 0644);
            //     write(fd,"abdallah but in the main dir",28);
        // }
}

