The system pinout and connections are as follows:
Sensors: SHT15 - [SCL SDA],[D3 D4]
         DHT11 - D3? (Connect if sht is absent)
         BH1750 - [SCL SDA]
         ACS712 - A0

Indicators: RED_LED - D5
            BLUE_LED - D6

Display: LCD(I2C) - [SCL SDA],[A4 A5]

Connectivity: Sim8001 - [Tx Rx],[D1 D2]

Actuators: Buzzer - D7
           Fan - D8

Relay: Pump - D9
       Solenoid? - D10

Buttons: Auto - D11
         Manual - D12

EC:FA:BC:13:A4:99

Organization ID: kmqbc6
Device Type: ESP8266
Device ID: ECFABC13A499
Authentication Method: use-token-auth
Authentication Token: kZXdbfoTpQ1wggOPk4
