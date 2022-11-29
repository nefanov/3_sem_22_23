#include "../headers/ss_start.h"

static FILE* maps_file;

int start_service(pid_t tr_pid) 
{
    // ----- STARTING SERVICE -----
    create_log("Starting service...\n");
    
    create_log("Tracking pid: %d", tr_pid);
    
    if (open_maps_file(tr_pid) == -1) 
    {
        return -1;
    }
    
    // ----- Parse /proc/${PID}/maps for the first time -----
    create_log("Parsing maps file...\n");
    
    MapsLine* PML_Cur = NULL;
    MapsLine* PML_Next = NULL;
    int num_lns_cur, num_lns_next;
    
    num_lns_cur = parse_maps(&PML_Cur);
    if (num_lns_cur == -1) 
    {
        return -1;
    }
    
    if (check_parse_maps_err(num_lns_cur) == -1) 
    {
        return -1;
    }
    
    fclose(maps_file);
    
    //  int i = 1;
    create_log("SUCCEED: Service started!\n");
    while(1) 
    {
        sleep(SLEEP_TIME);
        
        if (open_maps_file(tr_pid) == -1) 
        {
            return -1;
        }
        
        num_lns_next = parse_maps(&PML_Next);
        if (num_lns_next == -1) 
        {
            return -1;
        }
        if (check_parse_maps_err(num_lns_next) == -1) 
        {
            return -1;
        }
        
        PML_diff(&PML_Cur, num_lns_cur, &PML_Next, num_lns_next);
        PML_swap(&PML_Cur, &num_lns_cur, &PML_Next, &num_lns_next);
        
        fclose(maps_file);
    }

    free_PML(&PML_Cur, num_lns_cur);

    return 0;
}

int open_maps_file(pid_t tr_pid) 
{
    char maps_path[PATH_LEN] = "/proc/";
    char tr_pid_dir[PATH_LEN];
    
    sprintf(tr_pid_dir, "%d", tr_pid);
    strcat(maps_path, tr_pid_dir);
    strcat(maps_path, "/maps");
    
    create_log("Opening \"%s\"..", maps_path);
    maps_file = fopen(maps_path, "r");
    if (maps_file == NULL) 
    {
        create_log("FAIL: Can't open %s: %s\n", maps_path, strerror(errno));
        return -1;
    } else {
        create_log("SUCCEED");
    }
    return 0;
}

int parse_maps(MapsLine** PML) 
{
    create_log("Parsing maps file started");
    
    char* buf = NULL;
    size_t len = 0;
    ssize_t read_sz;
    
    size_t PML_sz = 0;
    int count = 0;
    
    while((read_sz = getline(&buf, &len, maps_file)) != -1) 
    {
    //    create_log("Retrieved line of length %zu", read_sz);
        
       
        PML_sz += sizeof(MapsLine);
        *PML = realloc(*PML, PML_sz);
        (*PML)[count] = parse_maps_line(buf);
        
        if (check_parse_maps_line_err(&(*PML)[count]) == -1) 
        {
            create_log("Parsing of line #%d: FAIL", (count + 1));
            return -1;
        } else {
    //      create_log("Parsing maps line: SUCCEED");
        }
        
        print_parsed_maps_line(&(*PML)[count]);
        count++;
    }
    
    free(buf);
    return count;
}

MapsLine parse_maps_line(char* line) 
{
    MapsLine PML = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    int i = 1;
    char delim[] = " \n";
    for(char* p = strtok(line, delim); p != NULL; p = strtok(NULL, delim), i++) 
    {
        switch(i) 
        {
            case 1:
                PML.address = strdup(p);
                break;
            case 2:
                PML.perms = strdup(p);
                break;
            case 3:
                PML.offset = strdup(p);
                break;
            case 4:
                PML.device = strdup(p);
                break;
            case 5:
                PML.inode = strdup(p);
                break;
            case 6:
                PML.path = strdup(p);
                break;
            default:    // TODO: return some incorrect PML like -1.
                create_log("Parsing maps line: FAIL");
                break;
        }
    }
    
    return PML;
}

int PML_diff(MapsLine** Cur, int cnt_cur, MapsLine** Next, int cnt_next) 
{
    MapsLine* pCur = *Cur;
    MapsLine* pNext = *Next;
    create_log("Diff is started..");
    
    for(int i = 0; i < cnt_next && i < cnt_cur; i++) 
    {
        if (pCur[i].address != NULL && pNext[i].address != NULL) 
        {
            if (strcmp(pCur[i].address, pNext[i].address) != 0) 
            {
                print_diff_PML(&pCur[i], &pNext[i]);
                continue;
            }
        }
        
        if (pCur[i].perms != NULL && pNext[i].perms != NULL) 
        {
            if (strcmp(pCur[i].perms, pNext[i].perms) != 0) 
            {
                print_diff_PML(&pCur[i], &pNext[i]);
                continue;
            }
        }

        if (pCur[i].offset != NULL && pNext[i].offset != NULL) 
        {
            if (strcmp(pCur[i].offset, pNext[i].offset) != 0) 
            {
                print_diff_PML(&pCur[i], &pNext[i]);
                continue;
            }
        }

        if (pCur[i].device != NULL && pNext[i].device != NULL) 
        {
            if (strcmp(pCur[i].device, pNext[i].device) != 0) 
            {
                print_diff_PML(&pCur[i], &pNext[i]);
                continue;
            }
        }

        if (pCur[i].inode != NULL && pNext[i].inode != NULL) 
        {
            if (strcmp(pCur[i].inode, pNext[i].inode) != 0) 
            {
                print_diff_PML(&pCur[i], &pNext[i]);
                continue;
            }
        }

        if (pCur[i].path != NULL && pNext[i].path != NULL) 
        {
            if (strcmp(pCur[i].path, pNext[i].path) != 0) 
            {
                print_diff_PML(&pCur[i], &pNext[i]);
                continue;
            }
        }
    }
    
    create_log("Diff was completed SUCCESSFULLY\n");
    return 0;
}

int PML_swap(MapsLine** Cur, int* cnt_cur, MapsLine** Next, int* cnt_next) 
{
    create_log("Swapping is started..");
    
    // Swap arrays
    MapsLine* tmp = *Cur;
    *Cur = *Next;
    *Next = tmp;
    
    if (free_PML(Next, *cnt_cur) == -1) 
    {
        create_log("FAIL: free ended with error");
        return -1;
    }
    *Next = NULL;
    
    // Swap number of lines
    *cnt_cur = *cnt_next;
    *cnt_next = 0;
    
    create_log("Swap: SUCCEED\n");
    return 0;
}

int free_PML(MapsLine** pPML, int num_lines) 
{
    create_log("free(PML) is working..");
    
    MapsLine* PML_arr = *pPML;
    MapsLine PML;
    for (int i = 0; i < num_lines; i++) 
    {
        PML = PML_arr[i];
        for (int j = 0; j < MAPSLINE_FIELD_CNT; j++) 
        {
            switch (j) 
            {
                case 0:
                    if (PML.address != NULL) 
                    {
                        free(PML.address);
                    }
                    break;
                    
                case 1:
                    if (PML.perms != NULL) 
                    {
                        free(PML.perms);
                    }
                    break;
                
                case 2:
                    if (PML.offset != NULL) {
                        free(PML.offset);
                    }
                    break;
                
                case 3:
                    if (PML.device != NULL) 
                    {
                        free(PML.device);
                    }
                    break;
                
                case 4:
                    if (PML.inode != NULL) 
                    {
                        free(PML.inode);
                    }
                    break;
                
                case 5:
                    if (PML.path != NULL) 
                    {
                        free(PML.path);
                    }
                    break;
                
                default:
                    return -1;
            }
        }
    }
    free(*pPML);
    
    create_log("SUCCEED");
    return 0;
}

