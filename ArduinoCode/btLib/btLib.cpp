#include "btLib.h"

/*
 * de begin functie initialized het object.
 * als deze functie een tweede keer gecalled word word die call genegeerd
 */

void btLib::begin(Stream *stream, Stream *debug_out = NULL)
{
    if(!did_init)
    {
        if (debug_out)
        {
            has_debug = true;
            debug_stream = debug_out;
        }
        else
        {
            has_debug = false;
        }

        _stream = stream;

        map_uint8_t = MiniDictUint8(  );
        map_int = MiniDictInt( );
        map_bool = MiniDictBool( );

        did_init = true;
    }

}


/*
 * de add recieve functies zijn voor het toevoegen van vars aan de hashmaps.
 * als een naam niet in de hashmap staat dan worddat bericht genegeerd
 */
bool btLib::add_recieve_uint8(String name, uint8_t default_value = 0)
{
    if(map_uint8_t.spots_remaining() > 0)
    {
        map_uint8_t.add(name, default_value);

        return true;
    }
    else
    {
        return false;
    }
}

bool btLib::add_recieve_int(String name, int default_value = 0)
{
    if(map_int.spots_remaining() > 0)
    {
        map_int.add(name, default_value);
        return true;
    }
    else
    {
        return false;
    }
}

void btLib::add_recieve_bool(String name, bool default_value = false)
{
    println_int_debug("bulubby", map_bool.spots_remaining());
    if(map_bool.spots_remaining() > 0)
    {
        map_bool.add(name, default_value);
    }

}


/*
 * haalt waardes uit de hashmap als de waarde bestaat.
 * als de waarde niet gevonden is word NULL gereturnt
 */
uint8_t btLib::get_uint8(String name)
{
    return map_uint8_t.get(name);
}

int btLib::get_int(String name)
{
    return map_int.get(name);
}

bool btLib::get_bool(String name)
{
    return map_bool.get(name);
}


/*
 * deze methodes versturen de waardes over de serial.
 * het zou mooi zijn als dit gerefactored zou kunnen worden zodat
 * het gernerics gebruikt in plaats van de types te hardcoden.
 * het probleem is dat de ontvanger het ook moet kunnen snappen,
 * de type chars moeten zowiezo gehardcode worden, want als dat niet matcht
 * snapt de parser het aan de andere kant niet.
 * dit zou ik natuurlijk kunnen veranderen, maar dat zou ontstettend veel
 * werk zijn, en is in dit geval niet nodig.
 */
void btLib::send_uint8(String name, uint8_t value)
{
    init_send();
    uint8_t msg_len = name.length() + SIZE_UINT8_T + TYPE_MARKER_SIZE;
    _stream->write(msg_len);
    _stream->write(type_chars._uint8);
    _stream->write(value);
    send_name(name);
}

void btLib::send_int(String name, int value)
{
    init_send();
    uint8_t msg_len = name.length() + SIZE_INT + TYPE_MARKER_SIZE;
    _stream->write(msg_len);
    _stream->write(type_chars._int);
    _stream->write(value);
    _stream->write((value >> 8));
    send_name(name);
}

void btLib::send_bool(String name, bool value)
{
    init_send();
    uint8_t msg_len = name.length() + SIZE_BOOL + TYPE_MARKER_SIZE;
    _stream->write(msg_len);
    _stream->write(type_chars._bool);
    _stream->write((uint8_t) value);
    send_name(name);
}

void btLib::init_send()
{
    _stream->write(HEADER_1);
    _stream->write(HEADER_2);
}

void btLib::send_name(String name)
{
    // verstuurd de naam
    // de +1 is omdat de strings null terminated zijn
    int name_len = name.length();
    char buf[name_len+1];
    name.toCharArray(buf, name_len+1);
    for (int i=0; i < name_len; i++)
    {
        _stream->write(buf[i]);
    }
}




// todo is naam index nodig?
// is data length niet genoeg
void btLib::update()
{
    // deze methode leest de bytes 1 voor 1 uit de serial buffer.
    while (_stream->available() > 3)   // er is een rede voor de >3 want de andere lib deed het.
    {
        if (transfer_phase ==  READING_HEADER1)
        {
            uint8_t b = _stream->read();
            if((b) == HEADER_1)
            {
                // als header byte gevonden next phase
                println_string_debug(String("header1"));
                transfer_phase = READING_HEADER2;
            }
            else
            {
                // als header bytes niet gevonden trash buffer todat je 'm wel vind
                println_string_debug(String((char)b));
                transfer_phase = TRANSFER_FAILED;
                debug.trashed_bytes++;
            }
        }
        else if (transfer_phase ==  READING_HEADER2)
        {
            if(_stream->read() == HEADER_2)
            {
                // als header byte gevonden next phase
                println_string_debug(String("header2"));
                transfer_phase = READING_LEN;
            }
            else
            {
                // als header bytes niet gevonden trash buffer todat je 'm wel vind
                transfer_phase = TRANSFER_FAILED;
                debug.trashed_bytes + 2;
            }
        }
        else if (transfer_phase ==  READING_LEN)
        {
            // neemt de byte als len en gaat naar de volgende phase
            println_string_debug(String("len"));
            recieved.data_len = _stream->read();
            transfer_phase =  READING_TYPE;
        }
        else if (recieved.data_idx < recieved.data_len && transfer_phase > READING_LEN)
        {
            if (transfer_phase ==  READING_TYPE)
            {
                // pakt de eerstvolgede byte als het type.
                // als de byte niet van een type is word de transfer als failed verklaard;
                // als de byte valide is word de size ervan gepakt.
                println_string_debug(String("type"));
                char type = (char)_stream->read();
                if (type == type_chars._uint8 || type == type_chars._int || type == type_chars._bool)
                {
                    recieved.type_char = type;
                    if(type == type_chars._int)
                    {
                        recieved.type_len = SIZE_INT;
                    }
                    else if(type == type_chars._uint8)
                    {
                        recieved.type_len = SIZE_UINT8_T;
                    }
                    else if(type == type_chars._bool)
                    {
                        recieved.type_len = SIZE_BOOL;
                    }
                    recieved.name_len = recieved.data_len - recieved.type_len - TYPE_MARKER_SIZE;
                    
                    if (recieved.name_len <= NAME_MAX_LEN)
                    {
						transfer_phase = READING_VAL;
						recieved.data_idx++;
					}
					else
					{
						// als de naam te lang is lijd dat tot een buffer overflow
						transfer_phase = TRANSFER_FAILED;
					}
                }
                else
                {
                    transfer_phase = TRANSFER_FAILED;
                    debug.wrong_type++;
                }
            }
            else if (transfer_phase ==  READING_VAL)
            {
                // leest de waarde uit de byte stream
                println_string_debug(String("val"));
                recieved.val[recieved.val_idx] = _stream->read();
                recieved.val_idx++;
                recieved.data_idx++;
                if(!(recieved.val_idx < recieved.type_len ))
                {
                    recieved.val_idx = 0;
                    recieved.name_idx = 0;
                    transfer_phase = READING_NAME;
                }

            }
            else if (transfer_phase ==  READING_NAME)
            {
                // leest de naam uit de byte stream
                recieved.name_buf[recieved.name_idx] = _stream->read();
                println_int_debug(String("name"), recieved.name_buf[recieved.name_idx]);

                recieved.name_idx++;
                recieved.data_idx++;
            }
            else
            {

            }
        }
        else if (transfer_phase ==  TRANSFER_FAILED)
        {
            // als de transfer mislukt
            println_string_debug(String("failed"));
            debug.failed_transfers++;
            recieved.data_len = 0;
            recieved.type_len = 0;
            recieved.name_len = 0;
            recieved.name_idx = 0;
            recieved.data_idx = 0;
            recieved.val_idx = 0;
            transfer_phase = READING_HEADER1;
        }
        else
        {
            if (transfer_phase == READING_NAME)
            {
                println_string_debug(String("success"));

                // sucsess update hashmaps
                // hier word de data naar het juiste type gecast
                String name = "";
                for (int i=0; i < recieved.name_idx; i++)
                {
                    name += recieved.name_buf[i];
                }

                // recieved int
                if(recieved.type_char == type_chars._int)
                {
                    if(true)
                    {
                        // plakt de twee byte aan elkaar zodat je een 16 bit int krijgt
                        int val = recieved.val[0] | (int)recieved.val[1] << 8;
                        map_int.update(name, val);
                        println_string_debug(String("updated:")+ name);
                    }
                    else
                    {
                        println_string_debug(String("no update"));
                    }
                }
                // recieved
                else if(recieved.type_char == type_chars._uint8)
                {
                    if(true)
                    {

                        map_uint8_t.update(name, (uint8_t)recieved.val[0]);
                        println_string_debug(String("updated:" )+ name);
                    }
                    else
                    {
                        println_string_debug(String("no update"));
                    }
                }
                else if(recieved.type_char == type_chars._bool)
                {
                    if(true)
                    {
                        map_bool.update(name, (bool)recieved.val[0]);
                        println_string_debug(String("updated:" ) + name);
                    }
                    else
                    {
                        println_string_debug(String("no update"));
                    }
                }
                transfer_phase = TRANSFER_FAILED;
            }
            else
            {
                println_string_debug(String("failed: at end of msg before name is read"));
                transfer_phase = TRANSFER_FAILED;
            }
        }
    }
}

/*
 * debug print functies.
 */
void btLib::print_debug()
{
    println_int_debug("phase", (uint8_t)transfer_phase);
    println_int_debug("failed transfers", debug.failed_transfers);
    println_int_debug("trashed bytes", debug.trashed_bytes);
    println_int_debug("wrong_type", debug.wrong_type);
}

void btLib::println_string_debug(String str)
{
    if (debug_enabled)
    {
        str += "\n";
        uint16_t b_length = str.length() +1;
        char buf[b_length];
        str.toCharArray(buf, b_length);
        print_debug_buffer(buf);
    }


}

void btLib::println_int_debug(String name, int val)
{
    if (debug_enabled)
    {
        int b_size = name.length() + 10;
        char buffer[b_size];
        name += ":%d\n";
        sprintf(buffer, str_2_char(name), val);
        print_debug_buffer(buffer);

    }
}

void btLib::print_debug_buffer(char buffer[])
{
    if (debug_enabled)
    {
        int i = 0;
        char c = buffer[4];
        while(buffer[i] > 0)
        {
            debug_stream->write(buffer[i]);
            i++;
        }
    }
}

void btLib::debug_maps()
{
    //map_uint8_t.debug();
    //map_int.debug();
    //map_bool.debug();
}

/*
 * convert aruduino string naar char*
 * ik weet niet zeker of dit segfaults gaat veroorzaken
 */
char *btLib::str_2_char(String str)
{
    if(str.length()!=0)
    {
        char *p = const_cast<char*>(str.c_str());
        return p;
    }
    return NULL;
}



