 /*
 * SUNS.cpp
 *
 *  Created on: 2021/05/09
 *      Author: Hiroaki MASUDA
 *
 *****************************************************************************************
 *
 * COPYRIGHT(c) 2020 Aerospace Vehicle Dynamics Research Group, Nagoya University
 *
 * explain
 *
 *
 * UPDATE HISTORY
 * Date        Rev   Editor          Description
 * 2021/05/09  1.0   H.MASUDA      First revision
 *
  *****************************************************************************************
 */
#include <src_user/Drivers/Aocs/AOCScalc.h>
#include <src_user/Drivers/SENSOR/SUNS.h>
#include <src_user/Library/Delay.h>
//#include <src_core/System/AnomalyLogger/anomaly_logger.h>
#include <src_user/Library/General/DBG_printf.h>


#include <string.h>
#include <math.h>


//-----------------------------------------
// センサ依存変数宣言
//-----------------------------------------
//#defineしても良いため大文字で記述しておく
const uint8_t COMMAND_UNFILTERED_CELLS = 0x01;
const uint8_t COMMAND_FILTERED_CELLS = 0x03;
const uint8_t COMMAND_ANGULAR_POSITION = 0x04;
const uint8_t COMMAND_SYNC_WORD[] = { 0x1A, 0xCF, 0xFC, 0x1D };

static uint8_t suns_checksum(uint8_t *data, uint32_t const size);
static float hexToFloat(uint8_t *data);
static void SUN_SENSOR_nanoSSOCD60_SPI_Transmit_make_message(uint8_t command, uint8_t *msg, uint8_t length);
static int SUN_SENSOR_nanoSSOCD60_get_unfiltered_cells(SunSensorDriver* sun_driver, float out_data[4]);
static int SUN_SENSOR_nanoSSOCD60_get_filtered_cells(SunSensorDriver* sun_driver, float out_data[4]);
static void SUNS_Vector_Calc_Correct(float alpha, float beta, float SUNSensorVec[3]);

/**
 * @brief  nanoSSOCD60サンセンサに送信するメッセージを作成する関数
 * @param command 現状１Byteのみに対応、コマンド長さが変化しうる場合はそれに対応した実装が必要
 * @param msg 生成したメッセージの格納配列のポインタ
 * @param length　生成したメッセージの長さを格納する変数ポインタ//ポインタ?
 * @return  なし　生成したメッセージは配列に格納
 *
 * 結局生成られるメッセージはコマンドに対して固定値なので、
 * メッセージを固定値として宣言し、各メンバ関数でその固定値を読み出し送信するのも良い
 * 送信コマンドの種類がもっと増えたり、パラメータを送信するのであれば、このように関数で作成するようにすべき
 *
 */
static void SUN_SENSOR_nanoSSOCD60_SPI_Transmit_make_message(uint8_t command, uint8_t *msg, uint8_t length)
{
	uint8_t command_length = 1;
	uint8_t commands[] = {(uint8_t) command, (uint8_t) sizeof(command)};
	uint8_t sum = suns_checksum(commands, sizeof(commands));

	// message to be sent to the sun sensor
	// EX: SYNC_WORD(4bytes) + command(1byte) + length(1byte) + checksum(1byte)

	memcpy(msg, COMMAND_SYNC_WORD, sizeof(COMMAND_SYNC_WORD));
	length = sizeof(COMMAND_SYNC_WORD);
	memcpy(&msg[length], &command, command_length);
	length += command_length;
	memcpy(&msg[length], &command_length, 1);
	length += sizeof(command_length);
	memcpy(&msg[length], &sum, 1);
	length += sizeof(sum);
}


/**
 * @brief  サンセンサの初期化、CSピンをHighに設定し、データに0.0を格納。
 */
int SUN_SENSOR_init(SunSensorDriver* sun_driver, void* spi_port, void* gpio_port, uint16_t gpio_pin)
{
	sun_driver->spi_port = spi_port;
	sun_driver->gpio_port = gpio_port;
	sun_driver->gpio_pin = gpio_pin;

	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_SET);

	for (int i = 0; i < 4; i++) {
		sun_driver->unfiltered_photocell_voltage[i] = 0.0f;
		sun_driver->filtered_photocell_voltage[i] = 0.0f;
	}
	for (int i = 0; i < 2; i++) {
		sun_driver->angular_position[i] = 0.0f;
	}

	return 0;
}


/**
 * @brief  nanoSSOCD60サンセンサから電圧unfiltered_cellsを取得し、出力する
 * 　　　　　出力はoutdataに格納される
 * @param out_data 出力先の配列　&float[4] である必要がある
 * @return  正常終了なら0,SUN_SENSOR_StateTypeDef参照
 */
static int SUN_SENSOR_nanoSSOCD60_get_unfiltered_cells(SunSensorDriver* sun_driver, float out_data[4])
 {
	uint8_t msg[7];
	uint8_t trnsize = 7;
	uint8_t recsize = 23;
	uint8_t rec[23];
	//uint8_t recSum = 0;
	uint8_t ret = 0;

	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_SET);

	//  unfiltered_cell取得用メッセージの作成
	SUN_SENSOR_nanoSSOCD60_SPI_Transmit_make_message(COMMAND_UNFILTERED_CELLS, msg, trnsize);

	//transmit　message
	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_RESET);
	ret = HAL_SPI_Transmit_C2A(sun_driver->spi_port, msg, trnsize, SUN_SENSOR_SPI_TIMEOUT);
	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_SET);
	if(ret != 0) {
		sun_driver->state = SPI_ERROR;
		return ret;
	}

	//SUNセンサーが要求する　CS 立ち上がり持続時間　待つ
	c2a_delay_ms(1);//<-長いのではないでしょうか


	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_RESET);
	ret = HAL_SPI_Receive_C2A(sun_driver->spi_port, rec, recsize, SUN_SENSOR_SPI_TIMEOUT);
	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_SET);
	if(ret != 0) {
		sun_driver->state = SPI_ERROR;
		return ret;
	}

	uint8_t recSum = suns_checksum(&rec[4], recsize - 5);

	if (recSum != rec[recsize - 1]) {
		// Anomaly detection
		print(ANOMALY,"SUNS_CHECKSUM_ERROR_add_anomaly \r\n");
//		AL_add_anomaly(AL_GROUP_SUNS,CHECKSUM_ERROR);
		return sun_driver->state = CHECKSUM_ERROR;
	}

	//受信データの変換
	for (int i = 0; i < 4; i++) {
		float f = hexToFloat(&rec[6 + i * 4]); // or hexToFloatSwap()
		out_data[i] = f;
//		printf("%f[mV],", out_data[i]);
	}

	return sun_driver->state = NO_ERROR;

}


/**
* @brief  サンセンサから電圧unfiltered_cellsを取得し、sunに格納する。
* @return  正常終了で0を返す。0以外はエラーコード。
* 			異常の詳細はSUN_SENSOR_STRUCT構造体のstateを参照。
*/
int SUN_SENSOR_get_unfiltered_cells(SunSensorDriver* sun_driver)
{
	float unfiltered_photocell_voltage[4];
	//センサー依存関数の呼び出し
	int ret = SUN_SENSOR_nanoSSOCD60_get_unfiltered_cells(sun_driver, unfiltered_photocell_voltage);

	//20210509 inakawa エラーが返ってきたらデータを初期化すべき
	if(ret != 0){
		for (int i = 0; i < 4; i++) {
			sun_driver->unfiltered_photocell_voltage[i] = 0.0f;
		}
	}
	else{
		for (int i = 0; i < 4; i++) {
			sun_driver->unfiltered_photocell_voltage[i] = unfiltered_photocell_voltage[i];
		}
	}
	return ret;
}


/**
 * @brief  nanoSSOCD60サンセンサから電圧filtered_cellsを取得し、出力する
 * 出力はoutdataに格納される
 * @param out_data 出力先の配列　&float[4] である必要がある
 * @return  正常終了なら0,SUN_SENSOR_StateTypeDef参照
 */
static int SUN_SENSOR_nanoSSOCD60_get_filtered_cells(SunSensorDriver* sun_driver, float out_data[4])
{
	uint8_t msg[7];
	uint8_t trnsize = 7;
	uint8_t recsize = 23;
	uint8_t rec[23];
	uint8_t recSum;
	uint8_t ret = 0;

	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_SET);

	//  unfiltered_cell取得用メッセージの作成
	SUN_SENSOR_nanoSSOCD60_SPI_Transmit_make_message( COMMAND_FILTERED_CELLS, msg, trnsize);

	//transmit　message
	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_RESET);
	ret = HAL_SPI_Transmit_C2A(sun_driver->spi_port, msg, trnsize, SUN_SENSOR_SPI_TIMEOUT);
	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_SET);
	if(ret != 0) {
		sun_driver->state = SPI_ERROR;
		return ret;
	}
	//SUNセンサーが要求する　CS 立ち上がり持続時間　待つ
	c2a_delay_ms(1);//<-長いのではないでしょうか

	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_RESET);
	ret = HAL_SPI_Receive_C2A(sun_driver->spi_port, rec, recsize, SUN_SENSOR_SPI_TIMEOUT);
	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_SET);
	if(ret != 0) {
		sun_driver->state = SPI_ERROR;
		return ret;
	}

	recSum = suns_checksum(&rec[4], recsize - 5);
	if (recSum != rec[recsize - 1]) {
		// Anomaly detection
		print(ANOMALY,"SUNS_CHECKSUM_ERROR_add_anomaly \r\n");
//		AL_add_anomaly(AL_GROUP_SUNS,CHECKSUM_ERROR);
		return sun_driver->state = CHECKSUM_ERROR;
	}

	//受信データの変換
	for (int i = 0; i < 4; i++) {
		float f = hexToFloat(&rec[6 + i * 4]); // or hexToFloatSwap()
		out_data[i] = f;
	}

	return sun_driver->state = NO_ERROR;
}


/**
 * @brief  サンセンサから電圧filtered_cellsを取得し、sunに格納する。
 * @param  sun 個々のサンセンサを示すポインタ。
 * @return  正常終了で0を返す。
 * 			異常の詳細はSUN_SENSOR_STRUCT構造体のstateを参照。
 */
int SUN_SENSOR_get_filtered_cells(SunSensorDriver* sun_driver)
{

	float filtered_photocell_voltage[4];
	//センサー依存関数の呼び出し
	int ret = SUN_SENSOR_nanoSSOCD60_get_filtered_cells(sun_driver, filtered_photocell_voltage);

	//20210509 inakawa エラーが返ってきたらデータを初期化すべき
	if(ret != 0){
		for (int i = 0; i < 4; i++) {
			sun_driver->filtered_photocell_voltage[i] = 0.0f;
		}
	}
	else{
		for (int i = 0; i < 4; i++) {
			sun_driver->filtered_photocell_voltage[i] = filtered_photocell_voltage[i];
		}
	}
	return ret;
}


/**
 * @brief  nanoSSOCD60サンセンサから角度を取得し、出力する
 * 　　　　　出力はoutdataに格納される
 * @param out_data 出力先の配列　&float[2] である必要がある
 * @return  正常終了なら0,SUN_SENSOR_StateTypeDef参照
 */
int SUN_SENSOR_nanoSSOCD60_angular_position(SunSensorDriver* sun_driver, float out_data[2])
{
	uint8_t msg[7];
	uint8_t trnsize = 7;
	uint8_t recsize = 20;
	uint8_t rec[20];
	uint8_t recSum;
	uint8_t ret;

	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_SET);
	//メッセージの作成
	SUN_SENSOR_nanoSSOCD60_SPI_Transmit_make_message( COMMAND_ANGULAR_POSITION, msg, trnsize);

	//transmit　message
	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_RESET);
	ret = HAL_SPI_Transmit_C2A(sun_driver->spi_port, msg, trnsize, SUN_SENSOR_SPI_TIMEOUT);
	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_SET);
	if(ret != 0) {
		sun_driver->state = SPI_ERROR;
		return ret;
	}

	c2a_delay_ms(5);     //5ms以上じゃないとたまに0となる


	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_RESET);
	ret = HAL_SPI_Receive_C2A(sun_driver->spi_port, rec, recsize, SUN_SENSOR_SPI_TIMEOUT);
	HAL_GPIO_WritePin_C2A(sun_driver->gpio_port, sun_driver->gpio_pin, GPIO_PIN_SET);
	if(ret != 0) {
		sun_driver->state = SPI_ERROR;
		return ret;
	}

	recSum = suns_checksum(&rec[4], recsize - 5);

	if (recSum != rec[recsize - 1]) {
		// Anomaly detection
		print(ANOMALY,"SUNS_CHECKSUM_ERROR_add_anomaly \r\n");
//		AL_add_anomaly(AL_GROUP_SUNS,CHECKSUM_ERROR);
		return sun_driver->state = CHECKSUM_ERROR;
	}

//	// アルベドエラーチェック
//	uint8_t errorCode = rec[14];
//	if (errorCode != 0) {
//		if (errorCode == 11) {
//			return this->state = ALBEDO_EARTH_ERROR;
//		} else if (errorCode == 12) {
//			return this->state = ALBEDO_EARTH_SUN_ERROR;
//		} else {
//			return this->state = OTHER_ERROR;
//		}
//	}

	//受信データの変換
	for (int i = 0; i < 2; i++) {
		float f = hexToFloat(&rec[6 + i * 4]); // or hexToFloatSwap()
		out_data[i] = f;
	}

	return sun_driver->state = NO_ERROR;
}


/**
 * @brief  サンセンサからangular_positionを取得し、sunに格納する。また太陽方向ベクトルも求める
 * @param  sun 個々のサンセンサを示すポインタ。
 * @return  正常終了で0を返す。
 * 			異常の詳細はSUN_SENSOR_STRUCT構造体のstateを参照。
 */
int SUN_SENSOR_get_angular_position(SunSensorDriver* sun_driver)
{
	float angular_position[2];
	//センサー依存関数の呼び出し
	int ret = SUN_SENSOR_nanoSSOCD60_angular_position(sun_driver, angular_position);

	//20210509 inakawa エラーが返ってきたらデータを初期化すべき
	if(ret != 0){
		for (int i = 0; i < 2; i++) {
			sun_driver->angular_position[i] = 0.0f;
		}
	}
	else{
		for (int i = 0; i < 2; i++) {
			sun_driver->angular_position[i] = angular_position[i];
		}
	}

	SUNS_Vector_Calc(sun_driver->angular_position[0], sun_driver->angular_position[1], sun_driver->SUN_Vec);
//	SUNS_Vector_Calc_Correct(sun_driver->angular_position[0], sun_driver->angular_position[1], sun_driver->SUN_Vec);
//	printf("sun_vec = %f, %f, %f\r\n", sun_driver->SUN_Vec[0], sun_driver->SUN_Vec[1], sun_driver->SUN_Vec[2]);

	return ret;

}


void SUN_SENSOR_get_time(SunSensorDriver* sun_driver)
{
	sun_driver->sunsensor_time_before = sun_driver->sunsensor_time_now;
	sun_driver->sunsensor_time_now = TMGR_get_master_clock();
}


#ifdef HILS
void SUN_SENSOR_get_time_HILS(SunSensorDriver* sun_driver)
{
	sun_driver->sunsensor_time_before_hils = sun_driver->sunsensor_time_now_hils;
	sun_driver->sunsensor_time_now_hils = TMGR_get_master_clock();
}
#endif


/**
 * @brief  サンセンサから全データを取得し、sunに格納する。
 * @param  sun 個々のサンセンサを示すポインタ。
 * @return  正常終了で0を返す。
 * 			異常の詳細はSUN_SENSOR_STRUCT構造体のstateを参照。
 */
int SUN_SENSOR_get_all(SunSensorDriver* sun_driver)
{

	if (SUN_SENSOR_get_unfiltered_cells(sun_driver)) {
		return sun_driver->state;

	}
	if (SUN_SENSOR_get_filtered_cells(sun_driver)) {
		return sun_driver->state;

	}
	if (SUN_SENSOR_get_angular_position(sun_driver)) {
		return sun_driver->state;

	}
	SUN_SENSOR_get_time(sun_driver);

	return NO_ERROR;
}


/**
 * @brief  サンセンサから角度情報のみ取得し、sunに格納する。（高速スピンモード用）
 * @param  sun 個々のサンセンサを示すポインタ。
 * @return  正常終了で0を返す。
 * 			異常の詳細はSUN_SENSOR_STRUCT構造体のstateを参照。
 */
int SUN_SENSOR_get_only_angle(SunSensorDriver* sun_driver)
{
	if (SUN_SENSOR_get_angular_position(sun_driver)) {
		return sun_driver->state;
	}
	SUN_SENSOR_get_time(sun_driver);

	return NO_ERROR;
}


/**
  * @name   :SUN_Vector_DCM_Sensor2Body_Calc
  * @brief  :センサ座標系ベクトルから機体座標系ベクトルへの変換
  * @param  :
  * @retval :
  */
void SUN_Vector_DCM_Sensor2Body_Calc(SunSensorDriver* sun_driver, float SUNBodyVec[3])
{

	// convert sensor frame vector to body frame vector
	multiplication(&sun_driver->SUNS_DCM[0][0], 3, 3, sun_driver->SUN_Vec, 3, 1, SUNBodyVec, 3, 1);

//	printf("SUNSSensorVec: %f %f %f\r\n",sun_driver->SUN_Vec[0],sun_driver->SUN_Vec[1],sun_driver->SUN_Vec[2]);
//	printf("SUNSBodyVec: %f %f %f\r\n",SUNBodyVec[0],SUNBodyVec[1],SUNBodyVec[2]);

}


static uint8_t suns_checksum(uint8_t *data, uint32_t const size){
	uint32_t pos = 0;
	uint8_t sum = 0;
	while (pos < size) {
		sum += *(data + pos);
		pos++;
	}
	return sum;
}

static float hexToFloat(uint8_t *data) {
	uint8_t ret[] = { data[0], data[1], data[2], data[3] };
	return *((float*) ret);
}


/**
  * @name   :SUNS_Vector_Calc
  * @brief  :極座標におけるΦ,θから正規化されたベクトルを求める
  * @param  :
  * @retval :
  */
void SUNS_Vector_Calc(float alpha, float beta, float SUNSensorVec[3])
{
	float bunbo = sqrt(1.0f + (tan((alpha)*M_PI/180.0f))*(tan((alpha)*M_PI/180.0f)) + (tan((beta)*M_PI/180.0f))*(tan((beta)*M_PI/180.0f)));
	SUNSensorVec[0] = tan(alpha*M_PI/180.0f) / bunbo;
	SUNSensorVec[1] = tan(beta*M_PI/180.0f) / bunbo;
	SUNSensorVec[2] = 1.0f / bunbo;
}


static void SUNS_Vector_Calc_Correct(float alpha, float beta, float SUNSensorVec[3]) ////k_error tamaoki add
{
	float k_error = 0.1f;//mram
	float bunbo = sqrt(1.0f + (tan((alpha + k_error)*M_PI/180.0f))*(tan((alpha + k_error)*M_PI/180.0f)) + (tan((beta + k_error)*M_PI/180.0f))*(tan((beta + k_error)*M_PI/180.0f)));
	SUNSensorVec[0] = tan(alpha*M_PI/180.0f) / bunbo;
	SUNSensorVec[1] = tan(beta*M_PI/180.0f) / bunbo;
	SUNSensorVec[2] = 1.0f / bunbo;
}


//
//float hexToFloatSwap(uint8_t *data) {
//	uint8_t ret[] = { data[3], data[2], data[1], data[0] };
//	return *((float*) ret);
//}


//void SUNS_CLASS::SUN_Vector_Alignment_Correct(float phi, float theta, float psy, float SUN_Vec[3])
//{
//	// 3-2-3 Euler angle DCM (clockwise rotation angle is positive)
//	DCM_Euler[0][0] =  cos(phi)*cos()*cos() - sin()*sin();  DCM_Euler[0][1] = sin(phi)*cos()*cos() + cos()*sin();  DCM_Euler[0][2] = -sin()*cos();
//	DCM_Euler[1][0] = -sin(phi)*cos() - cos()*cos()*sin();  DCM_Euler[1][1] = cos(phi)*cos() - sin(phi)*cos()*sin();  DCM_Euler[1][2] = sin()*sin();
//	DCM_Euler[2][0] =  cos(phi)*sin();                      DCM_Euler[2][1] = sin()*sin();                      DCM_Euler[2][2] = cos();
//}
