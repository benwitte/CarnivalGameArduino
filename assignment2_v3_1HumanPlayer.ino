// coop-2-beat-the-comp

// each turn, the computer will choose randomly, one LED, in the his circle
// each player will need to light a parrallel LED in his circle - to beat the computer both of the players have to succeed - cooperate to beat the computer!

int comp_leds[] = {1,2,3,4,5, 6};
int player1_leds[] = {7,8,9,10,11, 12};
int button = 0; // button[0] = player1 control, button[1] = player2 control
bool player_won = true;
int comp_random_index; 

void setup() {

  for (int i = 0; i < 13; ++i)
        digitalWrite(i, LOW);

  Serial.begin(9600);
  randomSeed(analogRead(0));
  comp_random_index = random(6);

  for (int i=0; i<6; i++){
    pinMode(comp_leds[i], OUTPUT);
    pinMode(player1_leds[i], OUTPUT);
  }

    for (int i=0; i<2; i++){
      pinMode(button, INPUT_PULLUP);
  }
}

void loop() {

    // initialize a new challenge for the players
    if(player_won){
      player_won = false;
      winning_feedback();
      delay(1000);
 
      comp_random_index = random(6);
      computer_think_and_choose(comp_random_index);
    }

    // players trial
    int player1_random_index = random(6);
    player_won = players_trial(comp_random_index, player1_random_index);
}



void computer_think_and_choose(int random_index){
  // "thinking" UX - the comp LEDs will light sequently
  // full cycle following with a partial cycle that will stop the sequently lighting up at the LED which fits the random index
  // the chosen LED will light constantly at the second cycle
  for(int i = 0; i<(6+random_index-1); i++){
    digitalWrite(comp_leds[i%6], HIGH);
    delay(500);
    digitalWrite(comp_leds[i%6], LOW);
  }

  digitalWrite(comp_leds[random_index], HIGH);
  delay(500);
}


bool players_trial(int comp_random_index, int player1_index){
  
  int i = 0;
  bool player1_try = false;
  int speed1 = random(1,6);
  
  while(!player1_try){
    
    player1_index = (player1_index + i)%6;
    digitalWrite(player1_leds[player1_index], HIGH);
    delay(100*speed1);
    player1_try = buttonIsPressed(button);
    if(!player1_try){
      digitalWrite(player1_leds[player1_index], LOW);
      i = i+1;
      }   
  }

  player_won = (player1_index == comp_random_index); 
  if(!player_won){
    failing_feedback(comp_random_index, player1_index);
  }

  return player_won;

}

bool buttonIsPressed(int buttonPin){
  bool button_isPressed = false;
  bool pressed = !digitalRead(buttonPin);  // is the button is pressed? GND (LOW) is connected to the PIN (instead of the 5V (HIGH) with the internal 10ohm resistanse)
  if (pressed){
    button_isPressed = true;
    delay(10);                 // wait till the bouncing phonemanon is ended
    while(!digitalRead(buttonPin));    // wait till the button is unpressed
    delay(10);                // wait till the bouncing phonemanon is ended
  }

  return button_isPressed;
}

void winning_feedback(){
  for(int i=0; i<2; i++){
    for (int j=0; j<6; j++){
      digitalWrite(comp_leds[j], HIGH);
      digitalWrite(player1_leds[j], HIGH);
    }

    delay(500);

    for (int j=0; j<6; j++){
      digitalWrite(comp_leds[j], LOW);
      digitalWrite(player1_leds[j], LOW);
   }

 }
}

void failing_feedback(int com_led, int p1_led) {
  for(int i = 0; i<3; i++){
    digitalWrite(comp_leds[com_led], HIGH);
    digitalWrite(comp_leds[p1_led], HIGH);
  
    if(i == 2) {
      delay(1000);
      }
    else {
      delay(500);
      }

    digitalWrite(comp_leds[com_led], LOW);
    digitalWrite(comp_leds[p1_led], LOW);

    delay(500);
  }
}
