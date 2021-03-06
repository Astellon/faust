
#ifndef FAUSTFLOAT
#define FAUSTFLOAT double
#endif

#include "faust/gui/CGlue.h"
#include "controlTools.h"

//----------------------------------------------------------------------------
//FAUST generated code
// ----------------------------------------------------------------------------

<<includeIntrinsic>>

<<includeclass>>

mydsp* DSP;

int main(int argc, char* argv[])
{
    char rcfilename[256];
    FUI finterface;
    snprintf(rcfilename, 255, "%src", argv[0]);
    
    bool inpl = isopt(argv, "-inpl");
    
    DSP = newmydsp();
    
    CheckControlUI controlui;
    
    UIGlue glue2;
    buildUIGlue(&glue2, &finterface, true);
    buildUserInterfacemydsp(DSP, &glue2);
    
    // Get control and then 'initRandom'
    UIGlue glue3;
    buildUIGlue(&glue3, &controlui, true);
    buildUserInterfacemydsp(DSP, &glue3);
    controlui.initRandom();
    
    // init signal processor and the user interface values:
    initmydsp(DSP, 44100);
    
    // Check getSampleRate
    if (getSampleRatemydsp(DSP) != 44100) {
        cerr << "ERROR in getSampleRate" << std::endl;
    }
    
    // Check default after 'init'
    if (!controlui.checkDefaults()) {
        cerr << "ERROR in checkDefaults after 'init'" << std::endl;
    }
    
    // Check default after 'instanceResetUserInterface'
    controlui.initRandom();
    instanceResetUserInterfacemydsp(DSP);
    if (!controlui.checkDefaults()) {
        cerr << "ERROR in checkDefaults after 'instanceResetUserInterface'" << std::endl;
    }
    
    // Check default after 'instanceInit'
    controlui.initRandom();
    instanceInitmydsp(DSP, 44100);
    if (!controlui.checkDefaults()) {
        cerr << "ERROR in checkDefaults after 'instanceInit'" << std::endl;
    }
    
    // Init again
    initmydsp(DSP, 44100);
    
    int nins = getNumInputsmydsp(DSP);
    int nouts = getNumOutputsmydsp(DSP);
    
    channels* ichan = new channels(kFrames, ((inpl) ? std::max(nins, nouts) : nins));
    channels* ochan = (inpl) ? ichan : new channels(kFrames, nouts);
    
    int nbsamples = 60000;
    int linenum = 0;
    int run = 0;
    
    // recall saved state
    finterface.recallState(rcfilename);
    
    // print general informations
    printf("number_of_inputs  : %3d\n", nins);
    printf("number_of_outputs : %3d\n", nouts);
    printf("number_of_frames  : %6d\n", nbsamples);
    
    // print audio frames
    int i;
    try {
        while (nbsamples > 0) {
            if (run == 0) {
                ichan->impulse();
                finterface.setButtons(true);
            }
            if (run >= 1) {
                ichan->zero();
                finterface.setButtons(false);
            }
            int nFrames = min(kFrames, nbsamples);
            computemydsp(DSP, nFrames, ichan->buffers(), ochan->buffers());
            run++;
            for (int i = 0; i < nFrames; i++) {
                printf("%6d : ", linenum++);
                for (int c = 0; c < nouts; c++) {
                    FAUSTFLOAT f = normalize(ochan->buffers()[c][i]);
                    printf(" %8.6f", f);
                }
                printf("\n");
            }
            nbsamples -= nFrames;
        }
    } catch (...) {
        cerr << "ERROR in " << argv[1] << " line : " << i << std::endl;
    }
    
    return 0;
}
