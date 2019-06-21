#include <FastLED.h>

typedef uint8_t uint8;

#define NUM_LEDS 70
#define USED_LEDS 70
#define HALF_LEDS 35
#define TOP_IDX_0 23
#define TOP_IDX_1 46
#define TOP_IDX_2 USED_LEDS
#define INC_HUE false

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
        if(INC_HUE)
        {
            hue++;
        }
    }

    for (int i = TOP_IDX_0; i < TOP_IDX_1; i++)
    {
        uint8 _hue = hue + hsv_one_third;
        leds[i] = CHSV(_hue, saturation, value);
        if(INC_HUE)
        {
            hue++;
        }
    }

    for (int i = TOP_IDX_1; i < TOP_IDX_2; i++)
    {
        uint8 _hue = hue + hsv_two_third;
        leds[i] = CHSV( _hue, saturation, value);
        if(INC_HUE)
        {
            hue++;
        }
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
        if(INC_HUE)
        {
            hue++;
        }
    }

    for (int i = TOP_IDX_1; i < TOP_IDX_2; i++)
    {
        uint8 _hue = hue + hsv_one_twelfth;
        leds[i] = CHSV( _hue, saturation, value);
        if(INC_HUE)
        {
            hue++;
        }
    }
    FastLED.show();
}

void led_write_analogous(uint8 hue, uint8 saturation=255, uint8 value=255)
{
    for (int i = 0; i < TOP_IDX_0; i++)
    {
        uint8 _hue = hue - hsv_one_twelfth;
        leds[i] = CHSV(_hue, saturation, value);
        if(INC_HUE)
        {
            hue++;
        }
    }

    for (int i = TOP_IDX_0; i < TOP_IDX_1; i++)
    {
        leds[i] = CHSV(hue, saturation, value);
        if(INC_HUE)
        {
            hue++;
        }
    }

    for (int i = TOP_IDX_1; i < TOP_IDX_2; i++)
    {
        uint8 _hue = hue + hsv_one_twelfth;
        leds[i] = CHSV( _hue, saturation, value);
        if(INC_HUE)
        {
            hue++;
        }
    }
    FastLED.show();
}

void led_write_complementary(uint8 hue, uint8 saturation=255, uint8 value=255)
{
    for (int i = 0; i < HALF_LEDS; i++)
    {
        leds[i] = CHSV(hue, saturation, value);
        if(INC_HUE)
        {
            hue++;
        }
    }
    for (int i = HALF_LEDS; i < USED_LEDS; i++)
    {
        uint8 _hue = hue + hsv_half;
        leds[i] = CHSV( _hue, saturation, value);
        if(INC_HUE)
        {
            hue++;
        }
    }
    FastLED.show();
}


void update_serial()
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
            int bpm = cmd.toInt();
            test_code_speed = bpm;
            //test_code_speed = map(bpm, 120, 200, 5, 80);
            Serial.println(30);
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
}


void loop()
{
	
    for (uint8 i = 0; i < 255; i++)
    {
        led_write_triad(i);
        delay(test_code_speed);
        update_serial();

    }
    for (uint8 i = 0; i < 255; i++)
    {
        led_write_analogous(i);
        delay(test_code_speed);
        update_serial();

    }
    for (uint8 i = 0; i < 255; i++)
    {
        led_write_split_complementary(i);
        delay(test_code_speed);
        update_serial();

    }
    for (uint8 i = 0; i < 255; i++)
    {
        led_write_complementary(i);
        delay(test_code_speed);
        update_serial();
    }

}


