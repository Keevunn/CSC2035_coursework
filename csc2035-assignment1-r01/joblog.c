/*
 * Replace the following string of 0s with your student number
 * 200868305
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include "joblog.h"

/* 
 * DO NOT EDIT the new_log_name function. It is a private helper 
 * function provided for you to create a log name from a process 
 * descriptor for use when reading, writing and deleting a log file.
 * 
 * You must work out what the function does in order to use it properly
 * and to clean up after use.
 */
static char* new_log_name(proc_t* proc) {
    static char* joblog_name_fmt = "%s/%.31s%07d.txt";
                                // string format for the name of a log file
                                // declared static to have only one instance

    if (!proc)
        return NULL;

    char* log_name;
            
    asprintf(&log_name, joblog_name_fmt, JOBLOG_PATH, proc->type_label,
        proc->id);

    return log_name;
}

/* 
 * DO NOT EDIT the joblog_init function that sets up the log directory 
 * if it does not already exist.
 */
int joblog_init(proc_t* proc) {
    if (!proc) {
        errno = EINVAL;
        return -1;
    }
        
    int r = 0;
    if (proc->is_init) {
        struct stat sb;
    
        if (stat(JOBLOG_PATH, &sb) != 0) {
            errno = 0;
            r = mkdir(JOBLOG_PATH, 0777);
        }  else if (!S_ISDIR(sb.st_mode)) {
            unlink(JOBLOG_PATH);
            errno = 0;
            r = mkdir(JOBLOG_PATH, 0777);
        }
    }

    joblog_delete(proc);    // in case log exists for proc
    
    return r;
}

job_t* joblog_read(proc_t* proc, int entry_num, job_t* job) {
    int init_errno = errno;
    if (proc == NULL || entry_num < 0)
    {
        errno = init_errno;
        return NULL;
    }
    char* file = new_log_name(proc);
    if (file == NULL)
    {
        errno = init_errno;
        return NULL;
    }
    FILE* fileOpen = fopen(file,"r");
    if (fileOpen == NULL)
    {
        errno = init_errno;
        free(file);
        return NULL;
    }
    job_t* temp_job = (job == NULL) ? malloc(sizeof(job_t)) : job;
    char buffer[JOB_STR_SIZE];
    unsigned int count = 0;

    while(fgets(buffer, JOB_STR_SIZE+1, fileOpen)!=NULL)
    {
        if (count==entry_num)
        {
            unsigned int j = 0;

            char* job_str = malloc(JOB_STR_SIZE+1);

            for (unsigned int i = 0; i < strlen(buffer); i++)
                if (buffer[i] != '\n')
                    job_str[j++] = buffer[i];
            job_str[j] = '\0';

            str_to_job(job_str,temp_job);

            free(job_str);
            fclose(fileOpen);
            free(file);

            return temp_job;
        }
        count++;
    }
    errno = init_errno;
    fclose(fileOpen);
    free(file);
    if (job == NULL) free(temp_job);
    return NULL;
}

void joblog_write(proc_t* proc, job_t* job) {
    int init_errno = errno;
    char* file = new_log_name(proc);
    FILE* fileOpen = fopen(file,"a");
    if (proc == NULL || job == NULL || file == NULL || fileOpen == NULL)
    {
        errno = init_errno;
        return;
    }
    char* jobstr = malloc(JOB_STR_SIZE+1);;
    jobstr = job_to_str(job, jobstr);
    strcat(jobstr, "\n");
    fprintf(fileOpen, "%s", jobstr);
    fclose(fileOpen);
    free(file);
    return;
}

void joblog_delete(proc_t* proc) {
    int init_errno = errno;
    char* file = new_log_name(proc);
    if (remove(file) == 0){
        return;
    } else {
        errno = init_errno;
        return;
    }
}