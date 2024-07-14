#ifdef ONE_U
#include "countFreq.h"
#include "TOA2BOA.h"
#include "stm32f4xx_hal.h"
#include "src_user/Applications/Middleware/MemoryMW.h"
#include "src_user/IfWrapper/HAL.h"

extern IWDG_HandleTypeDef hiwdg;

//initial parameters
uint8_t toa2boa_params_[4] = {
		0x00,	//enable/disable TOA2BOA
		0x00,	//use mram params
		0x00,	//use programmed N
		0x00	//reserved
};

float mcam_gain_f_[3] = {1.0f, 1.0f, 1.0f};
float mcam_bias_f_[3] = {0.0f, 0.0f, 0.0f};
float mcam_esun_f_[3] = {1549.49f, 1820.75f, 2004.57f};
float mcam_pgain_f_[3] = {1.0f, 1.0f, 1.0f};
float mcam_pbias_f_[3] = {0.0f, 0.0f, 0.0f};
uint8_t mcam_N_u8_[4] = {
		0x00,
		0x00,
		0x00,
		0x32
};

float scam_gain_f_[3] = {1.0f, 1.0f, 1.0f};
float scam_bias_f_[3] = {0.0f, 0.0f, 0.0f};
float scam_esun_f_[3] = {1549.49f, 1820.75f, 2004.57f};
float scam_pgain_f_[3] = {1.0f, 1.0f, 1.0f};
float scam_pbias_f_[3] = {0.0f, 0.0f, 0.0f};
uint8_t scam_N_u8_[4] = {
		0x00,
		0x00,
		0x00,
		0x32
};

//loaded paramters
uint8_t toa2boa_params[4];
uint8_t mcam_N_u8[4];
uint8_t scam_N_u8[4];

//float
float mcam_gain_f[3];
float mcam_bias_f[3];
float mcam_esun_f[3];
float mcam_pgain_f[3];
float mcam_pbias_f[3];
uint32_t mcam_N_u32;

float scam_gain_f[3];
float scam_bias_f[3];
float scam_esun_f[3];
float scam_pgain_f[3];
float scam_pbias_f[3];
uint32_t scam_N_u32;

static void toa2boa_load_param(void){ //init-function of app
//	printf("toa2boa_param_init\r\n");
	read_param(toa2boa_params, toa2boa_params_,sizeof(toa2boa_params), 0x4F67);

	//load params
	if(toa2boa_params[1] == 0x01){
		//read param from mram
		read_param(mcam_gain_f,   mcam_gain_f_,  sizeof(mcam_gain_f),   0x4F6B);
		read_param(mcam_bias_f,   mcam_bias_f_,  sizeof(mcam_bias_f),   0x4F77);
		read_param(mcam_esun_f,   mcam_esun_f_,  sizeof(mcam_esun_f),   0x4F83);
		read_param(mcam_pgain_f,  mcam_pgain_f_, sizeof(mcam_pgain_f),  0x4F8F);
		read_param(mcam_pbias_f,  mcam_pbias_f_, sizeof(mcam_pbias_f),  0x4F9B);
		read_param(mcam_N_u8,		mcam_N_u8_,	   sizeof(mcam_N_u8),	  0x4FA7);

		read_param(scam_gain_f,   scam_gain_f_,  sizeof(scam_gain_f),   0x4FAB);
		read_param(scam_bias_f,   scam_bias_f_,  sizeof(scam_bias_f),   0x4FB7);
		read_param(scam_esun_f,   scam_esun_f_,  sizeof(scam_esun_f),   0x4FC3);
		read_param(scam_pgain_f,  scam_pgain_f_, sizeof(scam_pgain_f),  0x4FCF);
		read_param(scam_pbias_f,  scam_pbias_f_, sizeof(scam_pbias_f),  0x4FDB);
		read_param(scam_N_u8,		scam_N_u8_,    sizeof(scam_N_u8),     0x4FE7);

		//convert u8 (u32) to u32
		mcam_N_u32 = mcam_N_u8[0] << 24 | mcam_N_u8[1] << 16 |
				mcam_N_u8[2] << 8 | mcam_N_u8[3];
		scam_N_u32 = scam_N_u8[0] << 24 | scam_N_u8[1] << 16 |
				scam_N_u8[2] << 8 | scam_N_u8[3];
	}
	else{
		//use default value
		std::memcpy(mcam_gain_f,mcam_gain_f_,12);
		std::memcpy(mcam_bias_f,mcam_bias_f_,12);
		std::memcpy(mcam_esun_f,mcam_esun_f_,12);
		std::memcpy(mcam_pgain_f,mcam_pgain_f_,12);
		std::memcpy(mcam_pbias_f,mcam_pbias_f_,12);
		std::memcpy(scam_gain_f,scam_gain_f_,12);
		std::memcpy(scam_bias_f,scam_bias_f_,12);
		std::memcpy(scam_esun_f,scam_esun_f_,12);
		std::memcpy(scam_pgain_f,scam_pgain_f_,12);
		std::memcpy(scam_pbias_f,scam_pbias_f_,12);
		//convert u8 (u32) to u32
		mcam_N_u32 = mcam_N_u8_[0] << 24 | mcam_N_u8_[1] << 16 |
				mcam_N_u8[2] << 8 | mcam_N_u8[3];
		scam_N_u32 = scam_N_u8_[0] << 24 | scam_N_u8_[1] << 16 |
				scam_N_u8_[2] << 8 | scam_N_u8_[3];
	}
}

static void toa2boa_initialize_param(void){
	printf("toa2boa_param_exe\r\n");
	write_param(toa2boa_params, toa2boa_params_,sizeof(toa2boa_params), 0x4F67);

	write_param(mcam_gain_f,   mcam_gain_f_,  sizeof(mcam_gain_f),   0x4F6B);
	write_param(mcam_bias_f,   mcam_bias_f_,  sizeof(mcam_bias_f),   0x4F77);
	write_param(mcam_esun_f,   mcam_esun_f_,  sizeof(mcam_esun_f),   0x4F83);
	write_param(mcam_pgain_f,  mcam_pgain_f_, sizeof(mcam_pgain_f),  0x4F8F);
	write_param(mcam_pbias_f,  mcam_pbias_f_, sizeof(mcam_pbias_f),  0x4F9B);
	write_param(mcam_N_u8,		mcam_N_u8_,	   sizeof(mcam_N_u8),	  0x4FA7);

	write_param(scam_gain_f,   scam_gain_f_,  sizeof(scam_gain_f),   0x4FAB);
	write_param(scam_bias_f,   scam_bias_f_,  sizeof(scam_bias_f),   0x4FB7);
	write_param(scam_esun_f,   scam_esun_f_,  sizeof(scam_esun_f),   0x4FC3);
	write_param(scam_pgain_f,  scam_pgain_f_, sizeof(scam_pgain_f),  0x4FCF);
	write_param(scam_pbias_f,  scam_pbias_f_, sizeof(scam_pbias_f),  0x4FDB);
	write_param(scam_N_u8,		scam_N_u8_,    sizeof(scam_N_u8),     0x4FE7);
}

AppInfo toa2boa_param_update(void)
{
	return AI_create_app_info("toa2boa_param_init", toa2boa_load_param, toa2boa_initialize_param);
}


int deduct_Lp_RGB565(uint32_t *startAddr, uint32_t size, float *G, float *B,
                     float *pG, float *pB, float *Lp, uint8_t *OUT) {
  // deduct Lp for each pixel
  for (uint32_t i = 0; i < size/2; i++) {
    // unpack pixel
	uint8_t *b1 = (uint8_t*)(startAddr + 2*i);
	uint8_t *b2 = (uint8_t*)(startAddr + 2*i + 1);
    uint8_t pix[3];
    pix[0] = (*b1) & 0xf8;
    pix[1] = ((*b1) << 5) | (((*b2) & 0xe0) >> 3);
    pix[2] = (*b2) << 3;

    // gain bias
    float pix_f[3];
    for (uint8_t j = 0; j < 3; j++) {
      pix_f[j] = (float(pix[j]) * G[j] + B[j]);
    }

    // deduct
    for (uint8_t j = 0; j < 3; j++) {
      if (Lp[j] > pix_f[j])
        pix_f[j] = 0;
      else
        pix_f[j] -= Lp[j];

      // change back to pixel values
      // further transform if required
      pix[j] = uint8_t(((pix_f[j] - B[j]) / G[j]) * pG[j] + pB[j]);
    }

    // pack pixel
    uint8_t packpix[2];
    packpix[0] = (pix[0] & 0xf8) | (pix[1] >> 5);
    packpix[1] = ((pix[1] << 3) & 0xe0) | (pix[2] >> 3);
    // printf("%d, %d \n", packpix[0], packpix[1]);
    // overwrite pixel
    uint8_t *p1 = (uint8_t*)startAddr + 2*i;
    uint8_t *p2 = (uint8_t*)startAddr + 2*i + 1;
    *p1 = packpix[0];
    *p2 = packpix[1];
//    printf("%02x", *p1);
//    printf("%02x", *p2);
    HAL_IWDG_Refresh_C2A(&hiwdg);
  }
  printf("\n");
  return 0;
}

// Sz: Solar zenith in Radians
// mode 1: DOS1, 2: DOS2
// cam 1: MCAM, 2: SCAM
int TOA2BOA_DOS(uint32_t *startAddr, uint32_t size, float Sz, uint32_t N,
                uint8_t mode, uint8_t cam, uint8_t *BOA) {
	if(toa2boa_params[0] == 0x01){
		uint8_t Lp[3];
		uint32_t NN = N;
		float Tz;
		// float Sz; //solar zenith in Radians
		float cSz = cosf(Sz);

		/*
		float Gain_f[3] = {1.0f, 1.0f, 1.0f}; //{1.0, 1.0, 1.0};
		float Bias_f[3] = {0.0f, 0.0f, 0.0f};
		float ESUN_f[3] = {1549.49f, 1820.75f, 2004.57f};
		// http://bleutner.github.io/RStoolbox/r/2016/01/26/estimating-landsat-8-esun-values

		//post gain to change pixel to radiance if required
		float pGain_f[3] = {1.0f, 1.0f, 1.0f}; //{1.0, 1.0, 1.0};
		float pBias_f[3] = {0.0f, 0.0f, 0.0f};
		*/
		float Gain_f[3],Bias_f[3],ESUN_f[3],pGain_f[3],pBias_f[3];
//		if(cam == 1) {
//			//MCAM cam = 1
//			for(uint8_t i = 0; i < 3; i++) {
//				Gain_f[i] = mcam_gain_f[i];
//				Bias_f[i] = mcam_bias_f[i];
//				ESUN_f[i] = mcam_esun_f[i];
//				pGain_f[i] = mcam_pgain_f[i];
//				pBias_f[i] = mcam_pbias_f[i];
//			}
//			if(toa2boa_params[2] == 0x01){
//				NN = mcam_N_u32;
//			}
//		}
		if(cam == 2) {
			//SCAM
			for(uint8_t i = 0; i < 3; i++) {
				Gain_f[i] = scam_gain_f[i];
				Bias_f[i] = scam_bias_f[i];
				ESUN_f[i] = scam_esun_f[i];
				pGain_f[i] = scam_pgain_f[i];
				pBias_f[i] = scam_pbias_f[i];
			}
			if(toa2boa_params[2] == 0x01){
				NN = scam_N_u32;
			}
		}
		else {
			for(uint8_t i = 0; i < 3; i++) {
				Gain_f[i] = 1.0f;
				Bias_f[i] = 0.0f;
				pGain_f[i] = 1.0f;
				pBias_f[i] = 0.0f;
			}
			ESUN_f[0] = 1549.49f;
			ESUN_f[1] = 1820.75f;
			ESUN_f[2] = 2004.57f;
		}
		// Tz for DOS1 and DOS2
		if (mode == 1)
			Tz = 1.0f;
		else if (mode == 2)
			Tz = cSz;
		else Tz = 1.0f;

		// get Frequency N
		//freqN(startAddr, size, N, Lp);
		freqN(startAddr, size, NN, Lp);

		// printf("%d, %d, %d\n", Lp[0], Lp[1], Lp[2]);
		// deduct 1%
		float Lpf[3];
		for (uint8_t i = 0; i < 3; i++) {

			Lpf[i] = float(Lp[i]) * Gain_f[i] + Bias_f[i] -
							 0.01 * (ESUN_f[i] * cSz * Tz) / M_PI;
	//    if (Lpf[i] > 0)
	//      // Lp[i] = uint8_t(Lpf);
	//      Lpf[i] = Lpf[i];
	//    else
	//      // Lp[i] = 0;
	//      Lpf[i] = 0;
			if (Lpf[i] <= 0)
				Lpf[i] = 0;
		}

		// printf("%f, %f, %f\n", Lpf[0], Lpf[1], Lpf[2]);
		deduct_Lp_RGB565(startAddr, size, Gain_f, Bias_f, pGain_f, pBias_f, Lpf, BOA);

		return 1;
	}
	else{

		return 0;
	}
}
#endif/* for 1U*/
