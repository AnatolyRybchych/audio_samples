#include "snippet.h"

#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

void empty_snippet(Snippet *snippet){
    snippet->samples_cnt = 0;
    snippet->samples = NULL;
}

void free_snippet(Snippet *snippet){
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