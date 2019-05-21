String read_buffer = "";
const int buffer_size = 10;
const char cmd_sep = '|';

void setup()
{
  Serial.begin(9600);
  //while (!Serial.available()) {}  // wacht tot serial beschikbaar is.
  Serial.println("ready");;
}

void loop() {
  //Wait for data to arrive
  //Serial read section
    if (Serial.available() && read_buffer.length() < buffer_size ){
        char r_char = Serial.read();  // pakt een char van de serial en plakt hem aan de buffer
        read_buffer += r_char;
        //Serial.print(r_char);
    }else if((read_buffer.length() >= buffer_size) || read_buffer.indexOf(cmd_sep) > 0){
        int cmd_sep_idx = read_buffer.indexOf(cmd_sep);
        if(cmd_sep_idx > 0){
            // pakt commando en slicet de buffer
            String cmd = read_buffer.substring(0, cmd_sep_idx);
            read_buffer = read_buffer.substring(cmd_sep_idx + 1);
            execute_command(cmd);
        }else if (cmd_sep_idx == 0){
            read_buffer = read_buffer.substring(1);
        }else{
            Serial.println("recieved garbage clearing buffer");
            read_buffer = "";
        }
    }
    delay(1); // 1ms delay zodat ie 'm niet flipt
}

void execute_command(String command) {
  //Command can be max 5 characters long
  String str = "command: ";
  if (command == "forw" || command == "back" || command == "left" || command == "right") {
    Serial.println(command + "moving" + command);
  } else if (command == "dance") {
    Serial.println(str + "dance");
  } else if (command == "dancl") {
    Serial.println(str + "dancl");
  } else if (command == "blink") {
    Serial.println(str + "blink");
  } else if (command == "marm") {
    Serial.println(str + "marm");
  }else{
      Serial.println(str + "unknown " + command);
  }
}
