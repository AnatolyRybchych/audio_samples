#ifndef SNIPPET_H
#define SNIPPET_H

#define SNIPPET_FREQ 44100
#define PITCH 440


//progress is on range [0.0, 1.0]
typedef double (*InterpolationFunc)(double progress);
typedef struct Snippet Snippet;

// for SNIPPET_FREQ hz mono audio 
// should be freed by free_snippet
struct Snippet{
    int samples_cnt;
    double *samples;
};

// creates empty snippet 
void snippet_empty(Snippet *snippet);

// initializes snippet with data using frequency
void snippet_fill_freq(Snippet *snippet, double seconds_duration, double freq);

// initialize snippet "result" using data of "first" and "secoond"
void snippet_combine(Snippet *result, const Snippet *first, const Snippet *second);

// appends data of "second" to "snippet"
void snippet_append(Snippet *snippet, const Snippet *second);

// multiplies amplitude of data on interval from start to duration by value : (interpolate(time/duration))
void snippet_attack(Snippet *snippet, double duration, double (*interpolate)(double progress));

// multiplies amplitude of data on interval from end - duration end by value : (interpolate((end - time)/duration))
void snippet_release(Snippet *snippet, double duration, InterpolationFunc interpolate);

// clears snippet buffer
void snippet_free(Snippet *snippet);

#endif //SNIPPET_H
