// use twelite mwx c++ template library
#include <TWELITE> 
#include <NWK_SIMPLE>
#include <MONOSTICK>


/*** Config part */
// application ID
const uint32_t APP_ID = 0x1234abcd;
// channel
const uint8_t CHANNEL = 13;
/*** function prototype */
bool analyze_payload(packet_rx& rx);

/*** application defs */

/*** setup procedure (run once at cold boot) */
void setup() {
	/*** SETUP section */
	auto&& brd = the_twelite.board.use<MONOSTICK>();

	// Register Network
	auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(0x00) // set Logical ID. (0xFE means a child device with no ID)
	        << NWK_SIMPLE::repeat_max(3);   // can repeat a packet up to three times. (being kind of a router)

	// the twelite main class
	the_twelite
	<< TWENET::appid(APP_ID)    // set application ID (identify network group)
	<< TWENET::channel(CHANNEL) // set channel (pysical channel)
	<< TWENET::rx_when_idle(1);  // open receive circuit (if not set, it can't listen packts from others)

	// configure hardware
	brd.set_led_red(LED_TIMER::ON_RX, 200); // RED (on receiving)
	brd.set_led_yellow(LED_TIMER::BLINK, 500); // YELLOW (blinking)

	/*** BEGIN section */
	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "--- MONOSTICK_Parent act ---";
}

/*** loop procedure (called every event) */
void loop() {
}
void on_rx_packet(packet_rx& rx, bool_t &handled) {
    const uint8_t* p = rx.get_payload().begin(); // 受信したパケットのペイロードの先頭ポインタを取得
    if (rx.get_length() > 4) {
        // パケットの長さが4より大きい場合
        smplbuf_u8<128> buf; // サイズ128のバッファを作成
        mwx::pack_bytes(buf,
            uint8_t(rx.get_lqi()), // 信号品質指標
            make_pair(p + 4, rx.get_payload().end()) // ペイロードのデータ部分（ヘッダー以外）
        );

        serparser_attach pout; // シリアルパーサーのインスタンスを作成
        pout.begin(PARSER::ASCII, buf.begin(), buf.size(), buf.size()); // パーサーをASCII形式で初期化
        Serial << pout; // パーサーの内容をシリアル出力に送信
    }
}