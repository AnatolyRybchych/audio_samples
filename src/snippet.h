#ifndef SNIPPET_H
#define SNIPPET_H

#define SNIPPET_FREQ 44100
#define PITCH 440

typedef struct Snippet Snippet;

// for SNIPPET_FREQ hz mono audio 
// should be freed by free_snippet
struct Snippet{
    int samples_cnt;
    double *samples;
};

void snippet_empty(Snippet *snippet);
void snippet_fill_freq(Snippet *snippet, double seconds_duration, double freq);
void snippet_combine(Snippet *result, const Snippet *first, const Snippet *second);
void snippet_append(Snippet *snippet, const Snippet *second);
void snippet_free(Snippet *snippet);

#endif //SNIPPET_H
