/*****************************************************************************************
 *    RG 101 - Serial Communication     ______
 *    Number Guesser Game              | >_   |   
 *         With PWM LED                |      |   
 *                                     |______|   
 *                                                 
 *                              
 *  This code will genrerate a random number and prompt you to guess the number. When the
 *  user makes a guess, the LED will change brightness
 *  
 * 
 *    
 * For a more in depth explanation of wiring,  this code and more information on using 
 * serial communication and Arduino check out ourserial communication tutorial
 * http://learn.robotgeek.com/robotgeek-101-1/245-robotgeek-101-serial-communication.html
 *  
 * Important Functions
 *  Serial.begin()
 *  Serial.println()
 *  Serial.available()
 *  Serial.read()
 *  Serial.parseInt()
 *  Serial.readString()
 *  rand()
 *  digitalRead()
 *  digitalWrite()
 *  analogRead()
 *  analogWrite()
 *  
 *****************************************************************************************/

const int LED_PIN = 3;      //digital pin number for LED
const int BUTTON_PIN = 2;   //digital pin number for button
const int KNOB_PIN = 0;     //analog pin number for knob
const int RANDOM_PIN = 7;   //analog pin number for random number generator

char incomingData;  //create an empty chracter variable to hold the incoming data, one character at a time
String nameString;  //holds the name of the user as a String (an series of characters)

bool startFlag = false;  //a flag to check if the user has started the game / put in their name yet, set to false to assume they have not. We will also use this to reset the program if the button is pressed
int randomNumber;      //the random number that the user is trying to guess
int guess;             //the last guess the user entered
int randomMax;         //maximum random nuber for the current game
int guessDifference;   //difference between guess and random number, used to deterimine the hint LED brightness
int ledValue = 0;       //the brightness of the led fromm 0-255

//setup runs once when the Geekduino/Arduino is turned on
void setup()
{
  pinMode(LED_PIN, OUTPUT);   //set LED as an output
  pinMode(BUTTON_PIN, INPUT); //set button as an input
  
  digitalWrite(LED_PIN, LOW); //turn the LED off

  randomSeed(analogRead(RANDOM_PIN));        //seed the random number generator based on a 'random' reading from an unconnected analog input.
  
  Serial.begin(9600);   //start the Serial port at a baud rate of 9600 bits per second (bps)
 
  //uncomment the next 2 lines if you want to see the random number for debugging
  //Serial.print("Random Number: "); 
  //Serial.println(randomNumber); 

} //setup() is done, go to loop()

//loop runs forever once setup is complete
void loop()
{


   /*************************************************************************
   * loop while the nameFlag is still false (i.e. no name has been entered)
   *************************************************************************/
   if(startFlag == false)
   {

    ledValue = 255;                   //reset the ledvalue to 0, i.e. off
    analogWrite(LED_PIN, ledValue); //write the LED value to the LED pin
    
     //print ASCII art text
    Serial.println("   ");
    Serial.println("   "); 
    Serial.println("   ");
    Serial.println("    /$$   /$$                         /$$                                  /$$$$$$                                                        "    );
    Serial.println("  | $$$ | $$                        | $$                                 /$$__  $$                                                        "   );
    Serial.println("  | $$$$| $$ /$$   /$$ /$$$$$$/$$$$ | $$$$$$$   /$$$$$$   /$$$$$$       | $$  \\__/ /$$   /$$  /$$$$$$   /$$$$$$$ /$$$$$$$  /$$$$$$   /$$$$$$" );
    Serial.println("  | $$ $$ $$| $$  | $$| $$_  $$_  $$| $$__  $$ /$$__  $$ /$$__  $$      | $$ /$$$$| $$  | $$ /$$__  $$ /$$_____//$$_____/ /$$__  $$ /$$__  $$");
    Serial.println("  | $$  $$$$| $$  | $$| $$ \\ $$ \\ $$| $$  \\ $$| $$$$$$$$| $$  \\__/      | $$|_  $$| $$  | $$| $$$$$$$$|  $$$$$$|  $$$$$$ | $$$$$$$$| $$  \\__/");
    Serial.println("  | $$\\  $$$| $$  | $$| $$ | $$ | $$| $$  | $$| $$_____/| $$            | $$  \\ $$| $$  | $$| $$_____/ \\____  $$\\____  $$| $$_____/| $$      ");
    Serial.println("  | $$ \\  $$|  $$$$$$/| $$ | $$ | $$| $$$$$$$/|  $$$$$$$| $$            |  $$$$$$/|  $$$$$$/|  $$$$$$$ /$$$$$$$//$$$$$$$/|  $$$$$$$| $$      ");
    Serial.println("  |__/  \\__/ \\______/ |__/ |__/ |__/|_______/  \\_______/|__/             \\______/  \\______/  \\_______/|_______/|_______/  \\_______/|__/      ");
    
  
    ledValue = 0;                   //reset the ledvalue to 0, i.e. off
    analogWrite(LED_PIN, ledValue); //write the LED value to the LED pin
    
    Serial.println("Please Enter Your Name"); //print a user prompt 
    
    randomMax = analogRead(KNOB_PIN) + 1;   //read the analog value. Add one so that the maximum random value will be at least 1
    randomNumber = random(0, randomMax);  //generate a random number from 0 to randomMax-1

    //while the startFlag is false, read for data.
    while(startFlag == false)
    {
      //see if data is available on the serial port - if data is available, we're assuming that the name data has come in.
      if(Serial.available() == true)
      {
        nameString = Serial.readString(); //read the data as a string
        
        //print prompts  
        Serial.print("Hello "); 
        Serial.print(nameString); 
        Serial.print("! Please enter a number between 0 and . "); 
        Serial.println(randomMax); 
        
        startFlag = true; //a string has been sucessfully read, so change the flag so that we can leave the while loop  and progress with the program
      }//end if
    }//end while
   }//end if



   /*************************************************************************
   * check if the button has been pressed so that we can restart the program
   * digitalRead() returns HIGH if the button is pressed and LOW if it is not
   *************************************************************************/
  if(digitalRead(BUTTON_PIN) == HIGH)
  {
    startFlag = false;
  }


   /*************************************************************************
   * check is there are any characters incoming on the serial buffer.
   * Serial.available() returns the number of characters that have been input
   * so check if there are more than 0 characters
   *************************************************************************/
  if(Serial.available() > 0)
  {
    guess = Serial.parseInt();      //Serial.parseInt() will take the chacarters in the buffer and try to create an decimal integer
    Serial.print("You Guessed: ");  //print static text
    Serial.println(guess);          //print the data that was recieved

    if(guess > randomNumber)
    {
      Serial.println("You Guess is too high");  //print static text
    }
    
    else if(guess < randomNumber)
    {
      Serial.println("You Guess is too low");  //print static text
    }
    
    else if(guess == randomNumber)
    {
      Serial.println("You guessed correctly!");  //print static text

      //pulse LED for winner 3 times
      for (int j = 0; j< 3; j++)
      { 
        //brightness up - count from 0 to 255 and write value to LED_PIN
        for(int i = 0; i <= 255; i++)
        {
          analogWrite(LED_PIN, i);  //use analogWrite to set the PWM for the LED_PIN
          delay(2);
        }
        //brightness down - count from 255 to 0 and write value to LED_PIN
        for(int i = 255; i >= 0; i--)
        { 
          analogWrite(LED_PIN, i);//use analogWrite to set the PWM for the LED_PIN
          delay(2);
        }
      
      }


    }//end else if

    guessDifference = abs(randomNumber - guess);            //find the difference between the random number and the guess. Use abs() to make all numbers positive (absolute value)
    ledValue = map(guessDifference, 0, randomMax, 255, 0);  //map the difference to PWM values for the LED
    analogWrite(LED_PIN, ledValue);                          //write the ledValue to the LED_PIN
  }//end serial.available() check

  
}//go back to the first line in loop()





