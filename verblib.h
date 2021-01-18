/* Reverb Library
* Verblib version 0.3 - 2021-01-18
*
* Philip Bennefall - philip@blastbay.com
*
* See the end of this file for licensing terms.
* This reverb is based on Freeverb, a public domain reverb written by Jezar at Dreampoint.
*
* IMPORTANT: The reverb currently only works with 1 or 2 channels, at sample rates of 22050 HZ and above.
* These restrictions may be lifted in a future version.
*
* USAGE
*
* This is a single-file library. To use it, do something like the following in one .c file.
* #define VERBLIB_IMPLEMENTATION
* #include "verblib.h"
*
* You can then #include this file in other parts of the program as you would with any other header file.
*/

#ifndef VERBLIB_H
#define VERBLIB_H

#ifdef __cplusplus
extern "C" {
#endif

    /* COMPILE-TIME OPTIONS */

    /* The maximum sample rate that should be supported, specified as a multiple of 44100. */
#ifndef verblib_max_sample_rate_multiplier
#define verblib_max_sample_rate_multiplier 4
#endif

    /* PUBLIC API */

    typedef struct verblib verblib;

    /* Initialize a verblib structure.
    *
    * Call this function to initialize the verblib structure.
    * Returns nonzero (true) on success or 0 (false) on failure.
    * The function will only fail if one or more of the parameters are invalid.
    */
    int verblib_initialize ( verblib* verb, unsigned long sample_rate, unsigned int channels );

    /* Run the reverb.
    *
    * Call this function continuously to generate your output.
    * output_buffer may be the same pointer as input_buffer if in place processing is desired.
    * frames specifies the number of sample frames that should be processed.
    */
    void verblib_process ( verblib* verb, const float* input_buffer, float* output_buffer, unsigned long frames );

    /* Set the size of the room, between 0.0 and 1.0. */
    void verblib_set_room_size ( verblib* verb, float value );

    /* Get the size of the room. */
    float verblib_get_room_size ( const verblib* verb );

    /* Set the amount of damping, between 0.0 and 1.0. */
    void verblib_set_damping ( verblib* verb, float value );

    /* Get the amount of damping. */
    float verblib_get_damping ( const verblib* verb );

    /* Set the stereo width of the reverb, between 0.0 and 1.0. */
    void verblib_set_width ( verblib* verb, float value );

    /* Get the stereo width of the reverb. */
    float verblib_get_width ( const verblib* verb );

    /* Set the volume of the wet signal, between 0.0 and 1.0. */
    void verblib_set_wet ( verblib* verb, float value );

    /* Get the volume of the wet signal. */
    float verblib_get_wet ( const verblib* verb );

    /* Set the volume of the dry signal, between 0.0 and 1.0. */
    void verblib_set_dry ( verblib* verb, float value );

    /* Get the volume of the dry signal. */
    float verblib_get_dry ( const verblib* verb );

    /* Set the mode of the reverb, where values below 0.5 mean normal and values above mean frozen. */
    void verblib_set_mode ( verblib* verb, float value );

    /* Get the mode of the reverb. */
    float verblib_get_mode ( const verblib* verb );

    /* INTERNAL STRUCTURES */

    /* Allpass filter */
    typedef struct verblib_allpass verblib_allpass;
    struct verblib_allpass
    {
        float* buffer;
        float feedback;
        int  bufsize;
        int  bufidx;
    };

    /* Comb filter */
    typedef struct verblib_comb verblib_comb;
    struct verblib_comb
    {
        float* buffer;
        float feedback;
        float filterstore;
        float damp1;
        float damp2;
        int  bufsize;
        int  bufidx;
    };

    /* Reverb model tuning values */
#define numcombs 8
#define numallpasses 4
#define muted 0.0f
#define fixedgain 0.015f
#define scalewet 3.0f
#define scaledry 2.0f
#define scaledamp 0.4f
#define scaleroom 0.28f
#define offsetroom 0.7f
#define initialroom 0.5f
#define initialdamp 0.5f
#define initialwet 1.0f/scalewet
#define initialdry 0.0f
#define initialwidth 1.0f
#define initialmode 0.0f
#define freezemode 0.5f
#define stereospread 23

    /*
    * These values assume 44.1KHz sample rate, but will be scaled appropriately.
    * The values were obtained by listening tests.
    */
#define combtuningL1 1116
#define combtuningR1 1116+stereospread
#define combtuningL2 1188
#define combtuningR2 1188+stereospread
#define combtuningL3 1277
#define combtuningR3 1277+stereospread
#define combtuningL4 1356
#define combtuningR4 1356+stereospread
#define combtuningL5 1422
#define combtuningR5 1422+stereospread
#define combtuningL6 1491
#define combtuningR6 1491+stereospread
#define combtuningL7 1557
#define combtuningR7 1557+stereospread
#define combtuningL8 1617
#define combtuningR8 1617+stereospread
#define allpasstuningL1 556
#define allpasstuningR1 556+stereospread
#define allpasstuningL2 441
#define allpasstuningR2 441+stereospread
#define allpasstuningL3 341
#define allpasstuningR3 341+stereospread
#define allpasstuningL4 225
#define allpasstuningR4 225+stereospread

    /* The main reverb structure. This is the structure that you will create an instance of when using the reverb. */
    struct verblib
    {
        unsigned int channels;
        float gain;
        float roomsize, roomsize1;
        float damp, damp1;
        float wet, wet1, wet2;
        float dry;
        float width;
        float mode;

        /*
        * The following are all declared inline
        * to remove the need for dynamic allocation.
        */

        /* Comb filters */
        verblib_comb combL[numcombs];
        verblib_comb combR[numcombs];

        /* Allpass filters */
        verblib_allpass allpassL[numallpasses];
        verblib_allpass allpassR[numallpasses];

        /* Buffers for the combs */
        float bufcombL1[combtuningL1* verblib_max_sample_rate_multiplier];
        float bufcombR1[ ( combtuningR1 ) * verblib_max_sample_rate_multiplier];
        float bufcombL2[combtuningL2* verblib_max_sample_rate_multiplier];
        float bufcombR2[ ( combtuningR2 ) * verblib_max_sample_rate_multiplier];
        float bufcombL3[combtuningL3* verblib_max_sample_rate_multiplier];
        float bufcombR3[ ( combtuningR3 ) * verblib_max_sample_rate_multiplier];
        float bufcombL4[combtuningL4* verblib_max_sample_rate_multiplier];
        float bufcombR4[ ( combtuningR4 ) * verblib_max_sample_rate_multiplier];
        float bufcombL5[combtuningL5* verblib_max_sample_rate_multiplier];
        float bufcombR5[ ( combtuningR5 ) * verblib_max_sample_rate_multiplier];
        float bufcombL6[combtuningL6* verblib_max_sample_rate_multiplier];
        float bufcombR6[ ( combtuningR6 ) * verblib_max_sample_rate_multiplier];
        float bufcombL7[combtuningL7* verblib_max_sample_rate_multiplier];
        float bufcombR7[ ( combtuningR7 ) * verblib_max_sample_rate_multiplier];
        float bufcombL8[combtuningL8* verblib_max_sample_rate_multiplier];
        float bufcombR8[ ( combtuningR8 ) * verblib_max_sample_rate_multiplier];

        /* Buffers for the allpasses */
        float bufallpassL1[allpasstuningL1* verblib_max_sample_rate_multiplier];
        float bufallpassR1[ ( allpasstuningR1 ) * verblib_max_sample_rate_multiplier];
        float bufallpassL2[allpasstuningL2* verblib_max_sample_rate_multiplier];
        float bufallpassR2[ ( allpasstuningR2 ) * verblib_max_sample_rate_multiplier];
        float bufallpassL3[allpasstuningL3* verblib_max_sample_rate_multiplier];
        float bufallpassR3[ ( allpasstuningR3 ) * verblib_max_sample_rate_multiplier];
        float bufallpassL4[allpasstuningL4* verblib_max_sample_rate_multiplier];
        float bufallpassR4[ ( allpasstuningR4 ) * verblib_max_sample_rate_multiplier];
    };

#ifdef __cplusplus
}
#endif

#endif  /* VERBLIB_H */

/* IMPLEMENTATION */

#ifdef VERBLIB_IMPLEMENTATION

#include <stddef.h>

#ifdef _MSC_VER
#define VERBLIB_INLINE __forceinline
#else
#ifdef __GNUC__
#define VERBLIB_INLINE inline __attribute__((always_inline))
#else
#define VERBLIB_INLINE inline
#endif
#endif

#define undenormalise(sample) sample+=1.0f; sample-=1.0f;

/* Allpass filter */
static void verblib_allpass_initialize ( verblib_allpass* allpass, float* buf, int size )
{
    allpass->buffer = buf;
    allpass->bufsize = size;
    allpass->bufidx = 0;
}

static VERBLIB_INLINE float verblib_allpass_process ( verblib_allpass* allpass, float input )
{
    float output;
    float bufout;

    bufout = allpass->buffer[allpass->bufidx];
    undenormalise ( bufout );

    output = -input + bufout;
    allpass->buffer[allpass->bufidx] = input + ( bufout * allpass->feedback );

    if ( ++allpass->bufidx >= allpass->bufsize )
    {
        allpass->bufidx = 0;
    }

    return output;
}

static void verblib_allpass_mute ( verblib_allpass* allpass )
{
    int i;
    for ( i = 0; i < allpass->bufsize; i++ )
    {
        allpass->buffer[i] = 0.0f;
    }
}

/* Comb filter */
static void verblib_comb_initialize ( verblib_comb* comb, float* buf, int size )
{
    comb->buffer = buf;
    comb->bufsize = size;
    comb->filterstore = 0.0f;
    comb->bufidx = 0;
}

static void verblib_comb_mute ( verblib_comb* comb )
{
    int i;
    for ( i = 0; i < comb->bufsize; i++ )
    {
        comb->buffer[i] = 0.0f;
    }
}

static void verblib_comb_set_damp ( verblib_comb* comb, float val )
{
    comb->damp1 = val;
    comb->damp2 = 1.0f - val;
}

static VERBLIB_INLINE float verblib_comb_process ( verblib_comb* comb, float input )
{
    float output;

    output = comb->buffer[comb->bufidx];
    undenormalise ( output );

    comb->filterstore = ( output * comb->damp2 ) + ( comb->filterstore * comb->damp1 );
    undenormalise ( comb->filterstore );

    comb->buffer[comb->bufidx] = input + ( comb->filterstore * comb->feedback );

    if ( ++comb->bufidx >= comb->bufsize )
    {
        comb->bufidx = 0;
    }

    return output;
}

static void verblib_update ( verblib* verb )
{
    /* Recalculate internal values after parameter change. */

    int i;

    verb->wet1 = verb->wet * ( verb->width / 2.0f + 0.5f );
    verb->wet2 = verb->wet * ( ( 1.0f - verb->width ) / 2.0f );

    if ( verb->mode >= freezemode )
    {
        verb->roomsize1 = 1.0f;
        verb->damp1 = 0.0f;
        verb->gain = muted;
    }
    else
    {
        verb->roomsize1 = verb->roomsize;
        verb->damp1 = verb->damp;
        verb->gain = fixedgain;
    }

    for ( i = 0; i < numcombs; i++ )
    {
        verb->combL[i].feedback = verb->roomsize1;
        verb->combR[i].feedback = verb->roomsize1;
        verblib_comb_set_damp ( &verb->combL[i], verb->damp1 );
        verblib_comb_set_damp ( &verb->combR[i], verb->damp1 );
    }

}

static void verblib_mute ( verblib* verb )
{
    int i;
    if ( verblib_get_mode ( verb ) >= freezemode )
    {
        return;
    }

    for ( i = 0; i < numcombs; i++ )
    {
        verblib_comb_mute ( &verb->combL[i] );
        verblib_comb_mute ( &verb->combR[i] );
    }
    for ( i = 0; i < numallpasses; i++ )
    {
        verblib_allpass_mute ( &verb->allpassL[i] );
        verblib_allpass_mute ( &verb->allpassR[i] );
    }
}

static int verblib_get_scaled_buffer_size ( unsigned long sample_rate, unsigned long value )
{
    long double result = ( long double ) sample_rate;
    result /= 44100.0;
    result = ( ( long double ) value ) * result;
    if ( result < 1.0 )
    {
        result = 1.0;
    }
    return ( int ) result;
}

int verblib_initialize ( verblib* verb, unsigned long sample_rate, unsigned int channels )
{
    int i;

    if ( channels != 1 && channels != 2 )
    {
        return 0;    /* Currently supports only 1 or 2 channels. */
    }
    if ( sample_rate < 22050 )
    {
        return 0;    /* The minimum supported sample rate is 22050 HZ. */
    }
    else if ( sample_rate > 44100 * verblib_max_sample_rate_multiplier )
    {
        return 0; /* The sample rate is too high. */
    }

    verb->channels = channels;

    /* Tie the components to their buffers. */
    verblib_comb_initialize ( &verb->combL[0], verb->bufcombL1, verblib_get_scaled_buffer_size ( sample_rate, combtuningL1 ) );
    verblib_comb_initialize ( &verb->combR[0], verb->bufcombR1, verblib_get_scaled_buffer_size ( sample_rate, combtuningR1 ) );
    verblib_comb_initialize ( &verb->combL[1], verb->bufcombL2, verblib_get_scaled_buffer_size ( sample_rate, combtuningL2 ) );
    verblib_comb_initialize ( &verb->combR[1], verb->bufcombR2, verblib_get_scaled_buffer_size ( sample_rate, combtuningR2 ) );
    verblib_comb_initialize ( &verb->combL[2], verb->bufcombL3, verblib_get_scaled_buffer_size ( sample_rate, combtuningL3 ) );
    verblib_comb_initialize ( &verb->combR[2], verb->bufcombR3, verblib_get_scaled_buffer_size ( sample_rate, combtuningR3 ) );
    verblib_comb_initialize ( &verb->combL[3], verb->bufcombL4, verblib_get_scaled_buffer_size ( sample_rate, combtuningL4 ) );
    verblib_comb_initialize ( &verb->combR[3], verb->bufcombR4, verblib_get_scaled_buffer_size ( sample_rate, combtuningR4 ) );
    verblib_comb_initialize ( &verb->combL[4], verb->bufcombL5, verblib_get_scaled_buffer_size ( sample_rate, combtuningL5 ) );
    verblib_comb_initialize ( &verb->combR[4], verb->bufcombR5, verblib_get_scaled_buffer_size ( sample_rate, combtuningR5 ) );
    verblib_comb_initialize ( &verb->combL[5], verb->bufcombL6, verblib_get_scaled_buffer_size ( sample_rate, combtuningL6 ) );
    verblib_comb_initialize ( &verb->combR[5], verb->bufcombR6, verblib_get_scaled_buffer_size ( sample_rate, combtuningR6 ) );
    verblib_comb_initialize ( &verb->combL[6], verb->bufcombL7, verblib_get_scaled_buffer_size ( sample_rate, combtuningL7 ) );
    verblib_comb_initialize ( &verb->combR[6], verb->bufcombR7, verblib_get_scaled_buffer_size ( sample_rate, combtuningR7 ) );
    verblib_comb_initialize ( &verb->combL[7], verb->bufcombL8, verblib_get_scaled_buffer_size ( sample_rate, combtuningL8 ) );
    verblib_comb_initialize ( &verb->combR[7], verb->bufcombR8, verblib_get_scaled_buffer_size ( sample_rate, combtuningR8 ) );

    verblib_allpass_initialize ( &verb->allpassL[0], verb->bufallpassL1, verblib_get_scaled_buffer_size ( sample_rate, allpasstuningL1 ) );
    verblib_allpass_initialize ( &verb->allpassR[0], verb->bufallpassR1, verblib_get_scaled_buffer_size ( sample_rate, allpasstuningR1 ) );
    verblib_allpass_initialize ( &verb->allpassL[1], verb->bufallpassL2, verblib_get_scaled_buffer_size ( sample_rate, allpasstuningL2 ) );
    verblib_allpass_initialize ( &verb->allpassR[1], verb->bufallpassR2, verblib_get_scaled_buffer_size ( sample_rate, allpasstuningR2 ) );
    verblib_allpass_initialize ( &verb->allpassL[2], verb->bufallpassL3, verblib_get_scaled_buffer_size ( sample_rate, allpasstuningL3 ) );
    verblib_allpass_initialize ( &verb->allpassR[2], verb->bufallpassR3, verblib_get_scaled_buffer_size ( sample_rate, allpasstuningR3 ) );
    verblib_allpass_initialize ( &verb->allpassL[3], verb->bufallpassL4, verblib_get_scaled_buffer_size ( sample_rate, allpasstuningL4 ) );
    verblib_allpass_initialize ( &verb->allpassR[3], verb->bufallpassR4, verblib_get_scaled_buffer_size ( sample_rate, allpasstuningR4 ) );

    /* Set default values. */
    for ( i = 0; i < numallpasses; i++ )
    {
        verb->allpassL[i].feedback = 0.5f;
        verb->allpassR[i].feedback = 0.5f;
    }

    verblib_set_wet ( verb, initialwet );
    verblib_set_room_size ( verb, initialroom );
    verblib_set_dry ( verb, initialdry );
    verblib_set_damping ( verb, initialdamp );
    verblib_set_width ( verb, initialwidth );
    verblib_set_mode ( verb, initialmode );

    /* The buffers will be full of rubbish - so we MUST mute them. */
    verblib_mute ( verb );

    return 1;
}

void verblib_process ( verblib* verb, const float* input_buffer, float* output_buffer, unsigned long frames )
{
    int i;
    float outL, outR, input;

    if ( verb->channels == 1 )
    {
        while ( frames-- > 0 )
        {
            outL = 0.0f;
            input = ( input_buffer[0] * 2.0f ) * verb->gain;

            /* Accumulate comb filters in parallel. */
            for ( i = 0; i < numcombs; i++ )
            {
                outL += verblib_comb_process ( &verb->combL[i], input );
            }

            /* Feed through allpasses in series. */
            for ( i = 0; i < numallpasses; i++ )
            {
                outL = verblib_allpass_process ( &verb->allpassL[i], outL );
            }

            /* Calculate output REPLACING anything already there. */
            output_buffer[0] = outL * verb->wet1 + input_buffer[0] * verb->dry;

            /* Increment sample pointers. */
            ++input_buffer;
            ++output_buffer;
        }
    }
    else if ( verb->channels == 2 )
    {
        while ( frames-- > 0 )
        {
            outL = outR = 0.0f;
            input = ( input_buffer[0] + input_buffer[1] ) * verb->gain;

            /* Accumulate comb filters in parallel. */
            for ( i = 0; i < numcombs; i++ )
            {
                outL += verblib_comb_process ( &verb->combL[i], input );
                outR += verblib_comb_process ( &verb->combR[i], input );
            }

            /* Feed through allpasses in series. */
            for ( i = 0; i < numallpasses; i++ )
            {
                outL = verblib_allpass_process ( &verb->allpassL[i], outL );
                outR = verblib_allpass_process ( &verb->allpassR[i], outR );
            }

            /* Calculate output REPLACING anything already there. */
            output_buffer[0] = outL * verb->wet1 + outR * verb->wet2 + input_buffer[0] * verb->dry;
            output_buffer[1] = outR * verb->wet1 + outL * verb->wet2 + input_buffer[1] * verb->dry;

            /* Increment sample pointers. */
            input_buffer += 2;
            output_buffer += 2;
        }
    }
}

void verblib_set_room_size ( verblib* verb, float value )
{
    verb->roomsize = ( value * scaleroom ) + offsetroom;
    verblib_update ( verb );
}

float verblib_get_room_size ( const verblib* verb )
{
    return ( verb->roomsize - offsetroom ) / scaleroom;
}

void verblib_set_damping ( verblib* verb, float value )
{
    verb->damp = value * scaledamp;
    verblib_update ( verb );
}

float verblib_get_damping ( const verblib* verb )
{
    return verb->damp / scaledamp;
}

void verblib_set_wet ( verblib* verb, float value )
{
    verb->wet = value * scalewet;
    verblib_update ( verb );
}

float verblib_get_wet ( const verblib* verb )
{
    return verb->wet / scalewet;
}

void verblib_set_dry ( verblib* verb, float value )
{
    verb->dry = value * scaledry;
}

float verblib_get_dry ( const verblib* verb )
{
    return verb->dry / scaledry;
}

void verblib_set_width ( verblib* verb, float value )
{
    verb->width = value;
    verblib_update ( verb );
}

float verblib_get_width ( const verblib* verb )
{
    return verb->width;
}

void verblib_set_mode ( verblib* verb, float value )
{
    verb->mode = value;
    verblib_update ( verb );
}

float verblib_get_mode ( const verblib* verb )
{
    if ( verb->mode >= freezemode )
    {
        return 1.0f;
    }
    return 0.0f;
}

#endif /* VERBLIB_IMPLEMENTATION */

/* REVISION HISTORY
*
* Version 0.3 - 2021-01-18
* Added support for sample rates of 22050 and above.
*
* Version 0.2 - 2021-01-17
* Added support for processing mono audio.
*
* Version 0.1 - 2021-01-17
* Initial release.
*/

/* LICENSE

This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT No Attribution License
Copyright (c) 2019 Philip Bennefall

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.

In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
