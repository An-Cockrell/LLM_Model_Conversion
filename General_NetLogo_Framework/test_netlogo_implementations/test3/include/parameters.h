// parameters.h
#ifndef PARAMETERS_H
#define PARAMETERS_H

const bool RENDERING = true;
const bool SAVING_DATA = false;
const bool TURTLE_CONTINOUS_MOVEMENT = false;

const int RNG_SEED=108;
const int NUM_STEPS = 2000;


const static int WORLD_WIDTH = 101;
const static int WORLD_HEIGHT = 101;
const int PATCH_MAX_CAPACITY = 10;
const int MAX_RANDOM_DISTANCE = 100;

const float ENDOTOXIN_EVAPORATION_FACTOR = 0.7;
const float PAF_EVAPORATION_FACTOR = 0.7;
const float CYTOTOX_EVAPORATION_FACTOR = 0.7;
const float TNF_EVAPORATION_FACTOR = 0.6;
const float IL_1_EVAPORATION_FACTOR = 0.8;
const float STNFR_EVAPORATION_FACTOR = 0.9;
const float IL_1RA_EVAPORATION_FACTOR = 0.9;
const float SIL_1R_EVAPORATION_FACTOR = 0.9;
const float INF_G_EVAPORATION_FACTOR = 0.8;
const float IL_8_EVAPORATION_FACTOR = 0.7;
const float IL_10_EVAPORATION_FACTOR = 0.95;
const float IL_12_EVAPORATION_FACTOR = 0.8;
const float IL_4_EVAPORATION_FACTOR = 0.95;
const float GCSF_EVAPORATION_FACTOR = 0.95;

const float ENDOTOXIN_DIFFUSION_FACTOR = 1.0;
const float PAF_DIFFUSION_FACTOR = 0.6;
const float CYTOTOX_DIFFUSION_FACTOR = 0.4;
const float TNF_DIFFUSION_FACTOR = 0.8;
const float STNFR_DIFFUSION_FACTOR = 0.8;
const float IL_1_DIFFUSION_FACTOR = 0.6;
const float INF_G_DIFFUSION_FACTOR = 0.8;
const float IL_8_DIFFUSION_FACTOR = 0.6;
const float IL_10_DIFFUSION_FACTOR = 0.8;
const float IL_1RA_DIFFUSION_FACTOR = 0.8;
const float SIL_1R_DIFFUSION_FACTOR = 0.8;
const float IL_12_DIFFUSION_FACTOR = 0.8;
const float IL_4_DIFFUSION_FACTOR = 0.8;
const float GCSF_DIFFUSION_FACTOR = 1.0;

#endif