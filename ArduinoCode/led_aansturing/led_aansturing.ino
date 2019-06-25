#include <FastLED.h>

typedef uint8_t uint8;

#define NUM_LEDS 70
#define USED_LEDS 9
#define HALF_LEDS 5
#define TOP_IDX_0 3
#define TOP_IDX_1 6
#define TOP_IDX_2 USED_LEDS

#define DATA_PIN 11
#define CLOCK_PIN 13

// benodiged voor de aansturing van de leds
// MAAK HIER GEEN DEFINES VAN MAKEN KISHAN
const uint8 hsv_one_third = 85;
const uint8 hsv_two_third = hsv_one_third + hsv_one_third;
const uint8 hsv_half = 128;
const uint8 hsv_one_twelfth = 21;

String readString;
String read_buffer = "";
String cmd = "";


uint8 anim_steps = 10;
uint8 hsvs[3] = {0,0,0};
uint8 hsvs_new[3] = {0,0,0};


const int bufferSize = 10;
const char cmd_sep = '|';

uint8 test_code_speed = 5000;


uint8 hue = 0; // overflow bewust

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup()
{
    Serial.begin(115200);
    // Uncomment/edit one of the following lines for your leds arrangement.
    FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}


void led_write_triad(uint8 hue, uint8 saturation=255, uint8 value=255)
{
    for (int i = 0; i < TOP_IDX_0; i++)
    {
        leds[i] = CHSV(hue, saturation, value);

    }

    for (int i = TOP_IDX_0; i < TOP_IDX_1; i++)
    {
        uint8 _hue = hue + hsv_one_third;
        leds[i] = CHSV(_hue, saturation, value);

    }

    for (int i = TOP_IDX_1; i < TOP_IDX_2; i++)
    {
        uint8 _hue = hue + hsv_two_third;
        leds[i] = CHSV( _hue, saturation, value);

    }
    FastLED.show();
}





void led_write_hsvs(uint8 saturation=255, uint8 value=255)
{
    for (int i = 0; i < TOP_IDX_0; i++)
    {
        leds[i] = CHSV(hsvs[0], saturation, value);

    }

    for (int i = TOP_IDX_0; i < TOP_IDX_1; i++)
    {
        leds[i] = CHSV(hsvs[1], saturation, value);

    }

    for (int i = TOP_IDX_1; i < TOP_IDX_2; i++)
    {
        leds[i] = CHSV( hsvs[2], saturation, value);

    }
    FastLED.show();
}

void led_write_split_complementary (uint8 hue, uint8 saturation=255, uint8 value=255)
{
    for (int i = 0; i < TOP_IDX_0; i++)
    {
        uint8 _hue = hue - hsv_one_twelfth;
        leds[i] = CHSV(_hue, saturation, value);
    }

    for (int i = TOP_IDX_0; i < TOP_IDX_1; i++)
    {
        uint8 _hue = hue + hsv_half;
        leds[i] = CHSV(_hue, saturation, value);

    }

    for (int i = TOP_IDX_1; i < TOP_IDX_2; i++)
    {
        uint8 _hue = hue + hsv_one_twelfth;
        leds[i] = CHSV( _hue, saturation, value);

    }
    FastLED.show();
}

void led_write_analogous(uint8 hue, uint8 saturation=255, uint8 value=255)
{
    for (int i = 0; i < TOP_IDX_0; i++)
    {
        uint8 _hue = hue - hsv_one_twelfth;
        leds[i] = CHSV(_hue, saturation, value);

    }

    for (int i = TOP_IDX_0; i < TOP_IDX_1; i++)
    {
        leds[i] = CHSV(hue, saturation, value);

    }

    for (int i = TOP_IDX_1; i < TOP_IDX_2; i++)
    {
        uint8 _hue = hue + hsv_one_twelfth;
        leds[i] = CHSV( _hue, saturation, value);

    }
    FastLED.show();
}

void led_write_complementary(uint8 hue, uint8 saturation=255, uint8 value=255)
{
    for (int i = 0; i < HALF_LEDS; i++)
    {
        leds[i] = CHSV(hue, saturation, value);

    }
    for (int i = HALF_LEDS; i < USED_LEDS; i++)
    {
        uint8 _hue = hue + hsv_half;
        leds[i] = CHSV( _hue, saturation, value);

    }
    FastLED.show();
}


bool update_serial()
{
    if (Serial.available() && read_buffer.length() < bufferSize)
    {
        //Serial.println("Start Serial");
        char r_char = Serial.read(); // pakt een char van de serial en plakt hem aan de buffer
        read_buffer += r_char;
    }
    else if ((read_buffer.length() >= bufferSize) || read_buffer.indexOf(cmd_sep) > 0)
    {
        int cmd_sep_idx = read_buffer.indexOf(cmd_sep);
        if (cmd_sep_idx > 0)
        {
            cmd = read_buffer.substring(0, cmd_sep_idx);
            read_buffer = read_buffer.substring(cmd_sep_idx + 1);
            char _sep = '/';
            int prev_idx = cmd.indexOf(_sep);
            hsvs_new[0] = cmd.substring(0, prev_idx).toInt();
            cmd = cmd.substring(prev_idx +1);
            prev_idx = cmd.indexOf(_sep);
            hsvs_new[1] = cmd.substring(0, prev_idx).toInt();
            cmd = cmd.substring(prev_idx +1);
            hsvs_new[2] = cmd.toInt();
            int bpm = cmd.toInt();
            test_code_speed = bpm;
            //test_code_speed = map(bpm, 120, 200, 5, 80);
            return true;
        }
        else if (cmd_sep_idx == 0)
        {
            read_buffer = read_buffer.substring(1);
        }
        else
        {
            read_buffer = "";
        }
    }
    return false;
}


void loop()
{
    if(update_serial())
    {
        int d0 = ((int)hsvs_new[0] - (int)hsvs[0]) / 10;
        int d1 = ((int)hsvs_new[1] - (int)hsvs[1]) / 10;
        int d2 = ((int)hsvs_new[2] - (int)hsvs[2]) / 10;
        for (int i = 0; i < anim_steps-1; i++)
        {
            led_write_hsvs();
            hsvs[0] = (uint8)((int)hsvs[0] + d0);
            hsvs[1] = (uint8)((int)hsvs[1] + d1);
            hsvs[2] = (uint8)((int)hsvs[2] + d2);
            delay(5);
        }
        memcpy(&hsvs, &hsvs_new, sizeof(hsvs));
        led_write_hsvs();
    }
    delay(1);
}


