#include <AdafruitGPS.hpp>

bool AdafruitGPS::begin(uart_inst_t* uart_chn, uint32_t baudrate, uint32_t tx_pin, uint32_t rx_pin) {
    uart_id = uart_chn;

    uart_init(uart_chn, baudrate);
    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);

    return true;
}


AdafruitGPS::AdafruitGPS() {
    common_init();
}

AdafruitGPS::~AdafruitGPS() {
	;
}

void AdafruitGPS::common_init(void) {
    recvdflag = false;
    paused = false;
    lineidx = 0;
    currentline = line1;
    lastline = line2;

    hour = minute = seconds = year = month = day = fixquality = fixquality_3d =
    satellites = antenna = 0; // uint8_t
    lat = lon = mag = 0;          // char
    fix = false;                  // bool
    milliseconds = 0;             // uint16_t
    latitude = longitude = geoidheight = altitude = speed = angle = magvariation =
    HDOP = VDOP = PDOP = 0.0; // nmea_float_t		
}

bool AdafruitGPS::available(void) {
    if(paused) return 0;

    return uart_is_readable(uart_id);
}

void AdafruitGPS::write(uint8_t* c) {
    if(!uart_is_writable(uart_id)) return;

    uart_write_blocking(uart_id, c, sizeof(c)); 
}

char AdafruitGPS::read(void) {
    static uint32_t firstChar = 0; // first character received in current sentence
    uint32_t tStart = to_ms_since_boot(get_absolute_time());    // as close as we can get to time char was sent
    char c = 0;

    if (paused || noComms) return c;

    if(!uart_is_readable(uart_id)) return c;

    c = uart_getc(uart_id);

    currentline[lineidx] = c;
    lineidx = lineidx + 1;
    if (lineidx >= MAXLINELENGTH)
    lineidx = MAXLINELENGTH -
              1; // ensure there is someplace to put the next received character

    if(c == '\n') {
        currentline[lineidx] = 0;

        if (currentline == line1) {
          currentline = line2;
          lastline = line1;
        } else {
          currentline = line1;
          lastline = line2;
        }

        // Serial.println("----");
        // Serial.println((char *)lastline);
        // Serial.println("----");
        lineidx = 0;
        recvdflag = true;
        recvdTime = to_ms_since_boot(get_absolute_time()); // time we got the end of the string
        sentTime = firstChar;
        firstChar = 0; // there are no characters yet
        return c;      // wait until next character to set time
    }

    if (firstChar == 0) firstChar = tStart;
    return c;
}

void AdafruitGPS::sendCommand(const char *str) { 
    uart_puts(uart_id, str);      // send the string
    uart_puts(uart_id, "\r\n");   // println adds a newline
}

bool AdafruitGPS::newNMEAreceived(void) { return recvdflag; }

void AdafruitGPS::pause(bool p) { paused = p; }

char *AdafruitGPS::lastNMEA(void) {
  recvdflag = false;
  return (char *)lastline;
}