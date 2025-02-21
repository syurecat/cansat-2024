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
	auto&& set = the_twelite.settings.use<STG_STD>();

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
}

/*** loop procedure (called every event) */
void loop() {
}
void on_rx_packet(packet_rx& rx, bool_t &handled) {

    smplbuf_u8<128> buf;
    mwx::pack_bytes(buf, rx.get_payload());

    // シリアル出力
    for (size_t i = 0; i < buf.size(); i++) {
        char c = buf[i];
        if (isprint(c)) {
            Serial << c;
        } else {
            Serial << ".";
        }
    }
    Serial.println(rx.get_lqi());
}