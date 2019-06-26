#include <FastLED.h>

typedef uint8_t uint8;




const uint8 NUM_LEDS = 70;




#define pwmpin1 7
#define pwmpin2 6
#define in_a1 2
#define in_a2 4
#define in_b1 3
#define in_b2 5

struct
{
    uint8 USED_LEDS = 30;
    uint8 HALF_LEDS = 15;
    uint8 THIRD_0 = 10;
    uint8 THIRD_1 = 20;
    uint8 THIRD_2 = USED_LEDS;

    const uint8 hsv_one_third = 85;
    const uint8 hsv_two_third = hsv_one_third + hsv_one_third;
    const uint8 hsv_half = 128;
    const uint8 hsv_one_twelfth = 21;

    uint8 anim_steps = 10;
    uint8 hsvs[3] = {0,0,0};
    uint8 hsvs_new[3] = {0,0,0};
    uint8 hsv_offset = 0;
    uint8 hue = 0; // overflow bewust
    int beat_delta_ms = 0;
    int bpm = 0;
    unsigned long timer_start = millis();
    int step = 0;
} led_vars;





String readString;
String read_buffer = "";
String cmd = "";
const int bufferSize = 10;
const char cmd_sep = '|';



// Define the array of leds
CRGB leds[NUM_LEDS];


void m1 (uint8 pwm, bool dir)
{
	analogWrite(pwmpin1, pwm);
	if (dir)
	{
		digitalWrite(in_a1, 0);
		digitalWrite(in_b1, 1);
	}
	else
	{
		digitalWrite(in_a1, 1);
		digitalWrite(in_b1, 0);
	}
}


void setup()
{
    Serial.begin(115200);
    // Uncomment/edit one of the following lines for your leds arrangement.
    FastLED.addLeds<WS2801, 8 /*data pin*/,  9 /*clock pin*/, RGB>(leds, NUM_LEDS);
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
    pinMode(pwmpin1, OUTPUT);
    pinMode(pwmpin2, OUTPUT);
    pinMode(in_a1, OUTPUT);
    pinMode(in_b1, OUTPUT);
    pinMode(in_a2, OUTPUT);
    pinMode(in_b2, OUTPUT);
    m1(100, true);
}


void led_write_triad(uint8 hue, uint8 saturation=255, uint8 value=255)
{
    for (int i = 0; i < led_vars.THIRD_0; i++)
    {
        leds[i] = CHSV(hue, saturation, value);

    }

    for (int i = led_vars.THIRD_0; i < led_vars.THIRD_1; i++)
    {
        uint8 _hue = hue + led_vars.hsv_one_third;
        leds[i] = CHSV(_hue, saturation, value);

    }

    for (int i = led_vars.THIRD_1; i < led_vars.THIRD_2; i++)
    {
        uint8 _hue = hue + led_vars.hsv_two_third;
        leds[i] = CHSV( _hue, saturation, value);

    }
    FastLED.show();
}





void led_write_hsvs(uint8 saturation=255, uint8 value=255)
{
    for (int i = 0; i < led_vars.THIRD_0; i++)
    {
        leds[i] = CHSV(led_vars.hsvs[0]+led_vars.hsv_offset, saturation, value);

    }

    for (int i = led_vars.THIRD_0; i < led_vars.THIRD_1; i++)
    {
        leds[i] = CHSV(led_vars.hsvs[1]+led_vars.hsv_offset, saturation, value);

    }

    for (int i = led_vars.THIRD_1; i < led_vars.THIRD_2; i++)
    {
        leds[i] = CHSV( led_vars.hsvs[2]+led_vars.hsv_offset, saturation, value);

    }
    FastLED.show();
}

void led_write_split_complementary (uint8 hue, uint8 saturation=255, uint8 value=255)
{
    for (int i = 0; i < led_vars.THIRD_0; i++)
    {
        uint8 _hue = hue - led_vars.hsv_one_twelfth;
        leds[i] = CHSV(_hue, saturation, value);
    }

    for (int i = led_vars.THIRD_0; i < led_vars.THIRD_1; i++)
    {
        uint8 _hue = hue + led_vars.hsv_half;
        leds[i] = CHSV(_hue, saturation, value);

    }

    for (int i = led_vars.THIRD_1; i < led_vars.THIRD_2; i++)
    {
        uint8 _hue = hue + led_vars.hsv_one_twelfth;
        leds[i] = CHSV( _hue, saturation, value);

    }
    FastLED.show();
}

void led_write_analogous(uint8 hue, uint8 saturation=255, uint8 value=255)
{
    for (int i = 0; i < led_vars.THIRD_0; i++)
    {
        uint8 _hue = hue - led_vars.hsv_one_twelfth;
        leds[i] = CHSV(_hue, saturation, value);

    }

    for (int i = led_vars.THIRD_0; i < led_vars.THIRD_1; i++)
    {
        leds[i] = CHSV(hue, saturation, value);

    }

    for (int i = led_vars.THIRD_1; i < led_vars.THIRD_2; i++)
    {
        uint8 _hue = hue + led_vars.hsv_one_twelfth;
        leds[i] = CHSV( _hue, saturation, value);

    }
    FastLED.show();
}

void led_write_complementary(uint8 hue, uint8 saturation=255, uint8 value=255)
{
    for (int i = 0; i < led_vars.HALF_LEDS; i++)
    {
        leds[i] = CHSV(hue, saturation, value);

    }
    for (int i = led_vars.HALF_LEDS; i < led_vars.USED_LEDS; i++)
    {
        uint8 _hue = hue + led_vars.hsv_half;
        leds[i] = CHSV( _hue, saturation, value);

    }
    FastLED.show();
}

void emotie_boos(int ms)
{
    if (led_vars.step == 0)
    {
        led_write_analogous(0);
        FastLED.show();
        led_vars.step++;
        led_vars.timer_start = millis();
    }
    else if (led_vars.step == 1)
    {
        if (millis() - led_vars.timer_start  > ms)
        {
            for (int i = 0; i < NUM_LEDS; i++)
            {
                leds[i] = CRGB::Black;
            }
            FastLED.show();
            led_vars.timer_start = millis();
            led_vars.step++;
        }        
    }
    else if (led_vars.step == 2)
    {
		for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = CRGB::Black;
        }        
        FastLED.show();
        
        led_vars.timer_start = millis();
        led_vars.step++;

	}
	else if (led_vars.step == 3)
	{
		if (millis() - led_vars.timer_start  > ms)
        {

            led_vars.step = 0;
        }
	}
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
            if (cmd.indexOf(_sep) > -1)
            {
                int prev_idx = cmd.indexOf(_sep);
                led_vars.hsvs_new[0] = (int)((float)cmd.substring(0, prev_idx).toInt() * 1.3);
                cmd = cmd.substring(prev_idx +1);
                prev_idx = cmd.indexOf(_sep);
                led_vars.hsvs_new[1] = (int)((float)cmd.substring(0, prev_idx).toInt() * 1.3);
                cmd = cmd.substring(prev_idx +1);
                led_vars.hsvs_new[2] = cmd.toInt();
            }
            else
            {
                led_vars.bpm = cmd.toInt();
                led_vars.beat_delta_ms = 60000 / led_vars.bpm;
                Serial.println(led_vars.beat_delta_ms);
            }
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





void m2 (uint8 pwm, bool dir)
{
	analogWrite(pwmpin1, pwm);
	if (dir)
	{
		digitalWrite(in_a1, 0);
		digitalWrite(in_a2, 1);
	}
	else
	{
		digitalWrite(in_a1, 1);
		digitalWrite(in_a2, 2);
	}
}

void loop()
{
    update_serial();

    if (led_vars.beat_delta_ms > 0)
    {

        emotie_boos(led_vars.beat_delta_ms);
    }

    //emotie_boos();

}


