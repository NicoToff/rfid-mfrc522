require("dotenv").config();
const mqtt = require("mqtt").connect(`mqtt://${process.env.RASPBBERRY_IP}:1883`, {
    username: process.env.MQTT_USERNAME,
    password: process.env.MQTT_PASSWORD,
});

const TOPIC = "IoT/TP2";

mqtt.on("connect", () => {
    console.log("Connected to MQTT broker");
    mqtt.subscribe(TOPIC);
});

mqtt.on("message", (topic, message) => {
    console.log("Message received: ", message.toString());
});

// Publish a message every 5 seconds
setInterval(() => {
    mqtt.publish(TOPIC, "Hello World!");
}, 5000);
