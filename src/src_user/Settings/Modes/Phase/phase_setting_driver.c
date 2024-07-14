#include "phase_setting_driver.h"

void Judge_phase_flag(PHASE_STRUCT* phase_driver){
	uint8_t ret[3];
	ret[0] = phase_driver->phase_redundancy1;
	ret[1] = phase_driver->phase_redundancy2;
	ret[2] = phase_driver->phase_redundancy3;

    int count_max = 0;     // これまでに調べた登場回数の中で最大のもの

    for (int i = 0; i < 3; ++i) {
        int count = 1;
        for (int j = i + 1; j < 3; ++j) {
            if (ret[i] == ret[j]) {
                ++count;
            }
        }

        // これまでの最大の登場回数よりも多かったら、更新する
        if (count_max <= count) {
            count_max = count;
            phase_driver->current_phase = ret[i];
        }
    }

    if(phase_driver->current_phase >= PHASE_MAX){//意味不明な値が入ったら強制的にクリティカルフェーズへ遷移
    	phase_driver->current_phase = CRITICAL;
    }
}



