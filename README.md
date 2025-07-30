# WEATHER MONITORING SYSTEM

*NAME*: KOLLURI BHANUTEJA

## The Weather Forecasting System is a smart IoT-based solution developed using an ESP32 microcontroller to monitor real-time weather conditions and display both local sensor data and global weather forecasts through a web dashboard. The primary motivation behind this project is to combine local environmental sensing with external forecast data to give users a complete view of their surroundings. This project has practical applications in agriculture, outdoor event planning, and smart city development.

The system integrates a DHT11 sensor to capture real-time temperature and humidity, and an analog rain sensor to detect the presence of rainfall. These sensors are connected to an ESP32 board, which acts as a web server. When the ESP32 is connected to a Wi-Fi network (such as a mobile hotspot), users can access the live weather dashboard by entering the ESP32's IP address in any browser.

To enhance the usefulness of the system, it fetches 5-day weather forecasts from the OpenWeatherMap API using HTTP GET requests. The API provides a JSON response containing detailed weather information such as temperature, weather conditions, and forecast icons. The code uses the ArduinoJson library to parse this JSON data and display it meaningfully on the webpage hosted by the ESP32.

The web interface is built using responsive HTML and CSS and includes:

Real-time temperature and humidity values from the DHT11 sensor
Rain detection status from the analog rain sensor
Current weather description (e.g., “clear sky”, “rain”, etc.)
Forecast for the next 5 days with corresponding weather icons and temperatures
A scrolling banner showcasing the names and roll numbers of the team members who developed the project
The main city featured is Vijayawada, with coordinates hardcoded into the project. However, the system is designed in a modular way that allows for future upgrades, such as adding a search bar to dynamically fetch forecasts for multiple cities by entering their names.

The system uses WiFi.h and WebServer.h for networking and hosting, DHT.h for the temperature/humidity sensor, and ArduinoJson.h for parsing the API data. This makes the code lightweight and efficient, perfectly suited for the memory constraints of the ESP32 platform.

In addition to being a demonstration of how embedded systems can interact with web APIs, this project serves as a complete example of integrating hardware sensing with software visualization. It not only teaches IoT basics but also showcases practical skills in real-world web interfacing, HTTP requests, and sensor data acquisition.

The Weather Forecasting System stands out for its simplicity, real-time capabilities, and potential for customization. It proves how IoT and cloud services can work together to deliver meaningful, real-time data to end users, all hosted on a small, efficient device like the ESP32.

# OUTPUT
<img width="1440" height="900" alt="Image" src="https://github.com/user-attachments/assets/eb2cf1b0-b1ac-47de-85cd-31fd2bb35d12" />
