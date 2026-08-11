
#define IN1   14
#define IN2   12
#define PWM   27
#define STBY  13

#define ENC_A 34     
#define ENC_B 35     

volatile long encoderCount = 0;


void encoderISR()
{
    if (digitalRead(ENC_B))
        encoderCount++;
    else
        encoderCount--;
}

void motor_init()
{
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(PWM, OUTPUT);
    pinMode(STBY, OUTPUT);

    pinMode(ENC_A, INPUT_PULLUP);
    pinMode(ENC_B, INPUT_PULLUP);

    

    attachInterrupt(digitalPinToInterrupt(ENC_A), encoderISR, CHANGE);
}

void move_forward(int speed)
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(PWM, speed);
    digitalWrite(STBY, HIGH);
}

void move_backward(int speed)
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(PWM, speed);
    digitalWrite(STBY, HIGH);
}

void stop_motor()
{
    analogWrite(PWM, 0);
}

long getEncoderCount()
{
    noInterrupts();
    long count = encoderCount;
    interrupts();
    return count;
}

void resetEncoder()
{
    noInterrupts();
    encoderCount = 0;
    interrupts();
}