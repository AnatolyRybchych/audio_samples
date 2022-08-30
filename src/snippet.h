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

void empty_snippet(Snippet *snippet);
void snippet_fill_freq(Snippet *snippet, double seconds_duration, double freq);
void free_snippet(Snippet *snippet);

#endif //SNIPPET_H
