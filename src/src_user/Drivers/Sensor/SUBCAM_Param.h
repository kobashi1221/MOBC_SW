/*
 * SUBCAM_Param.h
 *
 *  Created on: 2023/06/12
 *      Author: Osaki
 */

#ifndef SRC_USER_DRIVERS_SENSOR_SUBCAM_PARAM_H_
#define SRC_USER_DRIVERS_SENSOR_SUBCAM_PARAM_H_

/******/
// Sensor Core Register
/******/

#define QS                      0x44
#define HSIZE                   0x51
#define VSIZE                   0x52
#define XOFFL                   0x53
#define YOFFL                   0x54
#define VHYX                    0x55
#define DPRP                    0x56
//#define TEST                    0x57
#define ZMOW                    0x5A
#define ZMOH                    0x5B
#define ZMHH                    0x5C
#define BPADDR                  0x7C
#define BPDATA                  0x7D
#define SIZEL                   0x8C
#define HSIZE8                  0xC0
#define VSIZE8                  0xC1
#define MS_SP                   0xF0
#define SS_ID                   0xF7
#define SS_CTRL                 0xF8
#define MC_AL                   0xFA
#define MC_AH                   0xFB
#define MC_D                    0xFC
#define P_CMD                   0xFD
#define P_STATUS                0xFE

#define R_BYPASS                0x05
#define R_BYPASS_DSP_EN         0x00
#define R_BYPASS_DSP_BYPAS      0x01

#define CTRLI                   0x50
#define CTRLI_LP_DP             0x80
#define CTRLI_ROUND             0x40

#define CTRL2                   0x86
#define CTRL2_DCW_EN            0x20
#define CTRL2_SDE_EN            0x10
#define CTRL2_UV_ADJ_EN         0x08
#define CTRL2_UV_AVG_EN         0x04
#define CTRL2_CMX_EN            0x01

#define CTRL3                   0x87
#define CTRL3_BPC_EN            0x80
#define CTRL3_WPC_EN            0x40

#define CTRL0                   0xC2
#define CTRL0_AEC_EN            0x80
#define CTRL0_AEC_SEL           0x40
#define CTRL0_STAT_SEL          0x20
#define CTRL0_VFIRST            0x10
#define CTRL0_YUV422            0x08
#define CTRL0_YUV_EN            0x04
#define CTRL0_RGB_EN            0x02
#define CTRL0_RAW_EN            0x01

#define CTRL1                   0xC3
#define CTRL1_AWB               0x08 /* Enable AWB */

#define R_DVP_SP                0xD3
#define R_DVP_SP_AUTO_MODE      0x80

#define IMAGE_MODE              0xDA
#define IMAGE_MODE_Y8_DVP_EN    0x40
#define IMAGE_MODE_JPEG_EN      0x10
#define IMAGE_MODE_YUV422       0x00
#define IMAGE_MODE_RAW10        0x04
#define IMAGE_MODE_RGB565       0x09
#define IMAGE_MODE_HREF_VSYNC   0x02
#define IMAGE_MODE_LBYTE_FIRST  0x01
#define IMAGE_MODE_GET_FMT(x)   ((x)&0xC)

//#define RESET                   0xE0
#define RESET_MICROC            0x40
#define RESET_SCCB              0x20
#define RESET_JPEG              0x10
#define RESET_DVP               0x04
#define RESET_IPU               0x02
#define RESET_CIF               0x01
#define RESET_NOTHING           0x00

#define MC_BIST                 0xF9
#define MC_BIST_RESET           0x80
#define MC_BIST_BOOT_ROM_SEL    0x40
#define MC_BIST_12KB_SEL        0x20
#define MC_BIST_12KB_MASK       0x30
#define MC_BIST_512KB_SEL       0x08
#define MC_BIST_512KB_MASK      0x0C
#define MC_BIST_BUSY_BIT_R      0x02
#define MC_BIST_MC_RES_ONE_SH_W 0x02
#define MC_BIST_LAUNCH          0x01

#define BANK_SEL                0xFF
#define BANK_SEL_DSP            0x00
#define BANK_SEL_SENSOR         0x01

/* Sensor register bank FF=0x01*/

#define GAIN                0x00
#define COM1                0x03
#define REG08               0x08
#define REG_PID             0x0A
#define REG_VER             0x0B
#define COM4                0x0D
#define AEC                 0x10
#define COM10               0x15
#define HSTART              0x17
#define HSTOP               0x18
#define VSTART              0x19
#define VSTOP               0x1A
#define MIDH                0x1C
#define MIDL                0x1D
#define AEW                 0x24
#define AEB                 0x25
#define REG2A               0x2A
#define FRARL               0x2B
#define ADDVSL              0x2D
#define ADDVSH              0x2E
#define YAVG                0x2F
#define HSDY                0x30
#define HEDY                0x31
#define ARCOM2              0x34
#define REG45               0x45
#define FLL                 0x46
#define FLH                 0x47
#define COM19               0x48
#define ZOOMS               0x49
#define COM22               0x4B
#define COM25               0x4E
#define BD50                0x4F
#define BD60                0x50
#define REG5D               0x5D
#define REG5E               0x5E
#define REG5F               0x5F
#define REG60               0x60
#define HISTO_LOW           0x61
#define HISTO_HIGH          0x62

#define REG04               0x04
#define REG04_DEFAULT       0x28
#define REG04_HFLIP_IMG     0x80
#define REG04_VFLIP_IMG     0x40
#define REG04_VREF_EN       0x10
#define REG04_HREF_EN       0x08
#define REG04_SET(x)        (uint8_t)(REG04_DEFAULT|x)

#define COM2                0x09
#define COM2_STDBY          0x10
#define COM2_OUT_DRIVE_1x   0x00
#define COM2_OUT_DRIVE_2x   0x01
#define COM2_OUT_DRIVE_3x   0x02
#define COM2_OUT_DRIVE_4x   0x03

#define CLKRC               0x11
#define CLKRC_DOUBLE        0x82
#define CLKRC_DIVIDER_MASK  0x3F

#define COM3                0x0C
#define COM3_DEFAULT        0x38
#define COM3_BAND_50Hz      0x04
#define COM3_BAND_60Hz      0x00
#define COM3_BAND_AUTO      0x02
#define COM3_BAND_SET(x)    (uint8_t)(COM3_DEFAULT|x)

#define COM7                0x12
#define COM7_SRST           0x80
#define COM7_RES_UXGA       0x00 /* UXGA */
#define COM7_RES_SVGA       0x40 /* SVGA */
#define COM7_RES_CIF        0x20 /* CIF  */
#define COM7_ZOOM_EN        0x04 /* Enable Zoom */
#define COM7_COLOR_BAR      0x02 /* Enable Color Bar Test */
#define COM7_GET_RES(x)     ((x)&0x70)

#define COM8                0x13
#define COM8_DEFAULT        0xC0
#define COM8_BNDF_EN        0x20 /* Enable Banding filter */
#define COM8_AGC_EN         0x04 /* AGC Auto/Manual control selection */
#define COM8_AEC_EN         0x01 /* Auto/Manual Exposure control */
#define COM8_SET(x)         (uint8_t)(COM8_DEFAULT|x)
#define COM8_SET_AEC(r,x)   (((r)&0xFE)|((x)&1))

#define COM9                0x14 /* AGC gain ceiling */
#define COM9_DEFAULT        0x08
#define COM9_AGC_GAIN_2x    0x00 /* AGC:    2x */
#define COM9_AGC_GAIN_4x    0x01 /* AGC:    4x */
#define COM9_AGC_GAIN_8x    0x02 /* AGC:    8x */
#define COM9_AGC_GAIN_16x   0x03 /* AGC:   16x */
#define COM9_AGC_GAIN_32x   0x04 /* AGC:   32x */
#define COM9_AGC_GAIN_64x   0x05 /* AGC:   64x */
#define COM9_AGC_GAIN_128x  0x06 /* AGC:  128x */
#define COM9_AGC_SET(x)     (uint8_t)(COM9_DEFAULT|(x<<5))

#define VV                  0x26
#define VV_AGC_TH_SET(h,l)  ((h<<4)|(l&0x0F))

#define REG32               0x32
#define REG32_UXGA          0x36
#define REG32_SVGA          0x09
#define REG32_CIF           0x00

#define VAL_SET(x, mask, rshift, lshift) (uint8_t)((((x) >> rshift) & mask) << lshift)

#define CTRLI_V_DIV_SET(x)      VAL_SET(x, 0x3, 0, 3)
#define CTRLI_H_DIV_SET(x)      VAL_SET(x, 0x3, 0, 0)

#define SIZEL_HSIZE8_11_SET(x)  VAL_SET(x, 0x1, 11, 6)
#define SIZEL_HSIZE8_SET(x)     VAL_SET(x, 0x7, 0, 3)
#define SIZEL_VSIZE8_SET(x)     VAL_SET(x, 0x7, 0, 0)

#define HSIZE8_SET(x)           VAL_SET(x, 0xFF, 3, 0)
#define VSIZE8_SET(x)           VAL_SET(x, 0xFF, 3, 0)

#define HSIZE_SET(x)            VAL_SET(x, 0xFF, 2, 0)
#define VSIZE_SET(x)            VAL_SET(x, 0xFF, 2, 0)

#define XOFFL_SET(x)            VAL_SET(x, 0xFF, 0, 0)
#define YOFFL_SET(x)            VAL_SET(x, 0xFF, 0, 0)

#define VHYX_VSIZE_SET(x)       VAL_SET(x, 0x1, (8+2), 7)
#define VHYX_HSIZE_SET(x)       VAL_SET(x, 0x1, (8+2), 3)
#define VHYX_YOFF_SET(x)        VAL_SET(x, 0x3, 8, 4)
#define VHYX_XOFF_SET(x)        VAL_SET(x, 0x3, 8, 0)

#define TEST_HSIZE_SET(x)       VAL_SET(x, 0x1, (9+2), 7)

#define ZMOW_OUTW_SET(x)        VAL_SET(x, 0xFF, 2, 0)
#define ZMOH_OUTH_SET(x)        VAL_SET(x, 0xFF, 2, 0)

#define ZMHH_ZSPEED_SET(x)      VAL_SET(x, 0x0F, 0, 4)
#define ZMHH_OUTH_SET(x)        VAL_SET(x, 0x1, (8+2), 2)
#define ZMHH_OUTW_SET(x)        VAL_SET(x, 0x3, (8+2), 0)

// *********************************************************

// Chip ID Values
#define OV2640_ID           (0x00)

// Resolution
#define CIF_WIDTH   (400)
#define CIF_HEIGHT  (296)
#define VGA_WIDTH	(640)
#define VGA_HEIGHT	(480)
#define SVGA_WIDTH  (800)
#define SVGA_HEIGHT (600)
#define SXGA_WIDTH  (1280)
#define SXGA_HEIGHT (1024)
#define UXGA_WIDTH  (1600)
#define UXGA_HEIGHT (1200)

typedef enum {
    FRAMESIZE_INVALID = 0,
    // C/SIF Resolutions
    FRAMESIZE_QQCIF,    // 88x72    *       25,344Byte できた
    FRAMESIZE_QCIF,     // 176x144  *      101,376Byte できた
    FRAMESIZE_CIF,      // 352x288  -      405,504Byte できるはずだが無理
    FRAMESIZE_QQSIF,    // 88x60    *       21,120Byte できた
    FRAMESIZE_QSIF,     // 176x120  *       84,480Byte できた
    FRAMESIZE_SIF,      // 352x240         337,920Byte
    // VGA Resolutions
    FRAMESIZE_QQQQVGA,  // 40x30             4,800Byte
    FRAMESIZE_QQQVGA,   // 80x60            19,200Byte
    FRAMESIZE_QQVGA,    // 160x120  *       76,800Byte できた
    FRAMESIZE_QVGA,     // 320x240  -      307,200Byte できるはずだが無理
    FRAMESIZE_VGA,      // 640x480  -    1,228,800Byte　無理
    FRAMESIZE_HQQQQVGA, // 30x20             2,400Byte
    FRAMESIZE_HQQQVGA,  // 60x40             9,600Byte
    FRAMESIZE_HQQVGA,   // 120x80           38,400Byte
    FRAMESIZE_HQVGA,    // 240x160  *      153,600Byte できた
    FRAMESIZE_HVGA,     // 480x320         614,400Byte　無理
    // Other
    FRAMESIZE_LCD,      // 128x160 *        81,920Byte できた
    FRAMESIZE_QQVGA2,   // 128x160 *        81,920Byte できた
    FRAMESIZE_WVGA,     // 720x480		 1,382,400Byte 無理
    FRAMESIZE_WVGA2,    // 752x480                     無理
    FRAMESIZE_SVGA,     // 800x600                     無理
    FRAMESIZE_XGA,      // 1024x768                    無理
    FRAMESIZE_WXGA,     // 1280x768                    無理
    FRAMESIZE_SXGA,     // 1280x1024                   無理
    FRAMESIZE_SXGAM,    // 1280x960                    無理
	TEST_SIZE,
} framesize_t;

typedef enum {
    GAINCEILING_2X,
    GAINCEILING_4X,
    GAINCEILING_8X,
    GAINCEILING_16X,
    GAINCEILING_32X,
    GAINCEILING_64X,
    GAINCEILING_128X,
} gainceiling_t;

typedef enum {
    SENSOR_ERROR_NO_ERROR               =  0,
    SENSOR_ERROR_CTL_FAILED             = -1,
    SENSOR_ERROR_CTL_UNSUPPORTED        = -2,
    SENSOR_ERROR_ISC_UNDETECTED         = -3,
    SENSOR_ERROR_ISC_UNSUPPORTED        = -4,
    SENSOR_ERROR_ISC_INIT_FAILED        = -5,
    SENSOR_ERROR_TIM_INIT_FAILED        = -6,
    SENSOR_ERROR_DMA_INIT_FAILED        = -7,
    SENSOR_ERROR_DCMI_INIT_FAILED       = -8,
    SENSOR_ERROR_IO_ERROR               = -9,
    SENSOR_ERROR_CAPTURE_FAILED         = -10,
    SENSOR_ERROR_CAPTURE_TIMEOUT        = -11,
    SENSOR_ERROR_INVALID_FRAMESIZE      = -12,
    SENSOR_ERROR_INVALID_PIXFORMAT      = -13,
    SENSOR_ERROR_INVALID_WINDOW         = -14,
    SENSOR_ERROR_INVALID_FRAMERATE      = -15,
    SENSOR_ERROR_INVALID_ARGUMENT       = -16,
    SENSOR_ERROR_PIXFORMAT_UNSUPPORTED  = -17,
    SENSOR_ERROR_FRAMEBUFFER_ERROR      = -18,
    SENSOR_ERROR_FRAMEBUFFER_OVERFLOW   = -19,
    SENSOR_ERROR_JPEG_OVERFLOW          = -20,
} sensor_error_t;

#endif /* SRC_USER_DRIVERS_SENSOR_SUBCAM_PARAM_H_ */
