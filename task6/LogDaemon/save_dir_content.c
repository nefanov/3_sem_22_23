#include <dirent.h>
#include <LogDaemon.h>

int save_dir_content (const char *dirpath)
{
    char* dest_path[PATH_MAX] = {};
    strcpy(dest_path, DAEMON_DIR);
    char* dest_dir_end = NULL;
    dest_dir_end = strchr(dest_path, '\0');
    *dest_dir_end++ = '/';
    *dest_dir_end = '\0';

    char* src_path[PATH_MAX] = {};
    strcpy(src_path, dirpath);
    char* src_dir_end = NULL;
    src_dir_end = strchr(src_path, '\0');
    *src_dir_end++ = '/';
    *src_dir_end = '\0';

    puts(dest_path);
    puts(src_path);

    DIR *dirp;
    struct dirent *dp;

    dirp = opendir(dirpath);

    if (dirp == NULL) 
    {
        log("opendir failed on %s", dirpath);
        return ERROR;
    }

    for (;;) 
    {
        dp = readdir(dirp);

        if (dp == NULL)
            break;

        puts(dp->d_name);

        if (dp->d_type != DT_REG)
            continue;
        
        strcat(dest_path, dp->d_name);
        strcat(src_path, dp->d_name);
        cp(dest_path, src_path);

        puts(dest_path);
        puts(src_path);

        // Cut off names from full path
        *dest_dir_end = '\0';
        *src_dir_end = '\0';
    }

    return 0;
}