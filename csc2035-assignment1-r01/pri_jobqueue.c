/*
 * Replace the following string of 0s with your student number
 * 200868305
 */
#include <stdlib.h>
#include <stdbool.h>
#include "pri_jobqueue.h"


pri_jobqueue_t* pri_jobqueue_new() {
    pri_jobqueue_t* pjq = (pri_jobqueue_t*) malloc(sizeof(pri_jobqueue_t));
    pri_jobqueue_init(pjq);
    return pjq;
}

void pri_jobqueue_init(pri_jobqueue_t* pjq) {
    if (pjq != NULL)
    {
        pjq->buf_size = JOB_BUFFER_SIZE;
        pjq->size=0;
        job_t job;
        job_init(&job);
        char str[JOB_STR_SIZE];

        for (int i = 0; i < pjq->buf_size; i++)
        {
            pjq->jobs[i] = job;
        }
    }
    return;
}

job_t* pri_jobqueue_dequeue(pri_jobqueue_t* pjq, job_t* dst) {
    if (pjq == NULL || pri_jobqueue_is_empty(pjq))
        return NULL;
    if (dst == NULL)
        dst = malloc(sizeof(job_t));

    *dst = pjq->jobs[pjq->size - 1];

    job_init(&pjq->jobs[pjq->size - 1]);
    pjq->size--;

    return dst;
}

void pri_jobqueue_enqueue(pri_jobqueue_t* pjq, job_t* job) {
    if (pjq == NULL || pri_jobqueue_is_full(pjq) || job == NULL || pjq->size == pjq->buf_size || job->priority <= 0 )
        return;

    int top = pjq->size;

    for (int i = 0; i < pjq->size; i++) {
        if (pjq->jobs[i].priority <= job->priority)
        {
            top = i;
            break;
        }
    }
    for (int i = pjq->size - 1; i > top - 1; i--)
        pjq->jobs[i + 1] = pjq->jobs[i];
    pjq->jobs[top] = *job;
    pjq->size++;
    return;
}

bool pri_jobqueue_is_empty(pri_jobqueue_t* pjq) {
    if (pjq == NULL)
        return true;
    for (int i = 0; i < pjq->buf_size; ++i) {
        if (pjq->jobs[i].priority > 0)
            return false;
    }
    return true;
}

bool pri_jobqueue_is_full(pri_jobqueue_t* pjq) {
    if (pjq == NULL)
        return true;
    for (int i = 0; i < pjq->buf_size; ++i) {
        if (pjq->jobs[i].priority <= 0)
            return false;
    }
    return true;
}

job_t* pri_jobqueue_peek(pri_jobqueue_t* pjq, job_t* dst) {
    if (pjq == NULL || pri_jobqueue_is_empty(pjq))
        return NULL;
    if (dst == NULL)
        dst = malloc(sizeof(job_t));

    *dst = pjq->jobs[pjq->size - 1];
    return dst;
}

int pri_jobqueue_size(pri_jobqueue_t* pjq) {
    if (pjq == NULL)
        return 0;
    int size = 0;
    for (int i = 0; i < pjq->buf_size; ++i) {
        if (pjq->jobs[i].priority > 0)
            size++;
    }
    return size;
}

int pri_jobqueue_space(pri_jobqueue_t* pjq) {
    if (pjq == NULL)
        return 0;

    int space = 0;
    for (int i = 0; i < pjq->buf_size; ++i) {
        if (pjq->jobs[i].priority <= 0)
            space++;
    }
    return space;
}

void pri_jobqueue_delete(pri_jobqueue_t* pjq) {
    free(pjq);
    return;
}
