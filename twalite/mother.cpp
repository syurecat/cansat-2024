// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>

/*** Config part */
// application ID
const uint32_t APP_ID = 0x1234abcd;
// channel
const uint8_t CHANNEL = 13;

/*** function prototype */
MWX_APIRET transmit(uint8_t addr, const uint8_t* b, const uint8_t* e);

/*** application defs */
const uint8_t FOURCHARS[] = "WURT";

/*** setup procedure (run once at cold boot) */
void setup() {
	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(0x01) // set Logical ID. (0xFE means a child device with no ID)
	        << NWK_SIMPLE::repeat_max(3);   // can repeat a packet up to three times. (being kind of a router)

	/*** BEGIN section */
	SerialParser.begin(PARSER::ASCII, 128); // Initialize the serial parser
	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "init done"
}

/*** loop procedure (called every event) */
void loop() {
    // read from serial
    while(Serial.available())  {
        if (SerialParser.parse(Serial.read())) {
            Serial << ".." << SerialParser;
            const uint8_t* b = SerialParser.get_buf().begin();

            smplbuf_u8<64> binary_data;
            bool first = true;
            uint8_t val;

            for (const uint8_t* p = b; p < SerialParser.get_buf().end(); ++p) {
                if (*p == ',' || p == SerialParser.get_buf().end()-1) {
                    if (!first) binary_data.push_back(val);
                    first = false;
                    val = 0;
                } else if (*p >= '0' && *p <= '9') {
                    val = val * 10 + (*p - '0');
                }
            }

            transmit(addr, binary_data.begin(), binary_data.end());
        }
    }
}

/** transmit a packet */
MWX_APIRET transmit(const uint8_t* b, const uint8_t* e) {
	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		// set tx packet behavior
		pkt << tx_addr(0x00) // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
			<< tx_retry(0x1) // set retry (0x3 send four times in total)
			<< tx_packet_delay(20,100,10); // send packet w/ delay (send first packet with randomized delay from 20 to 100ms, repeat every 10ms)

		// prepare packet payload
		pack_bytes(pkt.get_payload() // set payload data objects.
			, make_pair(FOURCHARS, 4) // string should be paired with length explicitly.
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
    if (rx.get_length() > 4 && !strncmp((const char*)p, (const char*)FOURCHARS, 4)) {
        Serial << format("..rx from %08x/%d", rx.get_addr_src_long(), rx.get_addr_src_lid()) << mwx::crlf;

        // 受信データのバイナリを取得
        const uint8_t* data_start = p + 4;
        const uint8_t* data_end = rx.get_payload().end();

        // 転送先アドレス（例: 0xFFでブロードキャスト、または特定アドレスに変更）
        uint8_t forward_addr = 0xFF;  

        // 受信データをそのまま転送
        transmit(forward_addr, data_start, data_end);
    }

}
