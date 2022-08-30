#include "snippet.h"

#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>

#define MIN(a, b) ((a) > (b) ? (b) : (a))

void snippet_empty(Snippet *snippet){
    snippet->samples_cnt = 0;
    snippet->samples = NULL;
}

void snippet_free(Snippet *snippet){
    if(snippet == NULL) return;

    snippet->samples = realloc(
        snippet->samples,
        snippet->samples_cnt = 0
    );
}

void snippet_fill_freq(Snippet *snippet, double seconds_duration, double freq){
    snippet->samples = realloc(
        snippet->samples,
        (snippet->samples_cnt = SNIPPET_FREQ * seconds_duration) * sizeof(double)
    );

    double *curr = snippet->samples;
    double *end = curr + snippet->samples_cnt;

    while (curr != end){
        *curr = sin((end - curr) * 2.0 * M_PI * freq / SNIPPET_FREQ);
        curr++;
    }
}

void snippet_combine(Snippet *result, const Snippet *first, const Snippet *second){
    result->samples = realloc(
        result->samples,
        (result->samples_cnt = first->samples_cnt + second->samples_cnt)
            * sizeof(double)
    );

    memcpy(
        result->samples, 
        first->samples, 
        first->samples_cnt * sizeof(double)
    );

    memcpy(
        result->samples + first->samples_cnt, 
        second->samples, 
        second->samples_cnt * sizeof(double)
    );
}

void snippet_append(Snippet *snippet, const Snippet *second){
    snippet->samples = realloc(
        snippet->samples,
        (snippet->samples_cnt = snippet->samples_cnt + second->samples_cnt)
            * sizeof(double)
    );

    memcpy(
        snippet->samples + snippet->samples_cnt - second->samples_cnt, 
        second->samples, 
        second->samples_cnt * sizeof(double)
    );
}

void snippet_attack(Snippet *snippet, double duration, InterpolationFunc interpolate){
    if(duration <= 0) return;
    int true_samples_duration = duration * SNIPPET_FREQ;
    int samples_duration = MIN(true_samples_duration, snippet->samples_cnt);

    double *curr = snippet->samples;
    double *end = curr + samples_duration;

    while (curr != end){
        *curr = *curr * interpolate((curr - snippet->samples) / (double)true_samples_duration); 
        curr++;
    }
}

void snippet_release(Snippet *snippet, double duration, double (*interpolate)(double progress)){
    if(duration <= 0) return;
    int true_samples_duration = duration * SNIPPET_FREQ;
    int samples_duration = MIN(true_samples_duration, snippet->samples_cnt);

    double *end = snippet->samples + snippet->samples_cnt;
    double *curr = end - samples_duration;

    while (curr != end){
        *curr = *curr * interpolate((end - curr) / (double)true_samples_duration); 
        curr++;
    }
}

void snippet_impose(Snippet *snippet, Snippet *over, ImposeFunc impose){
    int samples_cnt = MIN(snippet->samples_cnt, over->samples_cnt);

    double *snippet_curr = snippet->samples;
    double *over_curr = over->samples;
    double *snippet_end = snippet_curr + samples_cnt;

    while (snippet_curr != snippet_end){
        *snippet_curr = impose(*snippet_curr, *over_curr);

        snippet_curr++;
        over_curr++;
    }
}

void snippet_clone(Snippet *dst, const Snippet *src){
    dst->samples_cnt = src->samples_cnt;
    dst->samples = realloc(
        dst->samples,
        dst->samples_cnt * sizeof(double)
    );
    memcpy(dst->samples, src->samples, dst->samples_cnt * sizeof(double));
}

double note_freq(double note_number){
    return PITCH * pow(2.0, note_number / 12.0);
}

void snippet_fill_freq_inter(Snippet *snippet, double seconds_duration, InterpolationFunc interpolate){
    snippet->samples = realloc(
        snippet->samples,
        (snippet->samples_cnt = SNIPPET_FREQ * seconds_duration) * sizeof(double)
    );

    double *curr = snippet->samples;
    double *end = curr + snippet->samples_cnt;

    while (curr != end){
        double freq = interpolate((curr - snippet->samples) / (double)snippet->samples_cnt);
        *curr = sin((end - curr) * 2.0 * M_PI * freq / SNIPPET_FREQ);
        curr++;
    }
}