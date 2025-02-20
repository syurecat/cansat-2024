// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>
#include <SNS_BME280>

/*** Config part */
// application ID
const uint32_t APP_ID = 0x1234abcd;

// channel
const uint8_t CHANNEL = 13;

// DIO pins
const uint8_t PIN_BTN = 12;

/*** function prototype */
void vTransmit(const char* msg, uint32_t addr);

/*** application defs */
// packet message
SNS_BME280 sns_bme280;

enum class STATE {
    INIT = 0,
    CAPTURE_PRE,
    CAPTURE,
    TX,
    TX_WAIT_COMP,
    SUCCESS,
    ERROR
};
STATE State = STATE::INIT;

/*** setup procedure (run once at cold boot) */
void setup() {
	/*** SETUP section */
    Wire.begin();
    sns_bme280.setup();

	// the twelite main class
	the_twelite
		<< TWENET::appid(APP_ID)    // set application ID (identify network group)
		<< TWENET::channel(CHANNEL) // set channel (pysical channel)
		<< TWENET::rx_when_idle();  // open receive circuit (if not set, it can't listen packts from others)

	// Register Network
	auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	nwksmpl << NWK_SIMPLE::logical_id(0xFE) // set Logical ID. (0xFE means a child device with no ID)
	        << NWK_SIMPLE::repeat_max(3);   // can repeat a packet up to three times. (being kind of a router)

	the_twelite.begin(); // start twelite!

	/*** INIT message */
	Serial << "init done";
}

/*** loop procedure (called every event) */
void loop() {
	do {
		new_state = false;
		case STATE::INIT: // starting state
			// start sensor capture
			sns_bme280.begin();
			State =  STATE::CAPTURE_PRE;
		break;

		case STATE::CAPTURE_PRE: // wait for sensor capture completion
			if (TickTimer.available()) {
				sns_bme280.process_ev(E_EVENT_TICK_TIMER);
			}
			new_state = true;
			State =  STATE::CAPTURE;
		break;

		case STATE::CAPTURE:

			// get new packet instance.
			State = STATE::ERROR; // change this when success TX request...

			if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
				// set tx packet behavior
				pkt << tx_addr(0xFF)  // 0..0xFF (LID 0:parent, FE:child w/ no id, FF:LID broad cast), 0x8XXXXXXX (long address)
					<< tx_retry(0x1) // set retry (0x1 send two times in total)
					<< tx_packet_delay(0, 0, 2); // send packet w/ delay

				// prepare packet payload
				pack_bytes(pkt.get_payload() // set payload data objects.
					, make_pair(FOURCHARS, 4)  // just to see packet identification, you can design in any.
					, uint16_t(sns_bme280.get_temp_cent()) // temp
					, uint16_t(sns_bme280.get_humid_per_dmil())
					, uint16_t(sns_bme280.get_press())
				);

				// do transmit
				MWX_APIRET ret = pkt.transmit();
			} else {
				Serial << crlf << "!FATAL: MWX TX OBJECT FAILS. reset the system." << crlf;
			}
		break;

		case STATE::TX_WAIT_COMP: // wait for complete of transmit
			if (the_twelite.tx_status.is_complete(u8txid)) {
				Serial << crlf << format("..%04d/transmit complete.", millis() & 8191);
		
				// success on TX
				State = STATE::SUCCESS;
				new_state = true;
			} else if (millis() - u32tick_tx > 3000) {
				Serial << crlf << "!FATAL: MWX TX OBJECT FAILS. reset the system." << crlf;
				State = STATE::ERROR;
				new_state = true;
			} 
		break;

		case STATE::ERROR: // FATAL ERROR
			Serial.flush();
			delay(100);
			the_twelite.reset_system();
		break;

		case STATE::SUCCESS: // NORMAL EXIT (go into sleeping...)
			sleepNow();
		break;

	} while(step.b_more_loop()); // if state is changed, loop more.
}

void sleepNow() {
	uint32_t u32ct = 1750 + random(0,500);
	Serial << crlf << format("..%04d/sleeping %dms.", millis() % 8191, u32ct);
	Serial.flush();

	the_twelite.sleep(u32ct);
}

// wakeup procedure
void wakeup() {
	Wire.begin();

	Serial	<< crlf << "--- " << APP_NAME << ":" << FOURCHARS << " wake up ---";

	State = STATE::INIT; // go into INIT state in the loop()
}