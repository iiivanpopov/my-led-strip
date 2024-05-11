#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"

ESP8266WebServer server(80);

const char* ssid = "TP-Link_DE18";
const char* password = "70017366";

// Command constants
const int CMD_RAINBOW = 1;
const int CMD_PINK = 2;
const int CMD_RED = 3;
const int CMD_PURPLE = 4;
const int CMD_TURN_OFF = 5;
const int CMD_TURN_ON = 6;
const int CMD_PULSE = 7;
const int CMD_RUNNING = 8;
const int CMD_BRIGHTNESS_DOWN = 9;
const int CMD_BRIGHTNESS_UP = 10;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="UTF-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<title>Controls</title>
		<style>
			* {
				margin: 0;
				padding: 0;
				box-sizing: border-box;
			}
			body {
				font-family: sans-serif;
				font-weight: 700;
				font-size: 24px;
				background: #000;
				color: #fff;
			}
			.wrapper {
				display: grid;
				width: 100%;
				margin: 50px auto 0 auto;
				padding: 0 20px;
				justify-content: center;
				grid-template-columns: 1fr;
				gap: 20px;
			}
			button {
				background: #fff;
				padding: 10px 20px;
				font-size: 20px;
				font-weight: 700;
				outline: none;
				border: none;
				display: flex;
				justify-content: center;
				align-items: center;
				border-radius: 5px;
				transition: transform 0.3s ease-in-out;
			}
			button:hover {
				transform: scale(1.1);
			}

			.button-rainbow, .button-running {
				background: linear-gradient(
					90deg,
					rgba(255, 0, 0, 1) 0%,
					rgba(255, 154, 0, 1) 10%,
					rgba(208, 222, 33, 1) 20%,
					rgba(79, 220, 74, 1) 30%,
					rgba(63, 218, 216, 1) 40%,
					rgba(47, 201, 226, 1) 50%,
					rgba(28, 127, 238, 1) 60%,
					rgba(95, 21, 242, 1) 70%,
					rgba(186, 12, 248, 1) 80%,
					rgba(251, 7, 217, 1) 90%,
					rgba(255, 0, 0, 1) 100%
				);
			}

      .button-running {
        animation: running 5s linear infinite;
      }

      @keyframes running {
          0% {
              background-position: 0% 50%;
          }
          100% {
              background-position: 100% 50%;
          }
        }

			.button-pink {
				background-color: #e858dc;
			}
			.button-red {
				background-color: #d71919;
			}
			.button-purple {
				background-color: #8519d7;
			}
			.button-plus {
				background-color: rgb(35, 182, 79);
			}
			.button-minus {
				background-color: rgb(219, 69, 64);
			}
			.button-pulse {
				animation: pulse infinite 2s;
			}
			@keyframes pulse {
				0% {
					background-color: #007bff;
				}
				50% {
					background-color: #0056b3;
				}
				100% {
					background-color: #007bff;
				}
			}
			.button-on {
				background-color: #4caf50;
			}
			.button-off {
				background-color: #d32f2f;
			}
			.heading {
				color: #fff;
				text-align: center;
			}
			.color,
			.brightness,
			.effects,
			.on-off {
				display: grid;
				gap: 20px;
			}

			@media (min-width: 800px) {
				.wrapper {
					grid-template-columns: 3fr 1fr;
					padding: 0 100px;
				}
			}
		</style>
	</head>
	<body>
		<div class="wrapper">
			<div class="color">
				<h3 class="heading">Color</h3>
				<button class="button-rainbow" onclick="fetch('/rainbow')">Rainbow</button
				><button class="button-pink" onclick="fetch('/pink')">Pink</button
				><button class="button-red" onclick="fetch('/red')">Red</button
				><button class="button-purple" onclick="fetch('/purple')">Purple</button>
			</div>
			<div class="brightness">
				<h3 class="heading">Brightness</h3>
				<button class="button-plus" onclick="fetch('/brUp')">+</button
				><button class="button-minus" onclick="fetch('/brDown')">-</button>
			</div>
			<div class="effects">
				<h3 class="heading">Effects</h3>
				<button class="button-pulse" onclick="fetch('/pulse')">Pulse</button>
        <button class="button-running" onclick="fetch('/running')">Run</button>
			</div>
			<div class="on-off">
				<h3 class="heading">On/Off</h3>
				<button class="button-on" onclick="fetch('/on')">ON</button
				><button class="button-off" onclick="fetch('/off')">OFF</button>
			</div>
		</div>
	</body>
</html>
)rawliteral";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    server.on("/", handleRootPath);
    setupCommandHandlers();

    server.begin();
}

void loop() {
    server.handleClient();
}

void handleRootPath() {
    server.send(200, "text/html", index_html);
}

void setupCommandHandlers() {
    handleCommand("/on", CMD_TURN_ON);
    handleCommand("/off", CMD_TURN_OFF);
    handleCommand("/rainbow", CMD_RAINBOW);
    handleCommand("/pink", CMD_PINK);
    handleCommand("/red", CMD_RED);
    handleCommand("/purple", CMD_PURPLE);
    handleCommand("/brUp", CMD_BRIGHTNESS_UP);
    handleCommand("/brDown", CMD_BRIGHTNESS_DOWN);
    handleCommand("/pulse", CMD_PULSE);
    handleCommand("/running", CMD_RUNNING);
}

void handleCommand(String endpoint, int command) {
    server.on(endpoint, [command]() {
        Serial.println(command);
    });
}