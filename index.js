const mqtt = require("mqtt").connect(`mqtt://test.mosquitto.org:1883`);
const TOPIC = "helha/iot/tp2";

mqtt.on("connect", () => {
    console.log("Connected to MQTT broker");
    mqtt.subscribe(TOPIC);
});

mqtt.on("message", (topic, message) => {
    console.log("<" + new Date().toISOString().split("T").join(" ") + "> " + message.toString());
});

// Reconnexion automatique toutes les 10 secondes si déconnecté
setInterval(() => {
    while (!mqtt.connected) {
        mqtt.reconnect();
    }
}, 10000);
