#include <Arduino.h>
#include <Shell.h>

// Threshold where denom starts to go negative
//   #define R_BIAS 2193
//   #define V_BIAS 2204

// Threshold where r2 starts to = 0
//   #define R_BIAS 2193
//   #define V_BIAS 2205

// Working
//   #define R_BIAS 2200
//   #define V_BIAS 2218

#define R_BIAS 2200                                    // ohms
#define V_BIAS 2218                                   // millivolts
#define DAC_RESOLUTION 12                            // bits
#define ADC_VREF 3000                               // milivolts
#define ADC_RESOLUTION 13                          // bits
#define ADC_MAX ((1<<ADC_RESOLUTION) - 1)         // bit shift @ 13 bits = 8191


////uint32_t raw_v = 350;
//uint32_t raw_v = analogRead(A0);
//uint32_t vout = (raw_v * ADC_VREF) / ADC_MAX;
//uint32_t denom = V_BIAS - vout;
//// R2 = (Vout x R1) / (Vin - Vout)
//uint32_t r2 = (vout*R_BIAS)/denom;
//uint32_t r2 = 255;

int command_help(int argc, char** argv);
int command_test(int argc, char** argv);
int command_extra(int argc, char** argv);
int shell_reader(char * data);
void shell_writer(char data);


// The custom start message function has a bug. Entering the shell via pio 
// monitor or minicom a few seconds after the teensy is power cycled produces 
// the correct result. Waiting to enter a shell causes the message to only 
// print the first 15 characters. 
char* msg = (char*)" Type help to list commands.";


void setup() {
    // Prepare serial communication
    Serial.begin(9600);
    // Wait after reset or power on...
    delay(3000); //This time affects the start message bug.

    // Setup Teensy pins
    analogWriteResolution(DAC_RESOLUTION);
    analogReadResolution(ADC_RESOLUTION);
    // Generate V_BIAS @ DAC0
    analogWrite(A21, (((uint32_t)2200) * ((1<<DAC_RESOLUTION) - 1) / 3300));

    // Initialize command line interface (CLI)
    // Here we pass the function pointers to the read and write functions.
    // We can also pass a char pointer to display a custom start message.
    //shell_init(shell_reader, shell_writer, 0);
    shell_init(shell_reader, shell_writer, msg);

    // Add commands to the shell
    shell_register(command_help, PSTR("help"));
    shell_register(command_test, PSTR("test"));
    shell_register(command_extra, PSTR("extra"));
}


void loop() {
    // This should always be called to process user input.
    shell_task();
}


// Test commands: The commands should always use this function prototype.
// They receive 2 parameters: The total count of arguments (argc) and a pointer
// to the begining of each one of the null-terminated argument strings.
int command_help(int argc, char** argv) {
    // Print formatted text to terminal
    shell_println("-----------------------------------------------");
    shell_println("List of Commands:");
    shell_println("-----------------------------------------------");
    shell_println("");
    shell_println(" > help ");
    shell_println("");
    shell_println(" > test play ");
    shell_println(" > test vol+ ");
    shell_println(" > test vol- ");
    shell_println(" > test reserved ");
    shell_println("");
    shell_println(" > test 1 ");
    shell_println(" > test 2 ");
    shell_println(" > extra ");
    shell_println("");
    shell_println("-----------------------------------------------");
    shell_println("Exit...");
    return SHELL_RET_SUCCESS;
}


int command_test(int argc, char** argv) {
    //shell_printf("%d - \"%s\" [len:%d]\r\n", 1, argv[1], strlen(argv[1]) );
    // Print formatted text to terminal  
    shell_println("");
    shell_println("");
    shell_printf("Running Test %s...\r\n", argv[1] );
    shell_println("");

    if (strcmp(argv[1], "play") == 0) {
        shell_println("Please press and hold the play button for 5 seconds.");
        shell_println("Waiting for 5 seconds.");
        shell_println("");
        delay (5000);

        uint32_t raw_v = analogRead(A0);
        uint32_t vout = (raw_v * ADC_VREF) / ADC_MAX;
        uint32_t denom = V_BIAS - vout;
        uint32_t r2 = (vout*R_BIAS)/denom;          // R2 = (Vout x R1) / (Vin - Vout)

        switch (r2) {
            case 0: {
                shell_println(" Play/Pause is in Spec");
                break;
            }
            case 1 ... 1000: {
                shell_println(" Play/Pause is not in Spec");
                break;
            }
            default: {
                shell_println(" Press the button you would like to test.");
                //shell_println(" r2:");
            }
        }
        shell_println("");
        shell_println("Exit...");
    }

    if (strcmp(argv[1], "vol+") == 0) {
        shell_println("Please press and hold the vol+ button for 5 seconds.");
        shell_println("Waiting for 5 seconds.");
        shell_println("");
        delay (5000);

        uint32_t raw_v = analogRead(A0);
        uint32_t vout = (raw_v * ADC_VREF) / ADC_MAX;
        uint32_t denom = V_BIAS - vout;
        uint32_t r2 = (vout*R_BIAS)/denom;          // R2 = (Vout x R1) / (Vin - Vout)

        switch (r2) {
            case 0 ... 215: {
                shell_println(" Vol+ is not in Spec");
                break;
            }
            case 216 ... 265: {
                shell_println(" Vol+ is in Spec");
                break;
            }
            case 266 ... 1000: {
                shell_println(" Vol+ is not in Spec");
                break;
            }
            default: {
                shell_println(" Press the button you would like to test.");
                //shell_println(" r2:");
            }
        }
        shell_println("");
        shell_println("Exit...");
    }

    if (strcmp(argv[1], "vol-") == 0) {
        shell_println("Please press and hold vol- button for 5 seconds.");
        shell_println("Waiting for 5 seconds.");
        shell_println("");
        delay (5000);

        uint32_t raw_v = analogRead(A0);
        uint32_t vout = (raw_v * ADC_VREF) / ADC_MAX;
        uint32_t denom = V_BIAS - vout;
        uint32_t r2 = (vout*R_BIAS)/denom;          // R2 = (Vout x R1) / (Vin - Vout)

        switch (r2) {
            case 0 ... 422: {
                shell_println(" Vol- is not in Spec");
                break;
            }
            case 423 ... 517: {
                shell_println(" Vol- is in Spec");
                break;
            }
            case 518 ... 1000: {
                shell_println(" Vol- is not in Spec");
                break;
            }
            default: {
                shell_println(" Press the button you would like to test.");
                //shell_println(" r2:");
            }
        }
        shell_println("");
        shell_println("Exit...");
    }

    if (strcmp(argv[1], "reserved") == 0) {
        shell_println("Please press and hold reserved button for 5 seconds.");
        shell_println("Waiting for 5 seconds.");
        shell_println("");
        delay (5000);

        uint32_t raw_v = analogRead(A0);
        uint32_t vout = (raw_v * ADC_VREF) / ADC_MAX;
        uint32_t denom = V_BIAS - vout;
        uint32_t r2 = (vout*R_BIAS)/denom;          // R2 = (Vout x R1) / (Vin - Vout)

        switch (r2) {
            case 0 ... 120: {
                shell_println(" Reserved is not in Spec");
                break;
            }
            case 121 ... 148: {
                shell_println(" Reserved is in Spec");
                break;
            }
            case 149 ... 1000: {
                shell_println(" Reserved is not in Spec");
                break;
            }
            default: {
                shell_println(" Press the button you would like to test.");
                //shell_println(" r2:");
            }
        }
        shell_println("");
        shell_println("Exit...");
    }

    if (strcmp(argv[1], "1") == 0) {
        while (1) {  
            //shell_printf("Running Test %s...\r\n", argv[1] );

            uint32_t raw_v = analogRead(A0);
            uint32_t vout = (raw_v * ADC_VREF) / ADC_MAX;
            uint32_t denom = V_BIAS - vout;
            uint32_t r2 = (vout*R_BIAS)/denom;      // R2 = (Vout x R1) / (Vin - Vout)

            shell_printf("r_v: %d", raw_v );
            shell_printf(" vout: %d", vout );
            shell_printf(" denom: %d", denom );
            shell_printf(" r2: %d\r\n", r2 );
        }
    }

    if (strcmp(argv[1], "2") == 0) {
        while (1) {
            uint32_t raw_v = analogRead(A0);
            uint32_t vout = (raw_v * ADC_VREF) / ADC_MAX;
            uint32_t denom = V_BIAS - vout;
            uint32_t r2 = (vout*R_BIAS)/denom;      // R2 = (Vout x R1) / (Vin - Vout)

            switch (r2) {
                case 0: {
                    shell_println(" Play/Pause is in Spec");
                    break;
                }
                case 216 ... 265: {
                    shell_println(" Vol+ is in Spec");
                    break;
                }
                case 423 ... 517: {
                    shell_println(" Vol- is in Spec");
                    break;
                }
                case 121 ... 148: {
                    shell_println(" Reserved is in Spec");
                    break;
                }
                default: {
                    shell_println(" Press the button you would like to test.");
                    //shell_println(" r2:");
                }
            }
        }
    }

    return SHELL_RET_SUCCESS;
}


int command_extra(int argc, char** argv) {
    shell_println("Running \"extra\" now");
    shell_println("Exit...");
    return SHELL_RET_SUCCESS;
}


// Function to read data from serial port
// Functions to read from physical media should use this prototype:
// int my_reader_function(char * data)
int shell_reader(char * data) {
    // Wrapper for Serial.read() method
    if (Serial.available()) {
        *data = Serial.read();
        return 1;
    }
    return 0;
}


// Function to write data to serial port
// Functions to write to physical media should use this prototype:
// void my_writer_function(char data)
void shell_writer(char data) {
    // Wrapper for Serial.write() method
    Serial.write(data);
}
