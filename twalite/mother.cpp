// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>
#include <STG_STD>

/*** Config part */
// application ID
const uint32_t APP_ID = 0x1234abcd;
// channel
const uint8_t CHANNEL = 13;
/*** function prototype */
MWX_APIRET transmit(const uint8_t* b, const uint8_t* e);

uint8_t inputBuffer[128];
int bufferPos = 0;

/*** setup procedure (run once at cold boot) */
void setup() {

	Serial.begin(19200);
	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle(1);  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(0x01) // set Logical ID. (0xFE means a child device with no ID)
	        << NWK_SIMPLE::repeat_max(3);   // can repeat a packet up to three times. (being kind of a router)

	/*** BEGIN section */
	SerialParser.begin(PARSER::ASCII, 128); // Initialize the serial parser
	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "init done";
}

/*** loop procedure (called every event) */
void loop() {
  // シリアルからの入力を1文字ずつ読み出す
  while (Serial.available() > 0) {
    int c = Serial.read();
    Serial << "c = " << c << crlf;
    
    // CR (13) または LF (10) が来たら改行とみなし、送信処理を実施
    if (c == 13 || c == 10) {
      if (bufferPos > 1) { // 空行の場合は無視
        // 受信した文字列を送信
        Serial << "Sending: " << inputBuffer << crlf;
        transmit((uint8_t*)inputBuffer, (uint8_t*)(inputBuffer + bufferPos));
        bufferPos = 0;  // バッファをリセット
		inputBuffer[bufferPos++] = 1;
      }
    }
    else {
      // バッファサイズに余裕があれば受信文字を蓄積
      if (bufferPos < (int)(sizeof(inputBuffer) - 1)) {
        inputBuffer[bufferPos++] = (char)c;
        inputBuffer[bufferPos] = '\0'; // デバッグ用に文字列終了をセット
      }
      else {
        Serial << "Buffer overflow, resetting buffer" << crlf;
        bufferPos = 0;
		inputBuffer[bufferPos++] = 1;
      }
    }
  }
}

/** transmit a packet */
MWX_APIRET transmit(const uint8_t* b, const uint8_t* e) {
	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		// set tx packet behavior
		pkt << tx_addr(0x00) // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x3) // set retry (0x3 send four times in total)
			<< tx_packet_delay(20,100,10); // send packet w/ delay (send first packet with randomized delay from 20 to 100ms, repeat every 10ms)

		// prepare packet payload
		pack_bytes(pkt.get_payload() // set payload data objects.
			, make_pair(b, e) // put timestamp here.
		);
		
		// do transmit 
		return pkt.transmit(); 
	}

	return false;
}

/** on receiving a packet */
void on_rx_packet(packet_rx& rx, bool_t &handled) {
    // check the packet header.
    const uint8_t* p = rx.get_payload().begin();
	Serial << format("..rx from %08x/%d", rx.get_addr_src_long(), rx.get_addr_src_lid()) << mwx::crlf;

	const uint8_t* data_start = p;
	const uint8_t* data_end = rx.get_payload().end();

	transmit(data_start, data_end);
}
