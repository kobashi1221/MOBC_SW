#include "Circular_buffer.h"
// UART割込みで用いるPakcet群のリングバッファー用関数
// キューを初期化する
void queue_init(Queue* queue, size_t length, Buffer* buf) {
    queue_reset(&queue->list);
    queue->list.length = length;
    queue->list.buffer = buf;
    queue_count_reset(queue);
}

void queue_reset(Queue_LIST* list){
    list->tail = 0;
    list->head = 0;
}

void queue_count_reset(Queue* queue){
	queue->dri.byte_count=0;
	queue->dri.len=0;
}

// キューが空かどうかを判定する
QUEUE_ACK isEmpty(Queue_LIST* list) {
    return (QUEUE_ACK)(list->head == list->tail);
    //todo : type
}

// スタックが満杯かどうかを判定する
QUEUE_ACK isFull(Queue_LIST* list) {
    return (QUEUE_ACK)(list->head == ((list->tail + 1) % (list->length)));
    //todo : type
}

// enqueue (tail に要素を格納してインクリメント)
QUEUE_ACK enqueue(Queue_LIST* list, uint8_t* tcp) {
    if (isFull(list)) return QUEUE_IS_FULL;
    memcpy((list->buffer[list->tail++].data),tcp,get_packet_len(tcp));
//    list->tail++;//先に足すべきか，後に足すべきか．．．．．
    if (list->tail == list->length) list->tail = 0; // リングバッファの終端に来たら 0 に
    return QUEUE_OK;
}

// dequeue (head にある要素を返して head をインクリメント)
QUEUE_ACK dequeue(Queue_LIST* list, TCP* tcp) {
	if (isEmpty(list)) return QUEUE_IS_EMPTY;
	size_t len=get_packet_len(list->buffer[list->head].data);
    memcpy(tcp->packet, list->buffer[list->head].data, len);
    //パース終了の後にHeadを増やさないと，実際のパース前にTailからデータが上書きされ得る
    list->head++;
    if (list->head == list->length) list->head = 0;
    return QUEUE_OK;
}

size_t get_packet_len(uint8_t* packet){
	size_t packet_len=(packet[4]<<8)+packet[5]+7;
	return packet_len;
}

void Callback_(Queue* queue){
		inturrupt_driver* dri=&(queue->dri);
		ObcTime currenttime=TMGR_get_master_clock();
		//本当にここでいいか要検討　Noro
		if(dri->byte_count>0 && OBCT_diff_in_msec(&dri->first_rcv_time ,&currenttime)>500){
			queue_count_reset(queue);
		}
		dri->receive_buf[dri->byte_count]=dri->byte_buf;
		dri->byte_count++;

		if(dri->byte_count==1){
			dri->first_rcv_time=TMGR_get_master_clock();
			//1バイト目が正しくない
			if(!((dri->receive_buf[0])==0b00101001 || (dri->receive_buf[0])==0b00111001)){
				dri->byte_count=0;
			}
		}
		//Packet_lenまで来た時，受信終了　時間がシビアなので，IF文の先頭の方に持っていく
		else if(dri->byte_count>15 && dri->byte_count==dri->len){
			//パケットがいっぱいだと登録されずそのままBufferが初期化
			enqueue(&(queue->list),dri->receive_buf);
			queue_count_reset(queue);
		}
		//5バイト目が受信し終わった後，Byte Countのインクリメントをした後（Bytecount==6）のとき，パケット長を取得
		else if(dri->byte_count==6){
			dri->len=get_packet_len(dri->receive_buf);
		}
		//最大パケット長さを超える
		else if(dri->byte_count>TCP_MAX_LEN-1){
			queue_count_reset(queue);
		}
}

void Callback_Packet(Queue* queue, size_t Size){
	size_t len=get_packet_len(queue->dri.receive_buf);
	//Todo Ukita チェック増やす
	if(Size > TCP_MAX_LEN || Size < 15) return;//意味わからんのが連続出来てるor少なすぎる
	if(len != Size) return;//実際に受信したパケットとパケットの中身の長さが異なる
	uint8_t first_byte=queue->dri.receive_buf[0];
	if(!(first_byte == 0b00101001 || first_byte == 0b00111001)) return;
	enqueue(&(queue->list),queue->dri.receive_buf);//パケット受信成功
	return;
}


void Spi_Callback_Packet(Queue* queue, size_t Size){
    size_t len=get_packet_len(queue->dri.receive_buf);
    //Todo Ukita チェック増やす
    if(Size > TCP_MAX_LEN || Size < 15){
      return;
    }//意味わからんのが連続出来てるor少なすぎる
    if(len != Size){
      return;
    }//実際に受信したパケットとパケットの中身の長さが異なる(spiの場合受け取るパケットの長さは常時256byte)のため不要
    uint8_t first_byte = queue->dri.receive_buf[0];
    if(!(first_byte == 0b00101001 || first_byte == 0b00111001)){
      return;
    }
    enqueue(&(queue->list),queue->dri.receive_buf);//パケット受信成功
    return;
}
