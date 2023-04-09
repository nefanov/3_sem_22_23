#include "daemon.h"

enum Mods getCMDargs(int argc, char* argv[], pid_t* pid)
{
    int mod = BASH;

    switch (argc)
    {
    case 3:
        if (!strcmp(argv[1], "-b"))
        {
            mod = BASH;
        }
        else if(!strcmp(argv[1], "-d"))
        {
            mod = DAEMON;
        }
        else
        {
            fprintf(stderr, "Wrong Mod:\n\n");
            showExample();
            exit(EXIT_ERROR);
        }

        *pid = atoi(argv[2]);
        if (*pid == 0)
        {
            fprintf(stderr, "Wrong PID:\n\n");
            showExample();
            exit(EXIT_ERROR);
        }
        return mod;
        break;

    case 2:
        *pid = atoi(argv[1]);
        if (*pid == 0)
        {
            fprintf(stderr, "Wrong PID:\n\n");
            showExample();
            exit(EXIT_ERROR);
        }
        break;
    
    default:
        fprintf(stderr, "Wrong arguments:\n\n");
        showExample();
        exit(1);
        break;
    }

    return BASH;
}


char* getCurDir(pid_t pid, char* curDir)
{
    char proc_dir_path[PATH_MAX_SIZE] = {};

    sprintf(proc_dir_path, "/proc/%d/cwd", pid);

    if(readlink(proc_dir_path, curDir, PATH_MAX_SIZE) == -1)
    {
        perror("readlink:");
        LOG("readlink:");
        return NULL;
    }

    return curDir;
}

int cp(const char *to, const char *from)
{
    int fd_to, fd_from;
    char buf[4096];
    ssize_t nread;
    int saved_errno;

    fd_from = open(from, O_RDONLY);
    if (fd_from < 0)
        return -1;

    fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd_to < 0)
        goto out_error;

    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        close(fd_from);

        return 0;
    }

out_error:
    saved_errno = errno;

    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);

    errno = saved_errno;
    return -1;
}

int createDir(const char* dir_name)
{
    mode_t old_umask = umask(0);

    int ret = mkdir(dir_name, 0777);

    if (ret == ERROR && errno != EEXIST)
    {
        perror("mkdir:");
        LOG("mkdir:");
        return ERROR;
    }

    umask(old_umask);

    return ret;
}

int saveDirData(const char* curDir)
{
    char* destPath[PATH_MAX_SIZE] = {};
    strcpy(destPath, DAEMON_DIR);
    char* destDirEnd = strchr(destPath,'\0');
    *destDirEnd++ = '/';
    *destDirEnd   = '\0';

    char srcPath[PATH_MAX_SIZE] = {};
    strcpy(srcPath, curDir);
    char* srcDirEnd = strchr(srcPath, '\0');
    *srcDirEnd++ = '/';
    *srcDirEnd = '\0';

    DIR *dirp;
    struct dirent* sdir;

    dirp = opendir(curDir);

    if (dirp == NULL) 
    {
        LOG("opendir failed in PATH\n");
        return -1;
    }

    for (;;) 
    {
        sdir = readdir(dirp);

        if (sdir == NULL)
            break;

        if (sdir->d_type != DT_REG)
            continue;
        
        strcat(destPath, sdir->d_name);
        strcat(srcPath,  sdir->d_name);
        cp(destPath, srcPath);

        *destDirEnd = '\0';
        *srcDirEnd  = '\0';
    }

    closedir(dirp);
    
    return 0;
}