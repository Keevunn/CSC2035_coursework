/*
 * Replace the following string of 0s with your student number
 * 200868305
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "job.h"

/* 
 * DO NOT EDIT the job_new function.
 */
job_t* job_new(pid_t pid, unsigned int id, unsigned int priority, 
    const char* label) {
    return job_set((job_t*) malloc(sizeof(job_t)), pid, id, priority, label);
}

job_t* job_copy(job_t* src, job_t* dst) {  
	if (src == NULL)
	{
        errno = ENOMEM;
		return NULL;
	}if (src == dst) 
	{
		return src;
	}if (dst == NULL)
	{
		dst = (job_t*) malloc(sizeof(job_t));
	} 
	memcpy(dst, src, sizeof(job_t));
	return dst;
}

void job_init(job_t* job) {
	if (job != NULL)
	{
		job->pid = 0;
		job->id = 0;
		job->priority = 0;
		//snprintf(job->label, MAX_NAME_SIZE, "%s", PAD_STRING);
        strncpy(job->label, PAD_STRING, MAX_NAME_SIZE);
	}
}

bool job_is_equal(job_t* j1, job_t* j2) {
	if (j1 == j2) { return true;}
	if (j1 == NULL && j2 == NULL) { return true;}
    if (j1 == NULL || j2 == NULL) { return false;}
    if (j1->pid==j2->pid && j1->id==j2->id && j1->priority==j2->priority && !strncmp(j1->label, j2->label, MAX_NAME_SIZE)) { return true; } 
	return false;
}


job_t* job_set(job_t* job, pid_t pid, unsigned int id, unsigned int priority,
    const char* label) {
	if (job != NULL)
	{
        if (job->pid != pid) { job->pid = pid; }
        if (job->id != id) { job->id = id; }
        if (job->priority != priority) { job->priority = priority; }

        if (label == NULL) {
            strncpy(job->label, PAD_STRING, MAX_NAME_SIZE - 1);
        } else if (strncmp(label, "", MAX_NAME_SIZE) == 0) { strncpy(job->label, PAD_STRING, MAX_NAME_SIZE - 1); }
        else if (strncmp(job->label, label, MAX_NAME_SIZE - 1) != 0) {
            if (strlen(label) < MAX_NAME_SIZE - 1) {
                strncpy(job->label, label, strlen(label));
                memset(job->label + strlen(label), '*', MAX_NAME_SIZE - 1 - strlen(label));
            } else { strncpy(job->label, label, strlen(label)); }
        }
        job->label[MAX_NAME_SIZE - 1] = '\0';
        return job;
	}
    return NULL;
}

char* job_to_str(job_t* job, char* str) {
    if (str == NULL){ str = (char*) malloc(JOB_STR_SIZE); } 
	snprintf(str, JOB_STR_SIZE, JOB_STR_FMT, (int) job->pid, job->id, job->priority, job->label);
	return str;
}

job_t* str_to_job(char* str, job_t* job) {
    if (job == NULL) { job = (job_t*) malloc(sizeof(job_t)); }

	if (str == NULL || strlen(str) != JOB_STR_SIZE - 1)
	{
        return NULL;
	}

    if (sscanf(str, JOB_STR_FMT, &(job->pid), &(job->id), &(job->priority), &(job->label)) != 4)
    {
        if (job != NULL) {free(job);}
        return NULL;
    }

	return job;
}

void job_delete(job_t* job) {
    free(job);
}